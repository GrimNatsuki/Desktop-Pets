#include <random>

static std::mt19937 gen(std::random_device{}());

int intRNG()
{
	std::uniform_int_distribution<> dist(1, 100);
	return dist(gen);
}

float floatRNG()
{
	std::uniform_real_distribution<float> dist(0.f, 1.f);
	return dist(gen);
}
