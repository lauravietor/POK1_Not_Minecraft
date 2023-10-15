#include "../include/world.h"
#include <iostream>

World::World(unsigned int seed, unsigned int octaves, double frequency, double amplitude, double persistence)
{
	this->rng = std::mt19937_64();
	this->rng.seed(seed);
	this->frequency = frequency;
	this->amplitude = amplitude;
	this->persistence = persistence;
	this->noises = std::vector<SimplexNoise>();
	this->noises.reserve(octaves);
	this->octaves = octaves;
	for (size_t i = 0; i < octaves; i++)
	{
		this->noises[i] = SimplexNoise(this->rng);
	}
}

int World::getHeightAt(int x, int y)
{
	double f = frequency;
	double p = 1;
	double height = 0.;

	for (int i = 0; i < octaves; i++)
	{
		height += p * amplitude * noises[i].sample(((double)x + 0.5) * f, ((double)y + 0.5) * f);
		f *= 2;
		p *= persistence;
	}

	return static_cast<int>(height);
}
