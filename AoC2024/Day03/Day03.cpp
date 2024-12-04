#include "../common.h"

typedef string input_t;
input_t read_input(istream& is)
{
	string line;
	input_t input;
	while (getline(is, line) && !line.empty())
		input = input + line;
	return input;
}
struct instr_t
{
	bool enabled;
	int n1;
	int n2;
};
/*
typedef vector<instr_t> input_t;
input_t read_input(istream& is) 
{ 
	string line;
	input_t input;
	while (getline(is, line) && !line.empty())
	{
		instr_t i;
		smatch m;
		while (regex_search(line, m, regex(R"(mul\((\d+),(\d+)\))")))
		{
			i.n1 = stoi(m[1].str());
			i.n2 = stoi(m[2].str());
			input.push_back(i);
			line = m.suffix();
		}
	}
	return input;
}
*/
long long solve1(const input_t& input)
{
	string line = input;
	vector<instr_t> instr;
	smatch m;
	while (regex_search(line, m, regex(R"(mul\((\d+),(\d+)\))")))
	{
		instr_t i;
		i.n1 = stoi(m[1].str());
		i.n2 = stoi(m[2].str());
		instr.push_back(i);
		line = m.suffix();
	}

	return accumulate(instr.begin(), instr.end(), (long long)0, [](long long sum, const instr_t& i) { return sum + i.n1 * i.n2; });
}
long long solve2(const input_t& input)
{
	string line = input;
	vector<instr_t> instr;
	smatch m;
	bool enabled = true;
	while (regex_search(line, m, regex(R"(mul\((\d+),(\d+)\)|don't\(\)|do\(\))")))
	{
		instr_t i;
		if (m[0].str() == "don't()")
			enabled = false;
		else if (m[0].str() == "do()")
			enabled = true;
		else
		{
			i.enabled = enabled;
			i.n1 = stoi(m[1].str());
			i.n2 = stoi(m[2].str());
			instr.push_back(i);
		}
		line = m.suffix();
	}

	//return accumulate(instr.begin(), instr.end(), (long long)0, [](long long sum, const instr_t& i) { return sum + i.enabled ? i.n1 * i.n2 : 0; });
	long long sum = 0;
	for (auto i : instr)
		sum += i.enabled ? i.n1 * i.n2 : 0;
	return sum;
}

int main()
{
	test();
	ifstream is("Day03.txt");
	input_t input = read_input(is);

	cout << "Day3 Answer1: " << solve1(input) << endl;
	cout << "Day3 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))";
	auto test2 = R"(xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5)))";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test2: " << solve2(input) << endl;
}
