#pragma once

#include <random>

class Random
{
public:
	Random();
	Random(unsigned int seed);

	unsigned int getSeed() const;

	std::size_t getSizeT(std::size_t first, std::size_t last);
	bool throwCoin();

private:
	unsigned int seed;
	std::default_random_engine generator;
};
