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
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

int find_turn_old(const vector<int>& start, int turn)
{
	map<int, int> hist;
	for (int i = 0; i < start.size() - 1; ++i)
		hist[start[i]] = i + 1;
	int last = start.back();
	for (int cur = (int)start.size()+1; cur <= turn; ++cur)
	{
		auto it = hist.find(last);
		if (it == hist.end())
		{
			hist[last] = cur - 1;
			last = 0;
		}
		else
		{
			last = cur - 1 - it->second;
			it->second = cur - 1;
		}
	}
	return last;
}
int find_turn(const vector<int>& start, int turn)
{
	vector<int> hist(turn, 0);
	for (int i = 0; i < start.size() - 1; ++i)
		hist[start[i]] = i + 1;
	int last = start.back();
	for (int cur = (int)start.size(); cur < turn; ++cur)
	{
		int* p = &hist[last];
		if (*p == 0)
			last = 0;
		else
			last = cur - *p;
		*p = cur;
	}
	return last;
}
void Test();
int main()
{
	Test();// return 0;
	vector<int> start{ 0,8,15,2,12,1,4 };

	cout << "Day15 Answer1: " << find_turn(start, 2020) << endl;
	cout << "Day15 Answer2: " << find_turn(start, 30000000) << endl;
}

void Test()
{
	vector<vector<int>> tests = { { 0,3,6 },{1,3,2},{2,1,3},{1,2,3},{2,3,1},{3,2,1},{3,1,2}	};
	for (int i = 0; i < tests.size(); ++i)
		cout << "Test " << i+1 << ": " << find_turn(tests[i], 2020) << endl;
	//for (int i = 0; i < tests.size(); ++i)
	//	cout << "Test " << i + 1 + tests.size() << ": " << find_num(tests[i], 30000000) << endl;
}