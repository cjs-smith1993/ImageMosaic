#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Mosaic.h"
#include "Image.h"
#include "Stitcher.h"
#include "Utils.h"
#include "time.h"

std::vector<Image*> createPalette(Image*, std::string, int, int);
Image* resize(Image*, int, int);

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
	int numSrcRows = std::stoi(std::string(argv[3]));
	int numSrcCols = std::stoi(std::string(argv[4]));
	double getArgsTime = getCurrentTime();
	std::cout << "Get arguments time (s): " << getArgsTime - startTime << "\n";

	/* PREP TARGET */
	int targetHeight;
	int targetWidth;
	targetImage = getImageDims(targetImage, &targetHeight, &targetWidth);
	Image* target = new Image(targetImage);

	// Make sure target dims are evenly divisible by source rows/columns
	int newTargetHeight = target->height - (target->height % numSrcRows);
	int newTargetWidth = target->width - (target->width % numSrcCols);
	if (newTargetWidth != targetWidth || newTargetHeight != targetHeight) {
		targetHeight = newTargetHeight;
		targetWidth = newTargetWidth;
		target = target->crop(targetHeight, targetWidth);
	}
	double prepTargetTime = getCurrentTime();
	std::cout << "Prep target time (s): " << prepTargetTime - getArgsTime << "\n";

	/* CREATE PALETTE FROM SOURCE IMAGES */
	std::vector<Image*> palette = createPalette(target, srcImagesDir, numSrcRows, numSrcCols);
	double createPaletteTime = getCurrentTime();
	std::cout << "Create palette time (s): " << createPaletteTime - prepTargetTime << "\n";

	/* CREATE MOSAIC */
	Mosaic* mosaic = new Mosaic(target, palette, numSrcRows, numSrcCols);
	double createMosaicTime = getCurrentTime();
	std::cout << "Create mosaic time (s): " << createMosaicTime - createPaletteTime << "\n";

	/* MOSAIC TO IMAGE */
	Image* mosaicImg = mosaic->toImage();
	double stitchTime = getCurrentTime();
	std::cout << "Stitch mosaic time (s): " << stitchTime - createMosaicTime << "\n";

	/* WRITE TO FILE */
	mosaicImg->writeToFile("output/mosaic.ppm");
	double writeTime = getCurrentTime();
	std::cout << "Write time (s): " << writeTime-stitchTime << "\n";

	double endTime = getCurrentTime();
	std::cout << "Execution time (s): " << endTime-startTime << "\n";
}

std::vector<Image*> createPalette(Image* targetImage,
		std::string paletteImagesDirectory, int numSrcRows, int numSrcCols) {

	/* GET PALETTE IMAGES FROM DIRECTORY */
	std::vector<std::string> paletteImageNames = getImagesFromDirectory(paletteImagesDirectory);

	int eachHeight = targetImage->height / numSrcRows;
	int eachWidth = targetImage->width / numSrcCols;

	std::vector<Image*> images(paletteImageNames.size(), NULL);
	double targetDimsRatio = (eachWidth*1.0)/eachHeight;

	#pragma omp parallel for
	for (int i = 0; i < paletteImageNames.size(); i++) {

		std::string cellImageName = paletteImageNames[i];

		int imageHeight, imageWidth;
		getImageDims(cellImageName, &imageHeight, &imageWidth);

		double imageDimsRatio = (imageWidth * 1.0)/imageHeight;
		int resizeWidth = imageWidth;
		int resizeHeight = imageHeight;

		// if the height is the constraint
		if (imageDimsRatio > targetDimsRatio) {
			resizeWidth = eachHeight * imageDimsRatio + 0.5;
			resizeHeight = eachHeight;
		}
		// if the width is the constraint
		else if (imageDimsRatio < targetDimsRatio) {
			resizeWidth = eachWidth;
			resizeHeight = eachWidth / imageDimsRatio + 1; // the 1 is to prevent rounding errors
		}

		Image* tempImage = resize(new Image(cellImageName), resizeHeight, resizeWidth);
		images.at(i) = tempImage->crop(eachHeight, eachWidth);
	}

	return images;
}

Image* resize(Image* srcImage, int newHeight, int newWidth) {
	int srcHeight = srcImage->height;
	int srcWidth = srcImage->width;
	int srcColsPerNew = srcWidth / newWidth;
	int srcRowsPerNew = srcHeight / newHeight;

	Image* shrinkColumns = new Image(srcHeight, newWidth);

	// shrink cols
	#pragma omp parallel for
	for (int i = 0; i < srcHeight; i++) {
		int tempR = 0;
		int tempG = 0;
		int tempB = 0;

		for (int j = 0; j < srcWidth; j++) {
			Pixel* temp = srcImage->pixels[i][j];
			tempR += temp->r;
			tempG += temp->g;
			tempB += temp->b;

			if (j % srcColsPerNew == srcColsPerNew - 1 || j == srcWidth - 1) {
				int newRow = i;
				int newCol = j / srcColsPerNew;
				if (newRow >= srcHeight || newCol >= newWidth) {
					continue;
				}
				shrinkColumns->pixels[newRow][newCol] = new Pixel(tempR/srcColsPerNew, tempG/srcColsPerNew, tempB/srcColsPerNew);
				tempR = tempG = tempB = 0;
			}
		}
	}

	Image* shrinkRows = new Image(newHeight, newWidth);

	// shrink rows
	#pragma omp parallel for
	for (int j = 0; j < shrinkColumns->width; j++) {
		int tempR = 0;
		int tempG = 0;
		int tempB = 0;

		for (int i = 0; i < shrinkColumns->height; i++) {
			Pixel* temp = shrinkColumns->pixels[i][j];
			tempR += temp->r;
			tempG += temp->g;
			tempB += temp->b;

			if (i % srcRowsPerNew == srcRowsPerNew - 1 || i == shrinkColumns->height - 1) {
				int newRow = i / srcRowsPerNew;
				int newCol = j;
				if (newRow >= newHeight || newCol >= newWidth) {
					continue;
				}
				shrinkRows->pixels[newRow][newCol] = new Pixel(tempR/srcRowsPerNew, tempG/srcRowsPerNew, tempB/srcRowsPerNew);
				tempR = tempG = tempB = 0;
			}
		}
	}

	return shrinkRows;
}