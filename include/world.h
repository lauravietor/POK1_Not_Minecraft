#ifndef WORLD_H
#define WORLD_H

#include <random>
#include <vector>

#include "simplexnoise.h"

class World
{
public:
	World(unsigned int, unsigned int octaves, double frequency, double amplitude, double persistence);
	int getHeightAt(int x, int y);
private:
	std::mt19937_64 rng;
	double frequency;
	double amplitude;
	double persistence;
	unsigned int octaves;
	std::vector<SimplexNoise> noises;
};

#endif // WORLD_H
