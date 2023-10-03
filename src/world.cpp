#include "../include/world.h"

World::World(std::mt19937_64 &rng, unsigned int octaves, double frequency, double amplitude, double persistence)
{
	this->rng = rng;
	this->frequency = frequency;
	this->amplitude = amplitude;
	this->persistence = persistence;
	this->noises = std::vector<SimplexNoise>();
	this->noises.reserve(octaves);
	for (size_t i = 0; i < octaves; i++)
	{
		this->noises[i] = SimplexNoise(this->rng);
	}
}

int World::getHeightAt(int x, int y)
{
	double f = frequency;
	double p = persistence;
	double height = 0.;

	for (SimplexNoise n : noises)
	{
		height += p * amplitude * n.sample(((double)x + 0.5) * f, ((double)y + 0.5) * f);
		f *= 2;
		p *= persistence;
	}

	return (int)height;
}
