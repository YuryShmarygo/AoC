#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
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

	Machine(vector<long>& input) : _mem(input) {}

	void Run()
	{
		int cur = 0;
		int shift = 0;
		while (true)
		{
			auto op = OpCode(_mem, cur);
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
				long value;
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
					shift = op.Param(2) - cur;
				else
					shift = 3;
				break;
			case 6:
				if (op.Param(1) == 0)
					shift = op.Param(2) - cur;
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
				cout << "Invalid code: " << _mem[cur] << endl;
				break;
			};
			cur += shift;
		}
	}
};

int main()
{
	ifstream is("Day5.txt");
	auto input = ReadInput(is);

	cout << "Read " << input.size() << " values" << endl;

	Machine m(input);
	m.Run();

	return 0;
}
