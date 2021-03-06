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

struct Point
{
	int x;
	int y;
};
bool operator<(const Point& p1, const Point& p2)
{ return p1.y < p2.y || p1.y == p2.y && p1.x < p2.x; }
typedef set<Point> Points;
typedef vector<string> RoomMap;
ostream& operator<<(ostream& o, const RoomMap& rooms)
{
	for (auto line : rooms)
		o << line << endl;
	return o;
}

struct RoomParser
{
	RoomMap rooms;
	Point start;
	RoomParser(string input)
	{
		rooms = { "###","#X#","###" };
		rooms.reserve(10000);
		start = Point{ 1,1 };
		for (auto r : rooms) r.reserve(10000);
		Parse(input);
	}

	void Parse(string s)
	{
		list<Points> points(1, Points());
		points.back().insert(Point{ 0,0 });

		for (auto c : s)
		{
			auto cur = prev(points.end());
			switch (c)
			{
			case 'N': case 'S': case 'W': case 'E':
			{
				Points newP;
				for (auto& p : *cur)
					newP.insert(AddDoor(p, c));
				*cur = newP;
				break;
			}
			case '(':
				points.insert(points.end(), Points()); //group result
				points.insert(points.end(), *cur); // create copy
				break;
			case '|':
			{
				auto group = prev(points.end(), 2);
				group->insert(cur->begin(), cur->end());
				*cur = *prev(points.end(), 3);
				break;
			}
			case ')':
			{
				auto group = prev(points.end(), 2);
				group->insert(cur->begin(), cur->end());
				points.pop_back();
				points.erase(prev(points.end(), 2));
				cur = prev(points.end());
				break;
			}
			case '^': case '$':
				break;
			default:
				throw exception("Unexpected char in regex");
			};
		}
	}

	Point AddDoor(Point dp, char d)
	{
		Point p{ start.x + dp.x, start.y + dp.y };
		switch (d)
		{
		case 'N':
			if (p.y == 1)
			{
				Enlarge(d);
				p.y = 3;
			}
			rooms[p.y-1][p.x] = '-';
			rooms[p.y-2][p.x] = '.';
			return Point{ p.x - start.x, p.y - start.y - 2 };
		case 'S':
			if (p.y == rooms.size() - 2)
				Enlarge(d);
			rooms[p.y + 1][p.x] = '-';
			rooms[p.y + 2][p.x] = '.';
			return Point{ p.x - start.x, p.y - start.y + 2 };
		case 'W':
			if (p.x == 1)
			{
				Enlarge(d);
				p.x = 3;
			}
			rooms[p.y][p.x - 1] = '|';
			rooms[p.y][p.x - 2] = '.';
			return Point{ p.x - start.x - 2, p.y - start.y };
		case 'E':
			if (p.x == rooms[0].size() - 2)
				Enlarge(d);
			rooms[p.y][p.x + 1] = '|';
			rooms[p.y][p.x + 2] = '.';
			return Point{ p.x - start.x + 2, p.y - start.y };
		default:
			throw exception("Unexpected direction in AddDoor");
		};
	}
	void Enlarge(char d)
	{
		switch (d)
		{
		case 'N':
			rooms.insert(rooms.begin(), string(rooms[0].size(), '#'));
			rooms.insert(rooms.begin(), string(rooms[0].size(), '#'));
			start.y += 2;
			break;
		case 'S':
			rooms.insert(rooms.end(), string(rooms[0].size(), '#'));
			rooms.insert(rooms.end(), string(rooms[0].size(), '#'));
			break;
		case 'W':
			for (auto& l : rooms)
				l.insert(0, "##");
			start.x += 2;
			break;
		case 'E':
			for (auto& l : rooms)
				l.insert(l.size(), "##");
			break;
		};
	}
};

pair<int, int> GetDepth(const RoomMap& rooms)
{
	Point start;
	for (int y = 0; y < rooms.size(); ++y)
		for (int x = 0; x < rooms[y].size(); ++x)
			if (rooms[y][x] == 'X')
				start = Point{ x, y };
	vector<vector<char>> visit{ rooms.size(), vector<char>(rooms[0].size(), 0) };
	list<Point> cur(1, start);
	visit[start.y][start.x] = 1;
	int depth = 0;
	int r1 = 0;
	while (cur.size() > 0)
	{
		list<Point> next;
		for (auto p : cur)
		{
			if (rooms[p.y - 1][p.x] == '-' && visit[p.y - 2][p.x] == 0)
				next.push_back(Point{ p.x, p.y - 2 });
			if (rooms[p.y][p.x - 1] == '|' && visit[p.y][p.x - 2] == 0)
				next.push_back(Point{ p.x - 2, p.y });
			if (rooms[p.y][p.x + 1] == '|' && visit[p.y][p.x + 2] == 0)
				next.push_back(Point{ p.x + 2, p.y });
			if (rooms[p.y + 1][p.x] == '-' && visit[p.y + 2][p.x] == 0)
				next.push_back(Point{ p.x, p.y + 2 });
			for (auto n : next)
				visit[n.y][n.x] = 1;
			if (depth >= 1000)
				++r1;
		}
		cur = next;
		depth++;
	}

	return make_pair(depth-1, r1);
}

int main()
{
	ifstream is("d20.txt");
	string input;
	getline(is, input);

	auto rooms = RoomParser(input).rooms;
	//ofstream o("d20.out"); o << rooms;

	auto res = GetDepth(rooms);
	cout << "Day20 Answer1: " << res.first << endl;
	cout << "Day20 Answer2: " << res.second <<  endl;
	return 0;
}
