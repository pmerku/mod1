//
// Created by pmerku on 31/05/2021.
//

#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

#include <vector>

namespace gen {

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	double perlinNoise(float x, float y, std::vector<int> &permutations);

} // namespace gen

#endif //PERLINNOISE_HPP
