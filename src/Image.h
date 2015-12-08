#ifndef __IMAGE_H
#define __IMAGE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "Pixel.h"
#include "Utils.h"

class Image {
public:
	unsigned int width;
	unsigned int height;
	std::vector< std::vector<Pixel*> > pixels;

	Image();
	Image(std::string);
	Image(int, int);
	Image(int, int, const Pixel*);
	Image(const Image*);
	~Image();

	Image* crop(int, int);
	void writeToFile(std::string);
};

#endif