#include "Mosaic.h"

Mosaic::Mosaic(Image* target, std::vector<Image*> palette, int numSrcRows, int numSrcCols) {
	this->images = std::vector<std::vector<Image*>>();
	this->height = target->height;
	this->width = target->width;
	this->numSrcRows = numSrcRows;
	this->numSrcCols = numSrcCols;

	int sectionHeight = this->height / numSrcRows;
	int sectionWidth = this->width / numSrcCols;

	for (int row = 0; row < numSrcRows; row++) {
		this->images.push_back(std::vector<Image*>());
		for (int col = 0; col < numSrcCols; col++) {
			int targetRow = row * sectionHeight;
			int targetCol = col * sectionWidth;
			this->images[row].push_back(getBestFit(target, targetRow, targetCol, sectionHeight, sectionWidth, palette));
		}
	}
}

Mosaic::~Mosaic() {

}

Image* Mosaic::toImage() {
	Image* mosaic = new Image(this->height, this->width);
	int sectionHeight = this->height / this->numSrcRows;
	int sectionWidth = this->width / this->numSrcCols;

	for (int i = 0; i < mosaic->height; i++) {
		for (int j = 0; j < mosaic->width; j++) {
			int sectionRow = i / sectionHeight;
			int sectionCol = j / sectionWidth;

			std::vector<Image*> temp = this->images[sectionRow];

			Image* src = this->images[sectionRow][sectionCol];

			int sourceRow = i % sectionHeight;
			int sourceCol = j % sectionWidth;

			mosaic->pixels[i][j] = new Pixel(src->pixels[sourceRow][sourceCol]);
		}
	}

	return mosaic;
}

Pixel* getAveragePixel(Image* image, int startY, int endY, int startX,
		int endX) {
	Pixel* averagePixel = NULL;

	long sumR = 0;
	long sumG = 0;
	long sumB = 0;
	int numPixels = (endY - startY) * (endX - startX);

	for (int i = startY; i < endY; i++) {
		for (int j = startX; j < endX; j++) {
			Pixel* curPixel = image->pixels[i][j];
			sumR += curPixel->r;
			sumG += curPixel->g;
			sumB += curPixel->b;
		}
	}

	long avgR = sumR / numPixels;
	long avgG = sumG / numPixels;
	long avgB = sumB / numPixels;
	averagePixel = new Pixel(avgR, avgG, avgB);

	return averagePixel;
}

Image* Mosaic::getBestFit(Image* target, int targetRow, int targetCol,
		int sectionHeight, int sectionWidth, std::vector<Image*> palette) {
	Image* bestImage = NULL;

	Pixel* targetCellAverage = getAveragePixel(target,
		targetRow, targetRow + sectionHeight,
		targetCol, targetCol + sectionWidth);

	double minDifference = 1E100;
	for (int i = 0; i < palette.size(); i++) {
		Image* sourceImage = palette.at(i);
		Pixel* sourceCellAverage = getAveragePixel(sourceImage,
			0, sourceImage->height,
			0, sourceImage->width);

		double curDifference = targetCellAverage->distance(sourceCellAverage);
		if (curDifference < minDifference) {
			minDifference = curDifference;
			bestImage = sourceImage;
		}
	}

	return bestImage;
}
