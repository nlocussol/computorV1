# include "../inc/utils.hpp"

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

int isVariable(std::string &variable)
{
    if (variable[0] != 'X' || variable[1] != '^')
        return NOT_VARIABLE;
    char *endPtr;
    int number = strtod(&variable.c_str()[2], &endPtr);
    if (strlen(endPtr))
        return NOT_VARIABLE;
    return number;
}

bool isOperator(std::string & str) {
    if (str == "=" || str == "+"
        || str == "-" || str == "*")
        return true;
    return false;
}

bool isFloat(std::string & str) {
	char *endPtr;
	strtof(str.c_str(), &endPtr);
	return strlen(endPtr) > 0 ? false : true;
}

int countDecimal(float number) {
	if (number - int(number) == 0)
		return 0;
    std::string numStr = std::to_string(number);
    size_t decimalPos = numStr.find('.');
    if (decimalPos != std::string::npos)
        return numStr.length() - decimalPos - 1;
    return 0;
}


int GCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}