#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <deque>

using namespace std;

struct Vec
{
	int x;
	int y;
	Vec() : x(0), y(0){}
};

class Shift
{
public:
	int dx;
	int dy;
};

class Point
{
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};
bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}
bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}
Point operator+(const Point& p, const Shift& d)
{
	return Point(p.x + d.dx, p.y + d.dy);
}
int Dist(const Point& p1, const Point& p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

Point operator+(const Point& p, const Vec& v)
{
	return Point(p.x + v.x, p.y + v.y);
}

typedef deque<Vec> Wire;


struct Dir
{
	char d;
	int l;
};
typedef vector<Dir> Path;
typedef vector<Path> Wires;

istream& operator>>(istream& is, Dir& d)
{
	is >> d.d >> d.l;
	return is;
}

istream& operator>>(istream& is, Path& p)
{
	Dir d;
	while (is >> d)
	{
		p.push_back(d);
		char c;
		is >> c;
	}
	return is;
}

istream& operator>>(istream& is, Wires& w)
{
	string line;
	while (getline(is, line))
	{
		istringstream iss(line);
		Path p;
		iss >> p;
		w.push_back(p);
	}
	return is;
}



typedef map<char, int> Cmap;
typedef map<Point, Cmap> Steps;

class Board
{
public:
	vector<string> board;
	Point start;
	Steps steps;
	Shift shift;

	Board()
	{
		board.push_back("x");
		start = Point(0, 0);
		steps[start]['a'] = 0;
		steps[start]['b'] = 0;
		shift.dx = shift.dy = 0;
	}

	Point AddDir(char t, Point& p, Dir& d, int& dist)
	{
		dist = 99999999;
		int s = steps[p + shift][t];
		switch (d.d)
		{
			case 'R':
			{
				int to_add = p.x + d.l - (board[p.y].size() - 1);
				if (to_add > 0)
					for (auto& l : board)
						l.insert(l.size(), to_add, 0);
				auto& l = board[p.y];
				for (int x = p.x + 1; x <= p.x + d.l; ++x)
				{
					++s;
					if (l[x] == 0)
					{
						steps[Point(x, p.y) + shift][t] = s;
						l[x] = t;
					}
					else if (l[x] == t)
						;
					else
					{
						if (l[x] != 'x')
						{
							steps[Point(x, p.y) + shift][t] = s;
						}
						l[x] = 'x';
						dist = min(dist, Dist(start, Point(x, p.y)));
					}
				}
				return Point(p.x + d.l, p.y);
			}
			case 'L':
			{
				int to_add = d.l - p.x;
				if (to_add > 0)
				{
					for (auto& l : board)
						l.insert(0, to_add, 0);
					start.x += to_add;
					p.x += to_add;
					shift.dx -= to_add;
				}
				auto& l = board[p.y];
				for (int x = p.x - 1; x >= p.x - d.l; --x)
				{
					++s;
					if (l[x] == 0)
					{
						l[x] = t;
						steps[Point(x, p.y) + shift][t] = s;
					}
					else if (l[x] == t)
						;
					else
					{
						if (l[x] != 'x')
							steps[Point(x, p.y) + shift][t] = s;
						l[x] = 'x';
						dist = min(dist, Dist(start, Point(x, p.y)));
					}
				}
				return Point(p.x - d.l, p.y);
			}
			case 'U':
			{
				int to_add = d.l - p.y;
				if (to_add > 0)
				{
					for (int i = 0; i < to_add; ++i)
						board.insert(board.begin(), string(board[0].size(), 0));
					start.y += to_add;
					p.y += to_add;
					shift.dy -= to_add;
				}
				for (int y = p.y - 1; y >= p.y - d.l; --y)
				{
					++s;
					auto& b = board[y][p.x];
					if (b == 0)
					{
						b = t;
						steps[Point(p.x, y) + shift][t] = s;
					}
					else if (b == t)
						;
					else
					{
						if (b != 'x')
							steps[Point(p.x, y) + shift][t] = s;
						b = 'x';
						dist = min(dist, Dist(start, Point(p.x, y)));
					}

				}
				return Point(p.x, p.y - d.l);
			}
			case 'D':
			{
				int to_add = p.y + d.l - (board.size() - 1);
				if (to_add > 0)
					for (int i = 0; i < to_add; ++i)
						board.insert(board.end(), string(board[0].size(), 0));
				for (int y = p.y + 1; y <= p.y + d.l; ++y)
				{
					++s;
					auto& b = board[y][p.x];
					if (b == 0)
					{
						b = t;
						steps[Point(p.x, y) + shift][t] = s;
					}
					else if (b == t)
						;
					else
					{
						if (b != 'x')
							steps[Point(p.x, y) + shift][t] = s;
						b = 'x';
						dist = min(dist, Dist(start, Point(p.x, y)));
					}
				}
				return Point(p.x, p.y + d.l);
			}
			default:
				throw "Invalid direction: " + d.d;

		}
	}
	int AddPath(char t, Path& p)
	{
		int minD = 9999999;
		Point curP = start;
		for (auto& d : p)
		{
			int dist;
			curP = AddDir(t, curP, d, dist);
			minD = min(minD, dist);
		}
		return minD;
	}
};

int main()
{
	Wires w;
	ifstream is("Day3.txt");
	is >> w;

	cout << "Read " << w.size() << " lines [" << w[0].size() << ", " << w[1].size() << "]" << endl;

	Board b;
	b.AddPath('a', w[0]);
	int dist = b.AddPath('b', w[1]);

	cout << "Day3, answer1: " << dist << endl;
	
	int minInt = 999999;
	for (auto& s : b.steps)
	{
		int sa = s.second['a'];
		int sb = s.second['b'];
		if (sa > 0 && sb > 0)
			minInt = min(minInt, sa + sb);
	}

	cout << "Day3, answer2: " << minInt;
	return 0;
}