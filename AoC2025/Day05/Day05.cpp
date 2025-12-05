#include "../common.h"

struct frange
{
	long long s;
	long long e;
};
struct input_t
{
	vector<frange> fr;
	vector<long long> ig;
};
input_t read_input(istream& is) 
{
	input_t input;
	string line;
	while (getline(is, line) && !line.empty())
	{
		auto s = stringstream(line);
		frange fr;
		char c;
		s >> fr.s >> c >> fr.e;
		input.fr.push_back(fr);
	}
	while (getline(is, line))
	{
		input.ig.push_back(stoll(line));
	}

	return input;
}

bool is_in(long long i, const frange& fr)
{
	return i >= fr.s && i <= fr.e;
}
bool is_in(long long i, const vector<frange>& fr)
{
	for (auto& f : fr)
		if (is_in(i, f))
			return true;
	return false;
}
long long solve1(const input_t& input)
{
	return r::count_if(input.ig, [&](auto i) {return is_in(i, input.fr); });
}
long long rsize(const frange& f)
{
	return f.e - f.s + 1;
}
frange cross(const frange& f1, const frange& f2)
{
	frange res = f1;

	if (f1.s < f2.s && f1.e >= f2.s && f1.e <= f2.e)
	{
		res.s = f1.s;
		res.e = f2.s - 1;
	}
	else if (f1.s >= f2.s && f1.s <= f2.e && f1.e > f2.e)
	{
		res.s = f2.e+1;
		res.e = f1.e;
	}
	else if (f1.s >= f2.s && f1.e <= f2.e)
	{
		res = { 0, -1 };
	}

	return res;
}
long long solve2(const input_t& input)
{
	input_t res = input;
	for (auto it = res.fr.begin(); it != res.fr.end(); ++it)
	{
		for (auto it2 = res.fr.begin(); it2 != res.fr.end(); ++it2)
		{
			if (it == it2)
				continue;
			
			*it = cross(*it, *it2);
		}
	}
	long long sum = 0;

	for (auto it = res.fr.begin(); it != res.fr.end(); ++it)
		sum += rsize(*it);

	return sum;
}

int main()
{
	test();
	ifstream is("Day05.txt");
	input_t input = read_input(is);

	cout << "Day5 Answer1: " << solve1(input) << endl;
	cout << "Day5 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(3-5
10-14
16-20
12-18

1
5
8
11
17
32)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
