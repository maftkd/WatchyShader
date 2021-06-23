#ifndef LEHMER_H
#define LEHMER_H

#include <stdint.h>

void setSeed(uint32_t seed);
uint32_t rnd();
double randDouble(double min, double max);
int randInt(int min, int max);

#endif
