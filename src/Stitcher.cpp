#include "Stitcher.h"

Image* stitch(std::vector<Image*> images, int numRows, int numCols) {
	Image* mosaic = new Image(images[0]);

	int singleHeight = mosaic->height;
	int singleWidth = mosaic->width;
	int newHeight = singleHeight * numRows;
	int newWidth = singleWidth * numCols;
	mosaic->height = newHeight;
	mosaic->width = newWidth;

	// resize the mosaic image
	mosaic->pixels.resize(newHeight);
	for (int i = 0; i < mosaic->height; i++) {
		mosaic->pixels[i].resize(newWidth, NULL);
	}

	// stitch together the pixels
	for (int i = 0; i < mosaic->height; i++) {
		for (int j = 0; j < mosaic->width; j++) {
			int outerRow = i / singleHeight;
			int outerCol = j / singleWidth;
			int index = outerRow*numCols + outerCol;
			Image* source = images[index];

			int innerRow = i % singleHeight;
			int innerCol = j % singleWidth;
			mosaic->pixels[i][j] = new Pixel(source->pixels[innerRow][innerCol]);
		}
	}

	return mosaic;
}