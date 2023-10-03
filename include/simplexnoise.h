#ifndef SIMPLEXNOISE_H
#define SIMPLEXNOISE_H

#include <algorithm>
#include <array>
#include <numbers>
#include <random>
#include <cmath>

#include <glm/glm.hpp>

#define DIRECTIONS_COUNT 289

class SimplexNoise
{
public:
	SimplexNoise(std::mt19937_64 &seed);
	double sample(double x, double y);
private:
	std::array<glm::vec2, DIRECTIONS_COUNT> directions;
	static constexpr glm::mat2 skew_matrix = glm::mat2(1.3660254037844386, 0.3660254037844386,  // 1 + (sqrt(3) - 1) / 2, (sqrt(3) - 1)/2
													   0.3660254037844386, 1.3660254037844386); // Skew (x, y) into (x + (x + y) * F, y + (x + y) * F), where F = (sqrt(3) - 1)/2
	static constexpr glm::mat2 unskew_matrix = glm::mat2( 0.7886751345948129, -0.21132486540518713, // 1 - (3 - sqrt(3)) / 6, - (3 - sqrt(3)) / 6
														 -0.21132486540518713, 0.7886751345948129); // Unskew (x, y) into (x - (x + y) * G, y - (x + y) * G), where G = (3 - sqrt(3)) / 6
	static glm::vec2 skew(glm::vec2 vector);
	static glm::vec2 unskew(glm::vec2 vector);
	static unsigned int shuffle(int x);
};

#endif // SIMPLEXNOISE_H
