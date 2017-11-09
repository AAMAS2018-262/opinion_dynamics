#include "random.h"

#include <chrono>

Random::Random()
	: Random(std::chrono::system_clock::now().time_since_epoch().count())
{

}

Random::Random(unsigned int seed)
	: seed(seed)
{
	generator.seed(seed);
}

unsigned int Random::getSeed() const
{
	return seed;
}

std::size_t Random::getSizeT(std::size_t first, std::size_t last)
{
	std::uniform_int_distribution<std::size_t> distribution(first, last);
	return distribution(generator);
}

bool Random::throwCoin()
{
	std::bernoulli_distribution distribution(0.5);
	return distribution(generator);
}
