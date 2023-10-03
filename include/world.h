#ifndef WORLD_H
#define WORLD_H

#include <random>
#include <vector>

#include "simplexnoise.h"

class World
{
public:
	World(std::mt19937_64 &rng, unsigned int octaves, double frequency, double amplitude, double persistence);
	int getHeightAt(int x, int y);
private:
	std::mt19937_64 rng;
	double frequency;
	double amplitude;
	double persistence;
	std::vector<SimplexNoise> noises;
};

#endif // WORLD_H
