#ifndef __CONVERTER_H
#define __CONVERTER_H

#include <string>
#include <stdlib.h>

std::string getFileType(std::string);
std::string stripFileExtension(std::string);
std::string convertToPPM(std::string);

#endif