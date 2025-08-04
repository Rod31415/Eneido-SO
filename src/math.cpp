#include "headers/math.h"
#define PI 3.1415926535897932384626433238795028
#define PI_2 3.1415926535897932384626433238795028 / 2.0

static unsigned long int actualSeed = 0;

void srand(uint32 seed)
{
	actualSeed = seed;
}

uint32 rand()
{
	actualSeed = actualSeed * 1103515245 + 12345;
	return (uint32)(actualSeed / 65536) % 32768;
}

double sin(double x)
{
	uint32 index=(uint32)(x/PI*180.0);
	return sineLookUpTable[index];
}

double cos(double x)
{
	uint32 index=(uint32)(x/PI*180.0);
	return cosineLookUpTable[index];
}

float sqrt(float base)
{
	return 1;
}
