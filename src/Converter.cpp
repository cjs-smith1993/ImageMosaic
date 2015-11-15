#include "Converter.h"

std::string getFileType(std::string fileName) {
	return fileName.substr(fileName.find('.')+1);
}

std::string stripFileExtension(std::string fileName) {
	int index = fileName.find('.');
	return fileName.substr(0, index);
}

std::string convertToPPM(std::string fileName) {
	std::string newFileName(stripFileExtension(fileName) + ".ppm");
	std::string command("convert " + fileName + " -compress none " + newFileName);
	system(command.c_str());

	return newFileName;
}
