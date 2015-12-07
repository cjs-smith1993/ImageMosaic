#ifndef __STITCHER
#define __STITCHER

#include <vector>

#include "Image.h"

Image* stitch(Image*, std::vector<Image*> images, int, int);

#endif