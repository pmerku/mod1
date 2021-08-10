//
// Created by pmerku on 31/05/2021.
//

#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

namespace opengl {

	struct TerrainColor {
		TerrainColor(float height, glm::vec3 color);

		float 		_height;
		glm::vec3	_color;
	};

	class Terrain {
	public:
		Terrain();
		~Terrain();

		std::vector<GLuint> mapChunks;

		const float waterHeight = 0.1;
		const int chunkWidth = 127;
		const int chunkHeight = 127;

		const int chunkRenderDistance = 3;
		int nIndices;

	private:
		std::vector<int> generateIndices() const;
		std::vector<float> generateNoiseMap() const;
		std::vector<float> generateVertices(const std::vector<float> &noiseMap) const;
		static std::vector<float> generateNormals(const std::vector<int> &indices, const std::vector<float> &vertices);
		std::vector<float> generateColors(const std::vector<float> &vertices);
		void generateMapChunk(GLuint &vao);

		static glm::vec3 getColor(int r, int g, int b);
		static std::vector<int> getPermutationVector();

		GLuint _vbo[3];
		GLuint _ebo;

		const int _octaves = 5;
		const float _meshHeight = 32;
		const float _noiseScale = 64;
		const float _persistence = 0.5;
		const float _lacunarity = 2;
	};

} // namespace opengl

#endif //TERRAIN_HPP
