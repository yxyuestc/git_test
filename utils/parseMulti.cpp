#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

namespace utils
{

	void parse(const std::string &str, std::vector<int> &data)
	{
		int k1 = str.size(), k2 = 0;
		for (size_t i = str.size() - 1; i >= 0; i--)
		{
			if (str[i] < '0' || str[i] > '9')
				k1--;
			else
				break;
		}
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] < '0' || str[i] > '9')
				k2++;
			else
				break;
		}
		auto it = std::find(str.begin() + k2, str.begin() + k1, ',');
		if (it == str.end())
		{
			std::cout << "Input Error" << std::endl;
			return;
		}
		data[0] = atoi(std::string(str.begin() + k2, it).c_str());
		while (*it < '0' || *it > '9')
			++it;
		data[1] = atoi(std::string(it, str.begin() + k1 + 1).c_str());
	}

	void parseMulti(const std::string &str, std::vector<std::vector<int>> &datas)
	{
		auto it_begin = str.begin();
		while (it_begin != str.end())
		{
			auto it = std::find(it_begin, str.end(), ')');
			std::vector<int> data(2);
			parse(std::string(it_begin, it), data);
			datas.push_back(data);
			it_begin = it + 1;
		}
	}

}