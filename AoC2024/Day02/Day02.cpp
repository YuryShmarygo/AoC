#include "../common.h"

typedef vector<int> report_t;
report_t read_report(istream& is)
{
	istream_iterator<int> start(is), end;
	return report_t(start,end);
}
istream& operator>>(istream& is, report_t& r) 
{
	r.clear();
	string line;
	if (!getline(is, line) || line.empty()) return is;
	auto i = stringstream(line);
	istream_iterator<int> start(i), end;
	r.assign(start, end);
	return is;
}
typedef vector<report_t> input_t;
input_t read_input(istream& is) 
{ 
	string line;
	input_t input;
	while (getline(is, line) && !line.empty())
	{
		auto i = stringstream(line);
		auto r = read_report(i);
		input.push_back(r);
	}
	return input;
}

bool is_safe(const report_t& r)
{
	bool safe = true;
	int dir = r[1] - r[0];
	for (int i = 0; i < r.size() - 1 && safe; ++i)
	{
		int dif = r[i + 1] - r[i];
		safe = abs(dif) >= 1 && abs(dif) <= 3 && (dif > 0 && dir > 0 || dif < 0 && dir < 0);
	}
	return safe;
}
bool is_safe_t(const report_t& r)
{
	bool safe = is_safe(r);
	if (safe)
		return safe;
	for (int i = 0; i < r.size(); ++i)
	{ 
		report_t copy(r);
		copy.erase(copy.begin() + i);
		safe = is_safe(copy);
		if (safe)
			return safe;
	}
	return safe;
}

long long solve1(const input_t& input)
{
	return r::count_if(input, [](auto r) {return is_safe(r); });
}
long long solve2(const input_t& input)
{
	return r::count_if(input, [](auto r) {return is_safe_t(r); });
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
	auto test1 = R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
