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

double Pixel::distance(const Pixel* that) {
	double dR = this->r - that->r;
	double dG = this->g - that->g;
	double dB = this->b - that->b;

	double dist = sqrt((dR * dR) + (dG * dG) + (dB * dB));
	return dist;
}
