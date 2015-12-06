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
