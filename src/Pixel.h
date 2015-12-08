#ifndef __PIXEL_H
#define __PIXEL_H

#include <math.h>

class Pixel {
public:
	Pixel();
	Pixel(int, int, int);
	Pixel(const Pixel*);
	~Pixel();

	double distance(const Pixel*);

	unsigned int r;
	unsigned int g;
	unsigned int b;
};

#endif