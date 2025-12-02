#include "../common.h"

struct ids
{
	long long s;
	long long e;
};
typedef vector<ids> input_t;
input_t read_input(istream& is) 
{
	string line;
	getline(is, line);
	vector<ids> input;
	smatch m;
	while (regex_search(line, m, regex(R"((\d+)-(\d+),?)")))
	{
		ids i;
		i.s = stoll(m[1].str());
		i.e = stoll(m[2].str());
		input.push_back(i);
		line = m.suffix();
	}
	return input;
}

bool is_valid(long long n)
{
	string s = to_string(n);
	if (s.size() % 2 != 0)
		return true;
	return s.substr(0, s.size() / 2) != s.substr(s.size() / 2);
}
long long solve1(const input_t& input)
{
	long long sum = 0;
	
	for (auto& i : input)
	{
		for (long long n = i.s; n <= i.e; ++n)
		{ 
			if (!is_valid(n))
				sum += n;
		}
	}

	return sum;
}
bool is_valid2(long long n)
{
	string s = to_string(n);
	for (int l = 1; l <= s.size() / 2; ++l)
	{
		if (s.size() % l != 0)
			continue;
		bool matched = true;
		string sub = s.substr(0, l);
		for (auto it = s.begin() + l; it < s.end() - l + 1; it += l)
		{
			if (!equal(sub.begin(), sub.end(), it))
			{
				matched = false;
				break;
			}	
		}
		if (matched)
			return false;
	}
	return true;
}
long long solve2(const input_t& input)
{
	long long sum = 0;

	for (auto& i : input)
	{
		for (long long n = i.s; n <= i.e; ++n)
		{
			if (!is_valid2(n))
				sum += n;
		}
	}

	return sum;
}

int main()
{
	test();
	ifstream is("Day02.txt");
	input_t input = read_input(is);

	cout << "Day2 Answer1: " << solve1(input) << endl;
	cout << "Day2 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
