#include "Stitcher.h"

Image* stitch(Image* targetImage, std::vector<Image*> images, int numRows, int numCols) {
	Image* mosaic = new Image(targetImage);

	int singleHeight = mosaic->height / numRows;
	int singleWidth = mosaic->width / numCols;

	// std::cout << "Target: (" << mosaic->width << "x" << mosaic->height << ")" << std::endl;
	// std::cout << "Source: (" << singleWidth << "x" << singleHeight << ")" << std::endl;

	// stitch together the pixels
	// #pragma omp parallel for
	for (int i = 0; i < mosaic->height; i++) {
		for (int j = 0; j < mosaic->width; j++) {
			// std::cout << "row: " << i << " col: " << j << std::endl;

			int outerRow = i / singleHeight; // src image row
			int outerCol = j / singleWidth;	// src image col
			int index = outerRow*numCols + outerCol;
			Image* source = images[index];

			int innerRow = i % singleHeight;
			int innerCol = j % singleWidth;
			mosaic->pixels[i][j] = new Pixel(source->pixels[innerRow][innerCol]);
		}
	}

	return mosaic;
}