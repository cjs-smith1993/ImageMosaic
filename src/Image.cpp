#include "Image.h"

Image::Image(std::string fileName) {
	if (!isPPM(fileName)) {
		fileName = convertToPPM(fileName);
	}

	std::ifstream file(fileName.c_str());
	std::string line;
	std::getline(file, line);
	std::getline(file, line);
	sscanf(line.c_str(), "%d %d", &this->width, &this->height);
	std::getline(file, line);

	this->pixels = std::vector< std::vector<Pixel*> >();
	for (int i = 0; i < this->height; i++) {
		this->pixels.push_back(std::vector<Pixel*>());
		for (int j = 0; j < this->width; j++) {
			int r, g, b;
			file >> r >> g >> b;
			this->pixels[i].push_back(new Pixel(r, g, b));
		}
	}
}

Image::Image(int height, int width) {
	this->height = height;
	this->width = width;

	this->pixels = std::vector< std::vector<Pixel*> >();
	for (int i = 0; i < this->height; i++) {
		this->pixels.push_back(std::vector<Pixel*>());
		for (int j = 0; j < this->width; j++) {
			this->pixels[i].push_back(NULL);
		}
	}
}

Image::Image(const Image* that) {
	this->height = that->height;
	this->width = that->width;
	this->pixels = that->pixels;
}

Image* Image::crop(int height, int width) {
	Image* newImage = new Image(height, width);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			newImage->pixels[i][j] = new Pixel(this->pixels[i][j]);
		}
	}

	return newImage;
}

void Image::writeToFile(std::string fileName) {
	std::ofstream file(fileName.c_str());

	file << "P3\n";
	file << this->width << " " << this->height << "\n";
	file << "255\n";

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			Pixel* p = this->pixels[i][j];
			file << p->r << " " << p->g << " " << p->b << " ";
		}
		file << "\n";
	}

	file.close();
}