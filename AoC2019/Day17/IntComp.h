#pragma once
#include <iostream>
#include <vector>

std::istream& operator>>(std::istream& is, std::vector<long long>& input);

class Machine
{
public:
	std::vector<long long> _mem;
	int _cur;
	int _base;

	struct OpCode
	{
		OpCode(std::vector<long long>& mem, int p, int base);
		int code;
		int m[3];
		std::vector<long long>& _mem;
		int _cur;
		int _base;

		long long& Param(int i);
	};

	Machine(std::vector<long long>& input, int base = 0) : _mem(input), _cur(0), _base(base) {}

	void Run(std::vector<long long>& out);
};
