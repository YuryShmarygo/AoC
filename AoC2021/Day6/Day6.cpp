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

void Day(list<int>& fish)
{
	list<int> newf;
	for (auto it = fish.begin(); it != fish.end(); ++it)
	{
		if (*it == 0)
		{
			*it = 6;
			newf.push_back(8);
		}
		else
			--(*it);
	}
	fish.insert(fish.end(), newf.begin(), newf.end());
}

long long Process(list<int> fish, int days)
{
	while (days-- > 0)
		Day(fish);
	return (long long) fish.size();
}

uint64_t Process2(list<int> fish, int days)
{
	vector<uint64_t> fish_days(9);
	for (auto i : fish)
		++fish_days[i];
	for (; days > 0; --days)
	{
		auto tmp = fish_days[0];
		fish_days.erase(fish_days.begin());
		fish_days.push_back(tmp);
		fish_days[6] += tmp;
	}
	return accumulate(fish_days.begin(), fish_days.end(), (uint64_t)0);
}

void Test();
int main()
{
	Test();
	ifstream is("Day6.txt");
	list<int> input;
	string num;
	while (getline(is, num, ',') && !num.empty())
		input.push_back(stoi(num));

	//cout << "Read " << input.size() << " groups" << endl;

	cout << "Day6 Answer1: " << Process2(input, 80) << endl;
	cout << "Day6 Answer2: " << Process2(input, 256) << endl;
}

void Test()
{
	stringstream is(R"(3,4,3,1,2)");
	list<int> input;
	string num;
	while (getline(is, num, ',') && !num.empty())
		input.push_back(stoi(num));
	cout << "Day5 Test1: " << Process2(input, 80) << endl;
	cout << "Day5 Test2: " << Process2(input, 256) << endl;
}