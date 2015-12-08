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

Image* Mosaic::getBestFit(Image* target, int targetRow, int targetCol, int sectionHeight, int sectionWidth, std::vector<Image*> palette) {
	int randomIndex = rand() % palette.size();
	return palette[randomIndex];
}
