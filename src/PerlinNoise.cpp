//
// Created by pmerku on 31/05/2021.
//

#include "PerlinNoise.hpp"
#include <cmath>

double gen::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double gen::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double gen::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double gen::perlinNoise(float x, float y, std::vector<int> &permutations) {
	int z = 0.5;

	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	double curveX = gen::fade(x);
	double curveY = gen::fade(y);
	double curveZ = gen::fade(z);

	int A = permutations[X] + Y;
	int AA = permutations[A] + Z;
	int AB = permutations[A + 1] + Z;
	int B = permutations[X + 1] + Y;
	int BA = permutations[B] + Z;
	int BB = permutations[B + 1] + Z;

	return gen::lerp(curveZ,
		gen::lerp(curveY,
			gen::lerp(curveX,
				gen::grad(permutations[AA], x, y, z),
				gen::grad(permutations[BA], x - 1, y, z)),
			gen::lerp(curveX,
				gen::grad(permutations[AB], x, y - 1, z),
				gen::grad(permutations[BB], x - 1, y - 1, z))),
		gen::lerp(curveY,
			gen::lerp(curveX,
				gen::grad(permutations[AA + 1], x, y, z - 1),
				gen::grad(permutations[BA + 1], x - 1, y, z - 1)),
			gen::lerp(curveX,
				gen::grad(permutations[AB + 1], x, y - 1, z -1),
				gen::grad(permutations[BB + 1], x - 1, y - 1, z - 1))));
}