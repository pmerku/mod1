//
// Created by pmerku on 31/05/2021.
//

#include "Terrain.hpp"

#include <glm/glm.hpp>
#include <cmath>

#include "PerlinNoise.hpp"

using namespace opengl;

Terrain::Terrain() {
	originX = (chunkWidth  * xMapChunks) / 2 - chunkWidth / 2;
	originY = (chunkHeight * yMapChunks) / 2 - chunkHeight / 2;

	mapChunks = std::vector<GLuint>(xMapChunks * yMapChunks);

	for (int y = 0; y < yMapChunks; y++) {
		for (int x = 0; x < xMapChunks; x++) {
			generateMapChunk(mapChunks[x + y * xMapChunks], x, y);
		}
	}

	nIndices = chunkWidth * chunkHeight * 6;
}

Terrain::~Terrain() {
	for (unsigned int & mapChunk : mapChunks) {
		glDeleteVertexArrays(1, &mapChunk);
	}
	glDeleteBuffers(3, _vbo);
	glDeleteBuffers(1, &_ebo);
}

std::vector<int> Terrain::generateIndices() {
	std::vector<int> indices;

	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			int pos = x + y * chunkWidth;
			if (x == chunkWidth - 1 || y == chunkHeight - 1) {
				continue;
			} else {
				// Top triangle
				indices.push_back(pos + chunkWidth);
				indices.push_back(pos);
				indices.push_back(pos + chunkWidth + 1);
				// Bottom triangle
				indices.push_back(pos + 1);
				indices.push_back(pos + chunkWidth + 1);
				indices.push_back(pos);
			}
		}
	}
	return indices;
}

std::vector<float> Terrain::generateNoiseMap(int xOffset, int yOffset) {
	std::vector<float> noiseMap;
	std::vector<float> normalizedNoiseMap;
	std::vector<int> permutations = getPermutationVector();

	float amp = 1;
	float freq = 1;
	float maxPossibleHeight = 0;

	for (int i = 0; i < _octaves; i++) {
		maxPossibleHeight += amp;
		amp *= _persistence;
	}

	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			amp = 1;
			freq = 1;
			float noiseHeight = 0;
			for (int i = 0; i < _octaves; i++) {
				float xSample = (x + xOffset * (chunkWidth - 1)) / _noiseScale * freq;
				float ySample = (y + yOffset * (chunkHeight - 1)) / _noiseScale * freq;

				float perlinValue = gen::perlinNoise(xSample, ySample, permutations);
				noiseHeight += perlinValue * amp;

				amp *= _persistence;
				freq *= _lacunarity;
			}
			noiseMap.push_back(noiseHeight);
		}
	}

	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			normalizedNoiseMap.push_back((noiseMap[x + y * chunkWidth] + 1) / maxPossibleHeight);
		}
	}

	return normalizedNoiseMap;
}

std::vector<float> Terrain::generateVertices(const std::vector<float> &noiseMap) {
	std::vector<float> vertices;

	for (int y = 0; y < chunkHeight; y++) {
		for (int x = 0; x < chunkWidth; x++) {
			vertices.push_back(x);
			float easedNoise = std::pow(noiseMap[x + y * chunkWidth] * 1.1, 3);
			vertices.push_back(std::fmax(easedNoise * _meshHeight, waterHeight * 0.5 * _meshHeight));
			vertices.push_back(y);
		}
	}
	return vertices;
}

std::vector<float> Terrain::generateNormals(const std::vector<int> &indices, const std::vector<float> &vertices) {
	int pos;
	glm::vec3 normal;
	std::vector<float> normals;
	std::vector<glm::vec3> verts;

	for (int i = 0; i < indices.size(); i += 3) {
		for (int j = 0; j < 3; j++) {
			pos = indices[i + j] * 3;
			verts.emplace_back(vertices[pos], vertices[pos + 1], vertices[pos + 2]);
		}

		glm::vec3 edge1 = verts[i + 1] - verts[i];
		glm::vec3 edge2 = verts[i + 2] - verts[i];

		normal = glm::normalize(-glm::cross(edge1, edge2));
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}

	return normals;
}

std::vector<float> Terrain::generateColors(const std::vector<float> &vertices, int xOffset, int yOffset) {
	std::vector<float> colors;
	std::vector<TerrainColor> terrainColor;
	glm::vec3 color = getColor(255, 255, 255);

	terrainColor.push_back(TerrainColor(waterHeight * 0.5, getColor(60, 95, 190)));
	terrainColor.push_back(TerrainColor(waterHeight, getColor(60, 100, 190)));

	terrainColor.push_back(TerrainColor(0.15, getColor(210, 215, 130)));
	terrainColor.push_back(TerrainColor(0.30, getColor(95, 165, 30)));
	terrainColor.push_back(TerrainColor(0.40, getColor(65, 115, 20)));
	terrainColor.push_back(TerrainColor(0.50, getColor(90, 65, 60)));
	terrainColor.push_back(TerrainColor(0.80, getColor(75, 60, 55)));
	terrainColor.push_back(TerrainColor(1.00, getColor(255, 255, 255)));

	for (int i = 1; i < vertices.size(); i += 3) {
		for (int j = 0; j < terrainColor.size(); j++) {
			if (vertices[i] <= terrainColor[j]._height * _meshHeight) {
				color = terrainColor[j]._color;
				break;
			}
		}
		colors.push_back(color.r);
		colors.push_back(color.g);
		colors.push_back(color.b);
	}

	return colors;
}

void Terrain::generateMapChunk(GLuint &vao, int xOffset, int yOffset) {
	std::vector<int> indices = generateIndices();
	std::vector<float> noiseMap = generateNoiseMap(xOffset, yOffset);
	std::vector<float> vertices = generateVertices(noiseMap);
	std::vector<float> normals = generateNormals(indices, vertices);
	std::vector<float> colors = generateColors(vertices, xOffset, yOffset);


	glGenBuffers(3, _vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(2);
}

glm::vec3 Terrain::getColor(int r, int g, int b) {
	return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}

std::vector<int> Terrain::getPermutationVector() {
	std::vector<int> permutations;

	std::vector<int> permutation = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,
		140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,
		120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,
		177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
		74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,
		122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,
		143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,
		173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,
		255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,
		155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,
		232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,
		144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,
		192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
		127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
		128,195,78,66,215,61,156,180
	};

	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 256; i++) {
			permutations.push_back(permutation[i]);
		}

	return permutations;
}

TerrainColor::TerrainColor(float height, glm::vec3 color) : _height(height), _color(color) {}
