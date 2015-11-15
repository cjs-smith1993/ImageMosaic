#ifndef __UTILS_H
#define __UTILS_H

#include <dirent.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

static const char* imageTypes[] = {"ppm", "jpg", "png", "bmp"};

std::string getFileType(std::string);
std::string stripFileExtension(std::string);
std::string getBaseName(std::string);
std::vector<std::string> getImagesFromDirectory(std::string);
bool isImage(std::string);
bool isPPM(std::string);
std::string convertToPPM(std::string);

#endif