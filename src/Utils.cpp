#include "Utils.h"

std::array<std::string, 5> imageTypes = {{ "ppm", "jpg", "png", "bmp" }};

std::string getFileType(std::string fileName) {
	return fileName.substr(fileName.rfind('.')+1);
}

std::string stripFileExtension(std::string fileName) {
	int index = fileName.rfind('.');
	return fileName.substr(0, index);
}

std::string getBaseName(std::string fileName) {
	int index = fileName.rfind('/');
	return fileName.substr(index);
}

std::vector<std::string> getImagesFromDirectory(std::string directoryName) {
	std::vector<std::string> files;

	char curPath[FILENAME_MAX];
	getcwd(curPath, sizeof(curPath));
	std::string fullDirectoryName(curPath + std::string("/") + directoryName);

	DIR* directory;
	if ((directory = opendir(fullDirectoryName.c_str())) == NULL) {
		std::cout << "Cannot open directory: " << fullDirectoryName << "\n";
		return files;
	}

	char fileName[100];
	struct dirent* contents;
	while ((contents = readdir(directory)) != NULL) {
		struct stat status;
		sprintf(fileName, "%s/%s", fullDirectoryName.c_str(), contents->d_name);
		// if an error occured, ignore the file
		if (stat(fileName, &status) == -1) {
			continue;
		}
		// if file is a directory, ignore the file
		if ((status.st_mode & S_IFMT) == S_IFDIR) {
			// S_IFMT are the "file type" bits
			// S_IFIDR is the "directory" bit configuration
			continue;
		}
		// if file is an image, add it to the list of files
		if (isImage(fileName)) {
			files.push_back(fileName);
		}
	}

	return files;
}

bool isImage(std::string fileName) {
	std::string fileType = getFileType(fileName);
	for (int i = 0; i < imageTypes.size(); i++) {
		if (fileType == imageTypes[i]) {
			return true;
		}
	}
	return false;
}

bool isPPM(std::string fileName) {
	return getFileType(fileName) == "ppm";
}

std::string convertToPPM(std::string fileName) {
	std::string newFileName(stripFileExtension(fileName) + ".ppm");
	std::string command("convert " + fileName + " -compress none " + newFileName);
	system(command.c_str());

	return newFileName;
}

std::string resizeImage(std::string fileName, int target_width, int target_height) {
	std::string newFileName(stripFileExtension(fileName) + "_resized." + getFileType(fileName));

	std::string command("convert -resize " + std::to_string(target_width) + "x" + std::to_string(target_height) + " " + fileName + " -compress none " + newFileName);
	system(command.c_str());

	return newFileName;
}

std::string getImageDims(std::string fileName, int* height, int* width) {
	if (!isPPM(fileName)) {
		std::cout << "Converted " << fileName << " to a ppm\n";
		fileName = convertToPPM(fileName);
	}

	std::ifstream file(fileName.c_str());
	std::string line;
	std::getline(file, line);
	std::getline(file, line);
	sscanf(line.c_str(), "%d %d", width, height);

	return fileName;
}

void generateColorPalette(int granularity) {
	system("mkdir images_colors");
	system("rm -f images_colors/*");

	int MAX_VAL = 256;
	int step = MAX_VAL/granularity;
	for (int r = 0; r <= MAX_VAL; r += step) {
		for (int g = 0; g <= MAX_VAL; g += step) {
			for (int b = 0; b <= MAX_VAL; b += step) {
				Pixel* p = new Pixel(r, g, b);
				Image* curImage = new Image(100, 100, p);
				std::string rgb = std::to_string(r) + "_" +
									std::to_string(g) + "_" +
									std::to_string(b);
				// std::cout << rgb << "\n";
				curImage->writeToFile("images_colors/" + rgb + ".ppm");
			}
		}
	}
}
