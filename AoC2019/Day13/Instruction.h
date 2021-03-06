#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_
#include <vector>

struct Instruction
{
	std::vector<long long>& _mem;
	int _cur;
	int _base;
	int _code;
	Instruction(std::vector<long long>& mem, int p, int base) : _mem(mem), _cur(p), _base(base)
	{
		_code = _mem[p] % 100;
		m[0] = _mem[p] / 100 % 10;
		m[1] = _mem[p] / 1000 % 10;
		m[2] = _mem[p] / 10000 % 10;
		if (m[2] == 1)
			cout << "Invalid mode for output parameter. OpCode: " << _mem[p] << endl;

	}
};
class Instruction
{

};

#endif // _INSTRUCTION_H_