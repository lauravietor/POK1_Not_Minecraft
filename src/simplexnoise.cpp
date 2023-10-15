#include "../include/simplexnoise.h"
#include <iostream>

using std::numbers::pi;

SimplexNoise::SimplexNoise(std::mt19937_64 &rng)
{
	for (int i = 0; i < DIRECTIONS_COUNT; i++)
	{
		directions[i] = glm::vec2(cos(2.0 * pi * (double)i / (double)DIRECTIONS_COUNT),
								  sin(2.0 * pi * (double)i / (double)DIRECTIONS_COUNT));
	}

	std::shuffle(directions.begin(), directions.end(), rng);
}

double SimplexNoise::sample(double x, double y)
{
	const glm::vec2 coords = glm::vec2(x, y);
	const glm::vec2 skewed = skew(coords);
	glm::vec2 vertex0 = glm::floor(skewed);
	const glm::vec2 s_delta = skewed - vertex0;
	glm::vec2 vertex1 = vertex0 + (s_delta.x > s_delta.y ? glm::vec2(1., 0.) : glm::vec2(0., 1.));
	glm::vec2 vertex2 = vertex0 + glm::vec2(1., 1.);

	vertex0 = unskew(vertex0);
	vertex1 = unskew(vertex1);
	vertex2 = unskew(vertex2);

	glm::vec2 delta0 = coords - vertex0;
	glm::vec2 delta1 = coords - vertex1;
	glm::vec2 delta2 = coords - vertex2;

	glm::vec2 dir0 = directions[shuffle(shuffle((int)vertex0.x) + shuffle(shuffle((int)vertex0.x)))];
	glm::vec2 dir1 = directions[shuffle(shuffle((int)vertex1.x) + shuffle(shuffle((int)vertex1.x)))];
	glm::vec2 dir2 = directions[shuffle(shuffle((int)vertex2.x) + shuffle(shuffle((int)vertex2.x)))];

	// m0 = pow(std::max(0., 0.6 - glm::dot(delta0, delta0)), 4)
	double m0 = std::max(0., 0.6 - glm::dot(delta0, delta0));
	m0 *= m0;
	m0 *= m0;
	double m1 = std::max(0., 0.6 - glm::dot(delta1, delta1));
	m1 *= m1;
	m1 *= m1;
	double m2 = std::max(0., 0.6 - glm::dot(delta2, delta2));
	m2 *= m2;
	m2 *= m2;

	return m0 * glm::dot(delta0, dir0)
		 + m1 * glm::dot(delta1, dir1)
		 + m2 * glm::dot(delta2, dir2);
}

glm::vec2 SimplexNoise::skew(glm::vec2 vector)
{
	return skew_matrix * vector;
}

glm::vec2 SimplexNoise::unskew(glm::vec2 vector)
{
	return unskew_matrix * vector;
}

unsigned int SimplexNoise::shuffle(int x)
{
	return (unsigned int)(((22695477 * x + 1) % DIRECTIONS_COUNT + DIRECTIONS_COUNT) % DIRECTIONS_COUNT);
}
