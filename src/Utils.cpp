#include "Utils.h"

std::string getFileType(std::string fileName) {
	return fileName.substr(fileName.find('.')+1);
}

std::string stripFileExtension(std::string fileName) {
	int index = fileName.find('.');
	return fileName.substr(0, index);
}

std::string getBaseName(std::string fileName) {
	int index = fileName.rfind('/');
	return fileName.substr(index);
}

std::vector<std::string> getPPMsFromDirectory(std::string directoryName) {
	std::vector<std::string> files;

	DIR* directory;
	if ((directory = opendir(directoryName.c_str())) == NULL) {
		return files;
	}

	char fileName[100];
	struct dirent* contents;
	while ((contents = readdir(directory)) != NULL) {
		struct stat status;
		sprintf(fileName, "%s/%s", directoryName.c_str(), contents->d_name);
		if (stat(fileName, &status) == -1) {
			continue;
		}
		if ((status.st_mode & S_IFMT) == S_IFDIR) {
			continue;
		}
		if (isPPM(fileName)) {
			files.push_back(fileName);
		}
	}

	return files;
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
