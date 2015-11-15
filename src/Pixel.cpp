#include "Pixel.h"

Pixel::Pixel(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

Pixel::Pixel(const Pixel* that) {
	this->r = that->r;
	this->g = that->g;
	this->b = that->b;
}