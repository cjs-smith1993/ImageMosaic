#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Image.h"
#include "Converter.h"

int main(int argc, char* argv[]) {
	std::string fileName(argv[1]);
	Image* i = new Image("images_original/" + fileName);
	int minDim = std::min(i->width, i->height);
	Image* i2 = i->crop(minDim, minDim);
	std::string newFileName(stripFileExtension(fileName) + ".ppm");
	i2->writeToFile("images_square/" + newFileName);
}