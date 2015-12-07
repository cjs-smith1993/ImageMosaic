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

std::vector<Image*> createPalette(Image*, std::string, int, int);

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

	/* CREATE PALETTE FROM SOURCE IMAGES */
	std::vector<Image*> images = createPalette(target, srcImagesDir, nSourceRows, nSourceCols);
	double createPaletteTime = getCurrentTime();
	std::cout << "Create palette time (s): " << createPaletteTime - getTargetDimsTime << "\n";

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

std::string getNameForCell(Image* targetImage, std::vector<std::string> paletteImageNames, int curRow, int curCol, int cellHeight, int cellWidth) {
	int randomIndex = rand() % paletteImageNames.size();
	return paletteImageNames[randomIndex];
}

std::vector<Image*> createPalette(Image* targetImage,
		std::string paletteImagesDirectory, int nSourceRows, int nSourceCols) {

	/* GET PALETTE IMAGES FROM DIRECTORY */
	std::vector<std::string> paletteImageNames = getImagesFromDirectory(paletteImagesDirectory);

	int eachHeight = targetImage->height / nSourceRows;
	int eachWidth = targetImage->width / nSourceCols;

	int numSubCells = nSourceRows * nSourceCols;
	std::vector<Image*> images(numSubCells, NULL);
	double targetDimsRatio = (eachWidth*1.0)/eachHeight;

	// #pragma omp parallel for
	for (int i = 0; i < numSubCells; i++) {
		int curRow = i / nSourceRows;
		int curCol = i % nSourceCols;
		std::string cellImageName = getNameForCell(targetImage,
			paletteImageNames, curRow, curCol, eachHeight, eachWidth);

		int imageHeight, imageWidth;
		getImageDims(cellImageName, &imageHeight, &imageWidth);

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
			resizeHeight = eachWidth / imageDimsRatio + 1; // the 0.5 is to prevent rounding errors
		}

		cellImageName = resizeImage(cellImageName, resizeWidth, resizeHeight);
		getImageDims(cellImageName, &imageHeight, &imageWidth);
		// std::cout << i << ": " << cellImageName << "(" << imageWidth << "x" << imageHeight << ")\n";
		Image* tempImage = new Image(cellImageName);
		images.at(i) = tempImage->crop(eachHeight, eachWidth);
		// std::cout << i << ": " << cellImageName << "(" << images[i]->width << "x" << images[i]->height << ")\n";
	}

	system(std::string("rm " + paletteImagesDirectory + "/*resize*").c_str());
	return images;
}