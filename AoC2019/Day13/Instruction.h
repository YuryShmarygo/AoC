#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

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
};
class Instruction
{

};

#endif // _INSTRUCTION_H_