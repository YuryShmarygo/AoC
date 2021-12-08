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

struct Point
{
	int x;
	int y;
	Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
};
bool operator<(const Point& p1, const Point& p2) { return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y; }
bool operator==(const Point& p1, const Point& p2) { return p1.x == p2.x && p1.y == p2.y; }
Point operator+(const Point& p1, const Point& p2) { return Point{ p1.x + p2.x, p1.y + p2.y }; }
struct Line
{
	Point p1;
	Point p2;
};
istream& operator>>(istream& is, Line& l) 
{
	string line;
	getline(is, line);
	if (line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex("(\\d+),(\\d+) -> (\\d+),(\\d+)")))
	{
		l.p1.x = stoi(m[1]);
		l.p1.y = stoi(m[2]);
		l.p2.x = stoi(m[3]);
		l.p2.y = stoi(m[4]);
	}
	return is;
}

long long CountCrossing(const vector<Line>& lines)
{
	map<Point, int> crossing;
	for (auto it = lines.begin(); it != lines.end(); ++it)
	{
		Point m{ it->p2.x > it->p1.x ? 1 : it->p2.x < it->p1.x ? -1 : 0, it->p2.y > it->p1.y ? 1 : it->p2.y < it->p1.y ? -1 : 0};
		Point p = it->p1;
		while (true)
		{
			++crossing[p];
			if (p == it->p2)
				break;
			p += m;
		}
	}
	return count_if(crossing.begin(), crossing.end(), [](auto& p) {return p.second > 1; });
}

void Test();
int main()
{
	//Test();
	ifstream is("Day5.txt");
	istream_iterator<Line> start(is), end;
	vector<Line> input(start, end);
	//cout << "Read " << input.size() << " lines." << endl;

	vector<Line> filtered;
	copy_if(input.begin(), input.end(), back_inserter(filtered), [](auto& l) {return l.p1.x == l.p2.x || l.p1.y == l.p2.y; });
	cout << "Day5 Answer1: " << CountCrossing(filtered) << endl;
	cout << "Day5 Answer2: " << CountCrossing(input) << endl;
}

void Test()
{
	stringstream is(R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)");
	istream_iterator<Line> start(is), end;
	vector<Line> input(start, end);

	vector<Line> filtered;
	copy_if(input.begin(), input.end(), back_inserter(filtered), [](auto& l) {return l.p1.x == l.p2.x || l.p1.y == l.p2.y; });
	cout << "Day5 Test1: " << CountCrossing(filtered) << endl;
	cout << "Day5 Test2: " << CountCrossing(input) << endl;
}