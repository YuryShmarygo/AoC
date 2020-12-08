#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include "../util/instruction.h"

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "usage: Generator.exe <input_file>" << endl;
		return 0;
	}

	fstream is(argv[1]);
	ofstream os(S(argv[1] << ".out"));

	vector<Code> codes;
	is >> codes;
	cout << "Read " << codes.size() << " codes" << endl;
	Machine m(codes);
	m.has_call_back = true;
	m.generate(os);

	return 0;
}