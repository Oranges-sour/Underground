#include "Random.h"

using namespace std;

unsigned int Random::seed{ 0 };
default_random_engine Random::e{};
random_device Random::rd{};