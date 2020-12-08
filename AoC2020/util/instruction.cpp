#include "instruction.h"
#include<map>
#include<vector>
#include <iomanip>

#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
using namespace std;

map<string, ECode> string_to_code{ {"acc", acc}, {"nop", nop}, {"jmp", jmp} };
vector<string> code_to_string{ "acc", "nop", "jmp" };
istream& operator>>(istream& is, ECode& code) { string str; is >> str; code = string_to_code[str]; return is; }
ostream& operator<<(ostream& os, const ECode& code) { os << code_to_string[code]; return os; }

istream& operator>>(istream& is, Code& code) { is >> code.code >> code.num; return is; }
istream& operator>>(istream& is, vector<Code>& codes) { copy(istream_iterator<Code>(is), istream_iterator<Code>(), back_inserter(codes)); return is; }
ostream& operator<<(ostream& os, const Code& code) { os << code.code << ' ' << code.num; return os; }
ostream& operator<<(ostream& os, const vector<Code>& codes) { copy(codes.begin(), codes.end(), ostream_iterator<Code>(os, "\n")); return os; }

auto no_call_back = [](int& cur, long long& acc, Code& code) {return false; };
Machine::Machine(const std::vector<Code>& pcodes) 
	: codes(pcodes), acc(0), cur(0), has_call_back(false), call_back(no_call_back)
{ }

Machine::Machine(const std::vector<Code>& pcodes, std::function<bool(int& cur, long long& acc, Code& code)> pcall_back) 
	: codes(pcodes), acc(0), cur(0), has_call_back(true), call_back(pcall_back)
{ }

long long Machine::run()
{
	static vector < function<int(long long& acc, const Code& code)>> instructions{
		[](long long& acc, const Code& code) { acc += code.num; return 1; },
		[](long long& acc, const Code& code) { return 1; },
		[](long long& acc, const Code& code) { return code.num; } };
	while (cur < codes.size())
	{
		if (has_call_back && call_back(cur, acc, codes[cur]))
			return acc;
		cur += instructions[codes[cur].code](acc, codes[cur]);
	}
	return acc;
}

string Lbl(int i) { return S("l_" << i); }
void Machine::generate(ostream& os)
{
	static vector < function<string(int pos, const Code& code)>> instructions{
		[](int pos, const Code& code) { return S("acc += " << code.num << ";"); },
		[](int pos, const Code& code) { return S(";"); },
		[](int pos, const Code& code) { return S("goto " << Lbl(pos + code.num) << ";"); } };

	os << "#include <functional>" << endl;
	os << "#include \"../util/instruction.h\"" << endl << endl;
	os << "#pragma warning (disable : 4102)" << endl << endl;
	os << "using namespace std;" << endl << endl;
	os << "long long run(" << (has_call_back ? "function<bool(int& cur, long long& acc, Code& code)> call_back" : "") << ")" << endl << "{" << endl;
	os << "\t" << "long long acc = 0;" << endl;
	if (has_call_back)
	{
		os << "\t" << "int cur = 0;" << endl;
		os << "\t" << "Code code;" << endl;
	}
	for (int i = 0; i < codes.size(); ++i)
	{
		os << left << setw(5) << Lbl(i) << ": " << endl;
		if (has_call_back)
		{
			os << "\t" << "cur = " << i << ";" << endl;
			os << "\t" << "code = Code{ECode::" << codes[cur].code << ", " << codes[cur].num << "};" << endl;
			os << "\t" << "if (call_back(cur, acc, code))" << endl;
			os << "\t\t" << "return acc;" << endl;
		}
		os << "\t" << setw(15) << instructions[codes[i].code](i, codes[i]) << " //" << codes[i] << endl;
	}
	os << left << setw(5) << Lbl((int)codes.size()) << ": //END" << endl;

	os << "\t" << "return acc;" << endl << "}";
}
