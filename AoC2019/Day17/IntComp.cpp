#include "IntComp.h"


using namespace std;

istream& operator>>(istream& is, vector<long long>& input)
{
	long long val;
	while (is >> val)
	{
		input.push_back(val);
		char separator;
		is >> separator;
	}
	return is;
}

Machine::OpCode::OpCode(vector<long long>& mem, int p, int base) : _mem(mem), _cur(p), _base(base)
{
	code = _mem[p] % 100;
	m[0] = _mem[p] / 100 % 10;
	m[1] = _mem[p] / 1000 % 10;
	m[2] = _mem[p] / 10000 % 10;
	if (m[2] == 1)
		cout << "Invalid mode for output parameter. OpCode: " << _mem[p] << endl;

}
long long& Machine::OpCode::Param(int i)
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

void Machine::Run(vector<long long>& out)
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
			//cout << op.Param(1) << endl;
			out.push_back(op.Param(1));
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
