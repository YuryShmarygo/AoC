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

class Machine
{
public:
	vector<long long> _mem;
	int _cur;
	int _base;

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
						_mem.resize(_mem[_cur + i] + 1, 0);
					return _mem[_mem[_cur + i]];
				case 1:
					if (_cur + i >= (int)_mem.size())
						_mem.resize(_cur + i + 1, 0);
					return _mem[_cur + i];
				case 2:
					if (_cur + i >= (int)_mem.size())
						_mem.resize(_cur + i + 1, 0);
					if (_base + _mem[_cur + i] >= (int)_mem.size())
						_mem.resize(_base + _mem[_cur + i] + 1, 0);
					return _mem[_base + _mem[_cur + i]];
				default:
					cout << "Invalid parameter mode: " << m[i - 1] << endl;
			};
			throw "Invalid parameter mode";
		}
	};

	Machine(vector<long long>& input, int base = 0) : _mem(input), _cur(0), _base(base) {}

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
				long long value;
				cout << "Enter code:";
				cin >> value;
				op.Param(1) = value;
				shift = 2;
				break;
			}
			case 4:
				cout << op.Param(1) << endl;
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

void Test();
int main()
{
	//Test();
	ifstream is("Day9.txt");
	auto input = ReadInput(is);

	Machine m(input);
	m.Run();

	return 0;
}

void Test()
{
	stringstream is("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");
	auto input = ReadInput(is);
	Machine m(input);
	m.Run();

	is = stringstream("1102,34915192,34915192,7,4,7,99,0");
	input = ReadInput(is);
	m = Machine(input);
	m.Run();

	is = stringstream("104,1125899906842624,99");
	input = ReadInput(is);
	m = Machine(input);
	m.Run();
}
