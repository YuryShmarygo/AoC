#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
	//read input file
	ifstream is("input.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Read " << input.size() << " strings" << endl;

	// First task
	int sum2 = 0, sum3 = 0;
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		map<char, int> chars;
		for_each(it->begin(), it->end(), [&](auto c) {chars[c]++; });

		sum2 += find_if(chars.begin(), chars.end(), [](auto itc) { return itc.second == 2; }) == chars.end() ? 0 : 1;
		sum3 += find_if(chars.begin(), chars.end(), [](auto itc) { return itc.second == 3; }) == chars.end() ? 0 : 1;
	}
	cout << "Day2 Answer1: " << sum2 * sum3 << endl;

	// Second task
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		for (auto jt = it + 1; jt != input.end(); ++jt)
		{
			if (it->size() != jt->size())
				continue;
			stringstream tmp;
			transform(it->begin(), it->end(), jt->begin(), ostream_iterator<char>(tmp), [](auto c1, auto c2) { return c1 == c2 ? c1 : '\0'; });
			string str = tmp.str();
			str.erase(remove(str.begin(), str.end(), '\0'), str.end());
			if (str.size() == it->size() - 1)
			{
				cout << "Day2 Answer2: " << str << endl;
				return 0;
			}
		}
	}
	return 0;
}
