#ifndef __UTILS_H
#define __UTILS_H

#include <array>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "Image.h"
#include "Pixel.h"

std::string getFileType(std::string);
std::string stripFileExtension(std::string);
std::string getBaseName(std::string);
std::vector<std::string> getImagesFromDirectory(std::string);
bool isImage(std::string);
bool isPPM(std::string);
std::string convertToPPM(std::string);
std::string resizeImage(std::string, int, int);
std::string getImageDims(std::string, int*, int*);
void generateColorPalette(int);

#endif