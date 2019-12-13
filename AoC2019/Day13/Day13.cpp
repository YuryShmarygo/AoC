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

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

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
enum ETileType { Empty, Wall, Block, Paddle, Ball};
ostream& operator<<(ostream& os, ETileType tile)
{
	switch (tile)
	{
	case Wall: os << 'W'; break;
	case Block: os << 'B'; break;
	case Paddle: os << 'P'; break;
	case Ball: os << '8'; break;
	default:
	case Empty: os << ' '; break;
	}
	return os;
}
enum EState { XCapture, YCapture, TileCapture, ScoreCapture};
struct Board : public vector<vector<ETileType>>
{
	Board() : vector<vector<ETileType>>(1000, vector<ETileType>(1000, Empty)) {}
	void CheckSize(int x, int y)
	{
		if (x >= (int)(*this)[0].size())
			for_each(begin(), end(), [&](auto& l) {l.resize(x + 1, Empty); });
		if (y >= (int)size())
			resize(y + 1, vector<ETileType>((*this)[0].size(), Empty));
	}
};
ostream& operator<<(ostream& os, const Board& b)
{
	int max_y = 0;
	for (int y = 0; y < b.size(); ++y)
		if (find_if(b[y].begin(), b[y].end(), [&](auto& e) {return e != Empty; }) != b[y].end() && y > max_y)
			max_y = y;
	int max_x = 0;
	for (auto& l : b)
		for (int x = 0; x < l.size(); ++x)
			if (l[x] != Empty && x > max_x)
				max_x = x;
	for (int y = 0; y < max_y; ++y)
	{
		for (int x = 0; x < max_x; ++x)
			os << b[y][x];
		os << endl;
	}
	return os;
}
enum EJoystick {Neutral, Left, Right};

class Machine
{
public:
	vector<long long> _mem;
	int _cur;
	int _base;

	Board& _board;
	EState _state;
	int _x;
	int _y;
	int _score;
	EJoystick _joystick;
	ostream& _trace;

	struct OpCode
	{
		struct Param
		{
			OpCode& _op;
			int _mode;
			int _offset;
			
			Param(OpCode& op, int mode, int offset) : _op(op), _mode(mode), _offset(offset) {}
			void CheckSize(int size)
			{
				if ((size_t)size >= _op._mem.size())
					_op._mem.resize(size + 1);
			}
			long long& GetValue()
			{
				switch (_mode)
				{
				case 0:
					CheckSize(_op._cur + _offset);
					CheckSize(_op._mem[_op._cur + _offset]);
					return _op._mem[(int)_op._mem[(int)(_op._cur + _offset)]];
				case 1:
					CheckSize(_op._cur + _offset);
					return _op._mem[_op._cur + _offset];
				case 2:
					CheckSize(_op._cur + _offset);
					CheckSize(_op._base + _op._mem[_op._cur + _offset]);
					return _op._mem[_op._base + (int)_op._mem[_op._cur + _offset]];
				default:
					cout << "Invalid parameter mode: " << _mode << endl;
				};
				throw exception("Invalid parameter mode");
			}
			ostream& operator<<(ostream& os) const
			{
				switch (_mode)
				{
				case 0:
					os << "**(" << _op._cur + _offset << "):*(" << _op._mem[(int)_op._cur + _offset] << "):" << _op._mem[(int)_op._mem[(int)(_op._cur + _offset)]];
					break;
				case 1:
					os << "*(" << _op._cur + _offset << "):(" << _op._mem[(int)_op._cur + _offset] << ")";
					break;
				case 2:
					os <<"*b[" << _op._base << "](" << _op._cur + _offset << "):*[" << _op._base << "](" << _op._mem[(int)_op._cur + _offset] << "):" << _op._mem[_op._base + (int)_op._mem[_op._cur + _offset]];
					break;
				default:
					os << "Invalid parameter mode: " << _mode << endl;
				};
				return os;
			}
		};
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
		string _op;
		string _param[3];

		long long& Param(int i)
		{
			switch (m[i - 1])
			{
			case 0:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				if (_mem[_cur + i] >= (int)_mem.size())
					_mem.resize((const int)(_mem[_cur + i] + 1), 0);
				_param[i - 1] = S("**(" << _cur + 1 << "):*(" << _mem[(int)_cur + i] << "):" << _mem[(int)_mem[(int)(_cur + i)]]);
				return _mem[(int)_mem[(int)(_cur + i)]];
			case 1:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				_param[i - 1] = S("*(" << _cur + 1 << "):(" << _mem[(int)_cur + i] << ")");
				return _mem[_cur + i];
			case 2:
				if (_cur + i >= (int)_mem.size())
					_mem.resize(_cur + i + 1, 0);
				if (_base + _mem[_cur + i] >= (int)_mem.size())
					_mem.resize(_base + (int)_mem[_cur + i] + 1, 0);
				_param[i - 1] = S("*b[" << _base << "](" << _cur + 1 << "):*[" << _base << "](" << _mem[(int)_cur + i] << "):" << _mem[_base + (int)_mem[_cur + i]]);
				return _mem[_base + (int)_mem[_cur + i]];
			default:
				cout << "Invalid parameter mode: " << m[i - 1] << endl;
			};
			throw "Invalid parameter mode";
		}
	};

	Machine(vector<long long>& input, ostream& trace, Board& board, int base = 0) : _mem(input), _cur(0), _base(base), _board(board)
		, _x(0), _y(0), _state(XCapture), _score(0), _joystick(Neutral), _trace(trace)
	{	}

	void Run()
	{
		int shift = 0;
		while (true)
		{
			auto op = OpCode(_mem, _cur, _base);
			_trace << "OP(" << _cur << ": " << op.code << " ";
			switch (op.code)
			{
			case 99:
				_trace << "EXIT)" << endl;
				return;
			case 1:
				_trace << "ADD)(" << op._param[0] << ", " << op._param[1] << ", " << op._param[2] << ")";
				op.Param(3) = op.Param(1) + op.Param(2);
				shift = 4;
				_trace << " ==> " << op.Param(3) << endl;
				break;
			case 2:
				_trace << "MUL)(" << op._param[0] << ", " << op._param[1] << ", " << op._param[2] << ")";
				op.Param(3) = op.Param(1) * op.Param(2);
				shift = 4;
				_trace << " ==> " << op.Param(3) << endl;
				break;
			case 3:
			{
				_trace << "IN)(" << op._param[0] << ")";
				cout << _board;
				long long value;
				cout << "Enter code:";
				cin >> value;
				op.Param(1) = value;
				_trace << " ==> " << op.Param(1) << endl;
				shift = 2;
				break;
			}
			case 4:
				_trace << "OUT)(" << op._param[0] << ")";
				//cout << op.Param(1) << endl;
				switch (_state)
				{
				case XCapture:
					_x = (int)op.Param(1);
					_state = YCapture;
					_trace << " ==> _x = " << _x << endl;
					break;
				case YCapture:
					_y = (int)op.Param(1);
					_state = TileCapture;
					_trace << " ==> _y = " << _y << endl;
					if (_x == -1 && _y == 0)
						_state = ScoreCapture;
					break;
				case TileCapture:
					_board.CheckSize(_x, _y);
					if (_x < 0 || _y < 0)
						cout << "Inalid coordinates _x(" << _x << "), _y(" << _y << ")" << endl;
					_board[_y][_x] = (ETileType)op.Param(1);
					_state = XCapture;
					_trace << " ==> _board[" << _y << "][" << _x << "] = " << (ETileType)op.Param(1) << endl;
					break;
				case ScoreCapture:
					_score = (int)op.Param(1);
					_state = XCapture;
					cout << "New score: " << _score << endl;
					_trace << " ==> _score = " << _score << endl;
					break;
				default:
					cout << "Invalid state[" << _state << "]" << endl;
				}
				shift = 2;
				break;
			case 5:
				_trace << "IF NOT 0)(" << op._param[0] << ")";
				if (op.Param(1) != 0)
					shift = (int)(op.Param(2) - _cur);
				else
					shift = 3;
				_trace << " ==> next op address: " << _cur + shift << endl;
				break;
			case 6:
				_trace << "IF == 0)(" << op._param[0] << ")";
				if (op.Param(1) == 0)
					shift = (int)(op.Param(2) - _cur);
				else
					shift = 3;
				_trace << " ==> next op address: " << _cur + shift << endl;
				break;
			case 7:
				_trace << "LESS)(" << op._param[0] << ", " << op._param[1] << ", " << op._param[2] << ")";
				op.Param(3) = op.Param(1) < op.Param(2) ? 1 : 0;
				shift = 4;
				_trace << " ==> " << op.Param(3) << endl;
				break;
			case 8:
				_trace << "EQ)(" << op._param[0] << ", " << op._param[1] << ", " << op._param[2] << ")";
				op.Param(3) = op.Param(1) == op.Param(2) ? 1 : 0;
				shift = 4;
				_trace << " ==> " << op.Param(3) << endl;
				break;
			case 9:
				_trace << "BASE+=)(" << op._param[0] << ")";
				_base += (int)(op.Param(1));
				_trace << " ==> _base = " << _base << endl;
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
	ifstream is("Day13.txt");
	auto input = ReadInput(is);

	ofstream os("trace.log");

	Board board;
	Machine m(input, os, board);
	m.Run();

	long long s = 0;
	for (auto& l : board)
		s += count_if(l.begin(), l.end(), [&](auto t) {return t == Block; });
	cout << "Day13, answer1: " << s << endl;

	ofstream os2("trace2.log");
	input[0] = 2;
	board = Board();
	Machine m2(input, os2, board);
	m2.Run();

	cout << "Day 13, answer2: " << m._score << endl;

	return 0;
}
