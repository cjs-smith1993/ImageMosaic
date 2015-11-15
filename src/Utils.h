#ifndef __UTILS_H
#define __UTILS_H

#include <dirent.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>

std::string getFileType(std::string);
std::string stripFileExtension(std::string);
std::string getBaseName(std::string);
std::vector<std::string> getPPMsFromDirectory(std::string);
bool isPPM(std::string);
std::string convertToPPM(std::string);

#endif