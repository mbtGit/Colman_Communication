/*
 * Utils.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: user
 */

#include "Utils.h"

std::vector<std::string> & Utils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> Utils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    Utils::split(s, delim, elems);
    return elems;
}
