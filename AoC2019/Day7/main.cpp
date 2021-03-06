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

vector<long> ReadInput(istream& is)
{
	vector<long> input;
	long val;
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
	vector<long> _mem;
	long _lastOutput;
	bool _halt;
	int _cur;
	int _phase;
	bool _first;

	struct OpCode
	{
		OpCode(vector<long>& mem, int p) : _mem(mem), cur(p)
		{
			code = _mem[p] % 100;
			m[0] = _mem[p] / 100 % 10;
			m[1] = _mem[p] / 1000 % 10;
			m[2] = _mem[p] / 10000 % 10;
			if (m[2] != 0)
				cout << "Invalid mode for output parameter. OpCode: " << _mem[p];

		}
		int code;
		int m[3];
		vector<long>& _mem;
		int cur;

		long& Param(int i)
		{
			return m[i - 1] == 0 ? _mem[_mem[cur + i]] : _mem[cur + i];
		}
	};

	Machine(vector<long>& input, int phase) : _mem(input), _lastOutput(0), _halt(false), _cur(0), _phase(phase), _first(true) {}

	long Run(long i)
	{
		int shift = 0;
		while (true)
		{
			auto op = OpCode(_mem, _cur);
			switch (op.code)
			{
			case 99:
				_halt = true;
				return _lastOutput;
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
				if (_first)
				{
					op.Param(1) = _phase;
					_first = false;
				}
				else
				{
					op.Param(1) = i;
				}
				shift = 2;
				break;
			}
			case 4:
				_lastOutput = op.Param(1);
				_cur += 2;
				return _lastOutput;
			case 5:
				if (op.Param(1) != 0)
					shift = op.Param(2) - _cur;
				else
					shift = 3;
				break;
			case 6:
				if (op.Param(1) == 0)
					shift = op.Param(2) - _cur;
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
			default:
				cout << "Invalid code: " << _mem[_cur] << endl;
				break;
			};
			_cur += shift;
		}
		cout << "No output" << endl;
		return 0;
	}
};

int main()
{
	ifstream is("Day7.txt");
	auto input = ReadInput(is);

	vector<long> phases = { 0, 1, 2, 3, 4 };
	long maxo = -1;

	do
	{
		long i = 0;
		for (auto p : phases)
		{
			Machine m(input, p);
			auto o = m.Run(i);
			i = o;
		}
		if (i > maxo)
			maxo = i;
	} while (next_permutation(phases.begin(), phases.end()));
	cout << "Day7 Answer1: " << maxo << endl;

	phases = { 5, 6, 7, 8, 9 };
	maxo = -1;
	do
	{
		long i = 0;
		vector<Machine> machines;
		for (auto p : phases)
			machines.push_back(Machine(input, p));
		while (true)
		{
			auto m = machines.begin();
			for (auto p : phases)
			{
				auto o = m->Run(i);
				i = o;
				++m;
			}
			if (i > maxo)
				maxo = i;
			if (prev(m)->_halt)
				break;
		}
	} while (next_permutation(phases.begin(), phases.end()));
	cout << "Day7 Answer2: " << maxo << endl;

	return 0;
}
