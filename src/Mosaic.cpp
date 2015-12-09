#include "Mosaic.h"

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

Mosaic::Mosaic(Image* target, std::vector<Image*> palette, int numSrcRows,
		int numSrcCols) {
	this->images = std::vector<std::vector<Image*> >(numSrcRows,
		std::vector<Image*>(numSrcCols, NULL));
	this->height = target->height;
	this->width = target->width;
	this->numSrcRows = numSrcRows;
	this->numSrcCols = numSrcCols;

	int sectionHeight = this->height / numSrcRows;
	int sectionWidth = this->width / numSrcCols;

	std::vector<Pixel*> paletteAverages(palette.size(), NULL);
	#pragma omp parallel for
	for (int i = 0; i < palette.size(); i++) {
		Image* sourceImage = palette.at(i);
		paletteAverages.at(i) = getAveragePixel(sourceImage,
			0, sourceImage->height,
			0, sourceImage->width);
	}

	#pragma omp parallel for
	for (int row = 0; row < numSrcRows; row++) {
		#pragma omp parallel for
		for (int col = 0; col < numSrcCols; col++) {
			int targetRow = row * sectionHeight;
			int targetCol = col * sectionWidth;
			int bestIndex = getBestFit(target, targetRow, targetCol,
				sectionHeight, sectionWidth, paletteAverages);
			this->images.at(row).at(col) = palette.at(bestIndex);
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

int Mosaic::getBestFit(Image* target, int targetRow, int targetCol,
		int sectionHeight, int sectionWidth,
		std::vector<Pixel*> paletteAverages) {

	Pixel* targetCellAverage = getAveragePixel(target,
		targetRow, targetRow + sectionHeight,
		targetCol, targetCol + sectionWidth);

	int bestIndex = -1;
	double minDifference = 1E100;
	for (int i = 0; i < paletteAverages.size(); i++) {
		double curDifference = targetCellAverage->distance(paletteAverages.at(i));
		if (curDifference < minDifference) {
			minDifference = curDifference;
			bestIndex = i;
		}
	}

	return bestIndex;
}
