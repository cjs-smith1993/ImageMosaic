#ifndef __PIXEL_H
#define __PIXEL_H

class Pixel {
public:
	Pixel();
	Pixel(int, int, int);
	Pixel(const Pixel*);
	~Pixel();

	unsigned int r;
	unsigned int g;
	unsigned int b;
};

#endif