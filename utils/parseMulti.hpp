#ifndef Utils_ParseMulti_hpp
#define Utils_ParseMulti_hpp

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

namespace utils {

void parse(const std::string &str, std::vector<int> &data);
void parseMulti(const std::string &str, std::vector<std::vector<int>> &datas);

}
#endif