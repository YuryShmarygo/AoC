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
#include <unordered_map>

using namespace std;
using namespace placeholders;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

struct Point
{
	int x;
	int y;
	int z;
	int t;
};
istream& operator>>(istream& is, Point& p)
{
	string line;
	getline(is, line);
	sscanf_s(line.c_str(), "%d,%d,%d,%d", &p.x, &p.y, &p.z, &p.t);
	return is;
}
int Distance(const Point& p1, const Point& p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z) + abs(p1.t - p2.t);
}
typedef vector<Point> Group;
bool IsClose(const Point& p, const Group& g)
{
	for (auto& pg : g)
		if (Distance(p, pg) <= 3)
			return true;
	return false;
}
vector<Group> BuildGroups(const vector<Point>& input)
{
	vector<Group> groups;
	groups.reserve(input.size());
	for (auto& p : input)
	{
		vector<vector<Group>::iterator> close;
		for (auto it = groups.begin(); it != groups.end(); ++it)
			if (IsClose(p, *it))
				close.push_back(it);
		if (close.empty())
			groups.push_back({ p });
		else
		{
			close[0]->push_back(p);
			for (int i = (int)close.size() - 1; i >= 1; --i)
			{
				close[0]->insert(close[0]->end(), close[i]->begin(), close[i]->end());
				groups.erase(close[i]);
			}
		}
	}
	return groups;
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("d25.txt");
	istream_iterator<Point> start(is), end;
	vector<Point> input(start, end);
	cout << "Read " << input.size() << " points" << endl;

	auto groups = BuildGroups(input);

	cout << "Day25 Answer1: " << groups.size() << endl;
	cout << "Day25 Answer2: " << "" <<  endl;
	return 0;
}

void Test()
{
}