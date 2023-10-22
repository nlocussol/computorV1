# include "../inc/Computor.hpp"

std::vector<std::string> mysplit(std::string & line, std::string delimiter) {
	std::string str;
	std::vector<std::string> tab;
	for (unsigned long i = 0; i < line.size(); i++) {
		while (line[i] && strchr(delimiter.c_str(), line[i]))
			i++;
		if (line[i] == '\0')
			return (tab);
		while (line[i] && !strchr(delimiter.c_str(), line[i])) {
			str += line[i];
			i++;
		}
		tab.push_back(str);
		str.clear();
	}
	return (tab);
}

bool isFloat(std::string & str) {
	char *endPtr;
	strtof(str.c_str(), &endPtr);
	return strlen(endPtr) > 0 ? false : true;
}