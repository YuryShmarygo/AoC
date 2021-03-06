#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;

const int _W = 25;
const int _H = 6;

int main()
{
	vector<vector<int>> image(_H, vector<int>(_W, 2));
	int min0 = _W * _H;
	int res1 = 0;
	ifstream is("Day8.txt");
	while (is)
	{
		vector<int> c_l = { 0, 0, 0 };
		for (int y = 0; y < _H; ++y)
		{
			for (int x = 0; x < _W; ++x)
			{
				char c;
				is >> c;
				int d = c - '0';
				++c_l[d];
				image[y][x] = image[y][x] == 2 ? d : image[y][x];
			}
		}
		if (c_l[0] < min0)
		{
			min0 = c_l[0];
			res1 = c_l[1] * c_l[2];
		}
	}

	cout << "Day8 Answer1: " << res1 << endl;
	
	for (auto r : image)
	{
		for (auto d : r)
			cout << (d == 1 ? '*' : ' ');
		cout << endl;
	}

	return 0;
}
