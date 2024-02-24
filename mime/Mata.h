#ifndef MATA_H
#define MATA_H
#include <string>
#include <vector>

struct artistInfo {
	std::string name;
	unsigned int num;
};

struct mata {

	std::string musicName;
	
	std::vector<struct artistInfo> artist;

	std::string album;

	std::string format;
};

typedef mata Mata;

#endif
