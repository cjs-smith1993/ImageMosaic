#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Image.h"
#include "Stitcher.h"
#include "Utils.h"

int main(int argc, char* argv[]) {

	std::vector<std::string> imageNames = getImagesFromDirectory(argv[1]);
	std::vector<Image*> images;
	for (int i = 0; i < imageNames.size(); i++) {
		images.push_back(new Image(imageNames[i]));
	}

	int mosaicHeight, mosaicWidth;
	std::istringstream ss;
	ss.str(argv[2]);
	ss >> mosaicHeight;
	ss.clear();
	ss.str(argv[3]);
	ss >> mosaicWidth;

	int minHeight = std::numeric_limits<int>::max();
	int minWidth = std::numeric_limits<int>::max();

	for (int i = 0; i < images.size(); i++) {
		int curHeight = images[i]->height;
		int curWidth = images[i]->width;
		minHeight = (curHeight < minHeight) ? curHeight : minHeight;
		minWidth = (curWidth < minWidth) ? curWidth : minWidth;
	}

	for (int i = 0; i < images.size(); i++) {
		images[i] = images[i]->crop(minHeight, minWidth);
		// std::string oldName(stripFileExtension(getBaseName(imageNames[i])));
		// std::string newFileName("output/" + oldName + ".ppm");
		// images[i]->writeToFile(newFileName);
	}

	Image* mosaic = stitch(images, mosaicHeight, mosaicWidth);
	mosaic->writeToFile("output/mosaic.ppm");
}