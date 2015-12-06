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
#include "time.h"

int main(int argc, char* argv[]) {

	double startTime = getCurrentTime();

	std::vector<std::string> imageNames = getImagesFromDirectory(argv[1]);
	
	double getImagesTime = getCurrentTime();
	std::cout << "Get images time (s): " << getImagesTime - startTime << "\n";

	std::vector<Image*> images(imageNames.size(), NULL);

	// #pragma omp parallel for
	for (int i = 0; i < imageNames.size(); i++) {
		images[i] = new Image(imageNames[i]);
	}

	double createImagesTime = getCurrentTime();
	std::cout << "Create images time (s): " << createImagesTime - getImagesTime << "\n";

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

	// #pragma omp parallel for
	for (int i = 0; i < images.size(); i++) {
		images[i] = images[i]->crop(minHeight, minWidth);
		// std::string oldName(stripFileExtension(getBaseName(imageNames[i])));
		// std::string newFileName("output/" + oldName + ".ppm");
		// images[i]->writeToFile(newFileName);
	}

	double cropImagesTime = getCurrentTime();
	std::cout << "Crop images time (s): " << cropImagesTime-createImagesTime << "\n";

	Image* mosaic = stitch(images, mosaicHeight, mosaicWidth);

	double stitchTime = getCurrentTime();
	std::cout << "Stitch time (s): " << stitchTime-cropImagesTime << "\n";

	mosaic->writeToFile("output/mosaic.ppm");

	double writeTime = getCurrentTime();
	std::cout << "Write time (s): " << writeTime-stitchTime << "\n";

	double endTime = getCurrentTime();
	std::cout << "Execution time (s): " << endTime-startTime << "\n";
}