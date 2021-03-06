#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;


size_t React(string s)
{
	for (auto i = s.begin(); i != s.end(); )
	{
		auto j = i + 1;
		if (j != s.end() && toupper(*i) == toupper(*j) && (*i) != (*j))
		{
			i = s.erase(i, ++j);
			if (i != s.begin())
				--i;
		}
		else
			++i;
	}
	return s.size();
}
string Remove(string s, char c)
{
	s.erase(remove_if(s.begin(), s.end(), [c](auto i) {return toupper(i) == toupper(c); }), s.end());
	return s;
}
int main()
{
	//read input file
	string input; ifstream("input.txt") >> input;
	cout << "Read " << input.size() << " characters" << endl;

	cout << React(input) << endl;

	vector<char> g;
	generate_n(back_inserter(g), 'z' - 'a' + 1, []() {static char c = 'a'; return c++; });
	
	auto a = min_element(g.begin(), g.end(), [input](auto c1, auto c2) { return React(Remove(input, c1)) < React(Remove(input, c2)); });
	cout << React(Remove(input, *a)) << endl;
	return 0;
}
