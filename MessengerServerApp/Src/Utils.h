/*
 * Utils.h
 *
 *  Created on: Feb 19, 2016
 *      Author: user
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Utils.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Utils {
private:
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
public:
	static std::vector<std::string> split(const std::string &s, char delim);
};

#endif /* UTILS_H_ */
