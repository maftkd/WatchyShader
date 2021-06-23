
#include "lehmer.h"

uint32_t nProcGen=0;

void setSeed(uint32_t seed){
	nProcGen=seed;
}

uint32_t rnd(){
	nProcGen += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nProcGen * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

//this is kinda bunk
double randDouble(double min, double max)
{
	return ((double)rnd() / (double)(0x7FFFFFFF)) * (max - min) + min;
}

int randInt(int min, int max)
{
	return (rnd() % (max - min)) + min;
}
