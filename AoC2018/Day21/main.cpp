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
#include <unordered_map>
#include <unordered_set>

#include "Instruction.h"

using namespace std;
using namespace placeholders;

long long Solve2(int ipBound, const vector<Instr>& instr)
{
	long long last = 0;
	set<long long> ans;
	auto reg = Processor().Process(ipBound, instr, { 0,0,0,0,0,0 }, [&](int ip, Registers& reg, const Instr& instr)
	{
		if (ip == 28)
		{
			if (ans.end() != ans.find(reg[3]))
			{
				reg[3] = last;
				return true;
			}
			ans.insert(reg[3]);
			last = reg[3];
		}
		return false;
	});
	return reg[3];
}
std::vector<long long> Process(const std::vector<long long>& r);
int main()
{
	Converter::Convert("d21.txt", "d21_processor.cpp"); return 0;
	auto res = Process({ 0,0,0,0,0,0 });
	cout << "Day21 Answer1: " << res[0] << endl;
	cout << "Day21 Answer2: " << res[1] << endl;
	/* old slow solution
	auto input = ReadInstructions("d21.txt");
	cout << "Read " << input.second.size() << " instructions" << endl;

	input.second[28] = Instr{ "seti", (int)input.second.size(), 0, input.first };//exit
	auto reg = Processor().Process(input.first, input.second);
	cout << "Day21 Answer1: " << reg[3] << endl;
	
	input.second[28] = Instr{ "eqrr", 3, 0, 5 }; // put back original instruction
	cout << "Day21 Answer2: " << Solve2(input.first, input.second) << endl;
	*/
	return 0;
}
