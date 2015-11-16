#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

#include "Image.h"
#include "Stitcher.h"
#include "Utils.h"

double getCurrentTime() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}

int main(int argc, char* argv[]) {
	double startTime = getCurrentTime();

	std::vector<std::string> imageNames = getImagesFromDirectory(argv[1]);
	std::vector<Image*> images(imageNames.size(), NULL);

	#pragma omp parallel for
	for (int i = 0; i < imageNames.size(); i++) {
		images[i] = new Image(imageNames[i]);
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

	#pragma omp parallel for
	for (int i = 0; i < images.size(); i++) {
		images[i] = images[i]->crop(minHeight, minWidth);
		// std::string oldName(stripFileExtension(getBaseName(imageNames[i])));
		// std::string newFileName("output/" + oldName + ".ppm");
		// images[i]->writeToFile(newFileName);
	}

	Image* mosaic = stitch(images, mosaicHeight, mosaicWidth);
	mosaic->writeToFile("output/mosaic.ppm");

	std::ostringstream oss;
	oss << minWidth << "x" << minHeight;
	std::string command("convert output/mosaic.ppm -resize " + oss.str() + " output/mosaic.ppm");
	system(command.c_str());

	double endTime = getCurrentTime();
	std::cout << "Execution time (s): " << endTime-startTime << "\n";
}