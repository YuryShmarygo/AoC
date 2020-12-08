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
#include <queue>
#include "../util/instruction.h"

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

struct detect_loop
{
	detect_loop(const vector<Code>& codes) : executed(codes.size(), 0){ }
	bool operator()(int& cur, long long& acc, Code& code)
	{ return ++executed[cur] > 1; }
	vector<char> executed;
	bool is_loop()
	{ return any_of(executed.begin(), executed.end(), [](auto& e) {return e > 1;}); }
	function<bool(int& cur, long long& acc, Code& code)> create_capture() { return [&](int& cur, long long& acc, Code& code) { return (*this)(cur, acc, code); }; }
};
long long check_loop(const vector<Code>& codes)
{
	Machine m(codes, detect_loop(codes));
	return m.run();
}
long long remove_loop(const vector<Code>& codes)
{
	vector<Code> copy(codes);
	for (int i = 0; i < copy.size(); ++i)
	{
		if (copy[i].code == nop || copy[i].code == jmp)
		{
			if (copy[i].code == nop)
				copy[i].code = jmp;
			else
				copy[i].code = nop;

			detect_loop detecter(copy);
			Machine m(copy, detecter.create_capture());
			auto res = m.run();
			if (!detecter.is_loop())
				return res;

			copy[i].code = codes[i].code;
		}
	}
	return 0;
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day8.txt");
	vector<Code> codes;
	is >> codes;
	//cout << "Read " << codes.size() << " instructions" << endl;

	cout << "Day8 Answer1: " << check_loop(codes) << endl;

	cout << "Day8 Answer2: " << remove_loop(codes) << endl;
}

void Test()
{
	stringstream is(R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)");

	istream_iterator<Code> start(is), end;
	vector<Code> codes(start, end);
	cout << "test 1: " << check_loop(codes) << endl;
	cout << "test 2: " << remove_loop(codes) << endl;
}