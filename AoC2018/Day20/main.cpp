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
typedef vector<Point> Points;
typedef vector<string> RoomMap;
ostream& operator<<(ostream& o, const RoomMap& rooms)
{
	for (auto line : rooms)
		o << line << endl;
	return o;
}

struct RoomParser
{
	Point start;
	RoomMap rooms;
	vector<Points> _points;
	RoomParser(string input)
	{
		rooms = { "###","#X#","###" };
		rooms.reserve(10000);
		for (auto r : rooms) r.reserve(10000);
		start = Point{ 1, 1 };

		string::iterator it = input.begin();
		_points = vector<Points>{ Points{Point{0,0}} };
		_points.reserve(10000);
		Parse(_points.back(), it, input);
	}

	Points& Parse(Points& points, string::iterator& it, string& s)
	{
		_points.push_back(points);
		Points& cur = _points.back();
		for (; it != s.end(); ++it)
		{
			switch (*it)
			{
			case 'N':
			case 'S':
			case 'W':
			case 'E':
				for (auto& p : cur)
					p = AddDoor(p, *it);
				break;
			case '|':
			{
				auto newPoints = Parse(points, ++it, s);
				cur.insert(cur.end(), newPoints.begin(), newPoints.end());
				break;
			}
			case '(':
				cur = Parse(cur, ++it, s);
				_points.erase(_points.end() - 1);
				break;
			case ')':
				return cur;
			case '^':
			case '$':
				break;
			default:
				throw exception("Unexpected char in regex");
			};
		}

		return cur;
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
			{
				Enlarge(d);
			}
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
			{
				Enlarge(d);
			}
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

int main()
{
	ifstream is("d20.txt");
	string input;
	getline(is, input);

	auto rooms = RoomParser(input).rooms;
	ofstream o("d20.out");
	o << rooms;

	//cout << "Day20 Answer1: " << " " << endl;
	//cout << "Day20 Answer2: " << " " <<  endl;
	return 0;
}
