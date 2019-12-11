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

vector<long long> ReadInput(istream& is)
{
	vector<long long> input;
	long long val;
	while (is >> val)
	{
		input.push_back(val);
		char separator;
		is >> separator;
	}
	return input;
}

struct Point
{
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};
bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}
void Rotate(Point& p, int i)
{
	static map<Point, Point> _l =
	{
		{Point(0,-1), Point(-1,0)},
		{Point(-1,0), Point(0,1)},
		{Point(0,1), Point(1,0)},
		{Point(1,0), Point(0,-1)}
	};
	static map<Point, Point> _r =
	{
		{Point(0,-1), Point(1,0)},
		{Point(1,0), Point(0,1)},
		{Point(0,1), Point(-1,0)},
		{Point(-1,0), Point(0,-1)}
	};
	if (i == 0)
		p = _r[p];
	else
		p = _l[p];
}
void Move(Point& p, const Point& d)
{
	p.x += d.x;
	p.y += d.y;
}

struct Board : public vector<string>
{
	Point start;
	Board() : vector<string>(1000, string(1000, '.')), start(500,500) {}
};

class Machine
{
public:
	vector<long long> _mem;
	int _cur;
	int _base;

	Board& _board;
	set<Point> vis;
	Point pos;
	Point dir;
	bool isPaint;

	struct OpCode
	{
		OpCode(vector<long long>& mem, int p, int base) : _mem(mem), _cur(p), _base(base)
		{
			code = _mem[p] % 100;
			m[0] = _mem[p] / 100 % 10;
			m[1] = _mem[p] / 1000 % 10;
			m[2] = _mem[p] / 10000 % 10;
			if (m[2] == 1)
				cout << "Invalid mode for output parameter. OpCode: " << _mem[p] << endl;

		}
		int code;
		int m[3];
		vector<long long>& _mem;
		int _cur;
		int _base;

		long long& Param(int i)
		{
			switch (m[i - 1])
			{
			case 0:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				if (_mem[_cur + i] >= (int)_mem.size())
					_mem.resize((const int)(_mem[_cur + i] + 1), 0);
				return _mem[(int)_mem[(int)(_cur + i)]];
			case 1:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				return _mem[_cur + i];
			case 2:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				if (_base + _mem[_cur + i] >= (int)_mem.size())
					_mem.resize(_base + (int)_mem[_cur + i] + 1, 0);
				return _mem[_base + (int)_mem[_cur + i]];
			default:
				cout << "Invalid parameter mode: " << m[i - 1] << endl;
			};
			throw "Invalid parameter mode";
		}
	};

	Machine(vector<long long>& input, Board& board, int base = 0) : _mem(input), _cur(0), _base(base), _board(board)
		, pos(board.start), dir(0,-1), isPaint(true)
	{	}

	void Run()
	{
		int shift = 0;
		while (true)
		{
			auto op = OpCode(_mem, _cur, _base);
			switch (op.code)
			{
			case 99:
				return;
			case 1:
				op.Param(3) = op.Param(1) + op.Param(2);
				shift = 4;
				break;
			case 2:
				op.Param(3) = op.Param(1) * op.Param(2);
				shift = 4;
				break;
			case 3:
			{
				/*long long value;
				cout << "Enter code:";
				cin >> value;*/
				op.Param(1) = _board[pos.x][pos.y] == '.' ? 0 : 1;
				shift = 2;
				break;
			}
			case 4:
				//cout << op.Param(1) << endl;
				if (isPaint)
				{
					_board[pos.x][pos.y] = op.Param(1) == 0 ? '.' : '#';
					vis.insert(pos);
					isPaint = false;
				}
				else
				{
					Rotate(dir, (int)op.Param(1));
					Move(pos, dir);
					isPaint = true;
				}
				shift = 2;
				break;
			case 5:
				if (op.Param(1) != 0)
					shift = (int)(op.Param(2) - _cur);
				else
					shift = 3;
				break;
			case 6:
				if (op.Param(1) == 0)
					shift = (int)(op.Param(2) - _cur);
				else
					shift = 3;
				break;
			case 7:
				op.Param(3) = op.Param(1) < op.Param(2) ? 1 : 0;
				shift = 4;
				break;
			case 8:
				op.Param(3) = op.Param(1) == op.Param(2) ? 1 : 0;
				shift = 4;
				break;
			case 9:
				_base += (int)(op.Param(1));
				shift = 2;
				break;
			default:
				cout << "Invalid code: " << _mem[_cur] << endl;
				break;
			};
			_cur += shift;
		}
	}
};

int main()
{
	ifstream is("Day11.txt");
	auto input = ReadInput(is);

	Board board;
	Machine m(input, board);
	m.Run();

	cout << "Day11, answer1: " << m.vis.size() << endl;

	board = Board();
	board[board.start.x][board.start.y] = '#';
	Machine m2(input, board);
	m2.Run();

	Point start(board[0].size(), board.size());
	Point end(0,0);
	for (int y = 0; y < (int)board.size(); ++y)
	{
		for (int x = 0; x < (int)board[y].size(); ++x)
		{
			if (board[y][x] == '#')
			{
				if (x < start.x)
					start.x = x;
				if (x > end.x)
					end.x = x;
				if (y < start.y)
					start.y = y;
				if (y > end.y)
					end.y = y;
			}
		}
	}
	for (int y = start.y; y <= end.y; ++y)
	{
		for (int x = start.x; x <= end.x; ++x)
		{
			cout << (board[y][x] == '.' ? ' ' : '#');
		}
		cout << endl;
	}

	return 0;
}
