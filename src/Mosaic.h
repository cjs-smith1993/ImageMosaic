#ifndef __MOSAIC_H
#define __MOSAIC_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "Image.h"
#include "Utils.h"

class Mosaic {
public:
	unsigned int width;
	unsigned int height;
	unsigned int numSrcRows;
	unsigned int numSrcCols;
	std::vector< std::vector<Image*> > images;

	Mosaic(Image*, std::vector<Image*>, int, int);
	~Mosaic();

	int getBestFit(Image*, int, int, int, int, std::vector<Pixel*>);
	Image* toImage();
};

#endif