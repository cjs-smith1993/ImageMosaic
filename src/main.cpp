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

std::vector<Image*> createPalette(std::vector<std::string>, int, int);

/*
argv[0] -> program name 		("./bin/mosaic")
argv[1] -> target image 		("./images_ppms/Fett5.ppm")
argv[2] -> source image folder 	("./images_ppms/")
argv[3]	-> num source rows		("5")
argv[4]	-> num source columns	("5")
*/
int main(int argc, char* argv[]) {

	/* START */
	double startTime = getCurrentTime();

	std::string targetImage(argv[1]);
	std::string srcImagesDir(argv[2]);
	int nSourceRows = std::stoi(std::string(argv[3]));
	int nSourceCols = std::stoi(std::string(argv[4]));
	double getArgsTime = getCurrentTime();
	std::cout << "Get arguments time (s): " << getArgsTime - startTime << "\n";

	/* GET TARGET DIMENSIONS */
	int targetHeight;
	int targetWidth;
	targetImage = getImageDims(targetImage, &targetHeight, &targetWidth);
	Image* target = new Image(targetImage);
	// Make sure target dims are evenly divisible by source rows/columns
	int newTargetHeight = target->height - (target->height % nSourceRows);
	int newTargetWidth = target->width - (target->width % nSourceCols);
	if (newTargetWidth != targetWidth || newTargetHeight != targetHeight) {
		targetHeight = newTargetHeight;
		targetWidth = newTargetWidth;
		target = target->crop(targetHeight, targetWidth);
	}
	double getTargetDimsTime = getCurrentTime();
	std::cout << "Get target dimensions time (s): " << getTargetDimsTime - getArgsTime << "\n";

	/* GET SOURCE IMAGES FROM DIRECTORY */
	std::vector<std::string> imageNames = getImagesFromDirectory(srcImagesDir);
	double getImagesTime = getCurrentTime();
	std::cout << "Get source images time (s): " << getImagesTime - getTargetDimsTime << "\n";

	/* CREATE PALETTE FROM SOURCE IMAGES */
	int paletteHeight = targetHeight / nSourceRows;
	int paletteWidth = targetWidth / nSourceCols;
	std::vector<Image*> images = createPalette(imageNames, paletteHeight, paletteWidth);
	double createPaletteTime = getCurrentTime();
	std::cout << "Create palette time (s): " << createPaletteTime - getImagesTime << "\n";

	/* STITCH MOSAIC */
	Image* mosaic = stitch(target, images, nSourceRows, nSourceCols);
	double stitchTime = getCurrentTime();
	std::cout << "Stitch mosaic time (s): " << stitchTime - createPaletteTime << "\n";

	/* WRITE TO FILE */
	mosaic->writeToFile("output/mosaic.ppm");
	double writeTime = getCurrentTime();
	std::cout << "Write time (s): " << writeTime-stitchTime << "\n";

	double endTime = getCurrentTime();
	std::cout << "Execution time (s): " << endTime-startTime << "\n";
}

std::vector<Image*> createPalette(std::vector<std::string> imageNames, int eachHeight, int eachWidth) {
	std::vector<Image*> images(imageNames.size(), NULL);
	double targetDimsRatio = (eachWidth*1.0)/eachHeight;

	// #pragma omp parallel for
	for (int i = 0; i < imageNames.size(); i++) {

		int imageHeight, imageWidth;
		getImageDims(imageNames[i], &imageHeight, &imageWidth);

		double imageDimsRatio = (imageWidth * 1.0)/imageHeight;
		int resizeWidth = imageWidth;
		int resizeHeight = imageHeight;

		// if the height is the constraint
		if (imageDimsRatio > targetDimsRatio) {
			// std::cout << "height is the constraint" << std::endl;
			resizeWidth = eachHeight * imageDimsRatio + 0.5;
			resizeHeight = eachHeight;
		}
		// if the width is the constraint
		else if (imageDimsRatio < targetDimsRatio) {
			// std::cout << "width is the constraint" << std::endl;
			resizeWidth = eachWidth;
			resizeHeight = (eachWidth * 1.0) / imageDimsRatio + 1; // the 0.5 is to prevent rounding errors
		}

		imageNames[i] = resizeImage(imageNames[i], resizeWidth, resizeHeight);
		getImageDims(imageNames[i], &imageHeight, &imageWidth);
		// std::cout << i << ": " << imageNames[i] << "(" << imageWidth << "x" << imageHeight << ")\n";
		Image* tempImage = new Image(imageNames[i]);
		images[i] = tempImage->crop(eachHeight, eachWidth);
		// std::cout << i << ": " << imageNames[i] << "(" << images[i]->width << "x" << images[i]->height << ")\n";
	}

	return images;
}