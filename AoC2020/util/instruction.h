#pragma once
#include <sstream>
#include <vector>
#include <functional>

enum ECode { acc, nop, jmp };
std::istream& operator>>(std::istream& is, ECode& code);
std::ostream& operator<<(std::ostream& os, const ECode& code);
struct Code
{
	ECode code;
	int num;
};
std::istream& operator>>(std::istream& is, Code& code);
std::istream& operator>>(std::istream& is, std::vector<Code>& codes);
std::ostream& operator<<(std::ostream& os, const Code& code);
std::ostream& operator<<(std::ostream& os, const std::vector<Code>& codes);

struct Machine
{
	Machine(const std::vector<Code>& codes);
	Machine(const std::vector<Code>& codes, std::function<bool(int& cur, long long& acc, Code& code)> call_back);

	long long run();
	void generate(std::ostream& os);

	std::vector<Code> codes;
	long long acc;
	int cur;
	bool has_call_back;
	std::function<bool(int& cur, long long& acc, Code& code)> call_back;
};
