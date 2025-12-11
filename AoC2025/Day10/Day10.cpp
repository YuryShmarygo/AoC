#include "../common.h"

struct machine
{
	string indicator;
	vector<vector<int>> buttons;
	vector<int> jolts;
};
typedef vector<machine> input_t;
input_t read_input(istream& is) 
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
	{
		machine ma;
		smatch m;
		regex_search(line, m, regex(R"(\[([.#]+)\] )"));
		ma.indicator = m[1].str();
		line = m.suffix();
		while (regex_search(line, m, regex(R"(\(((?:\d,?)+\)))")))
		{
			string str = m[1].str();
			smatch n;
			vector<int> but;
			while (regex_search(str, n, regex(R"((\d),?)")))
			{
				but.push_back(stoi(n[1].str()));
				str = n.suffix();
			}
			ma.buttons.push_back(but);
			line = m.suffix();
		}
		while (regex_search(line, m, regex(R"((\d+),?)")))
		{
			ma.jolts.push_back(stoi(m[1].str()));
			line = m.suffix();
		}
		res.push_back(ma);
	}
	return res;
}
int pattern_to_num(const string& pattern)
{
	int num = 0;
	for (auto it = pattern.rbegin(); it != pattern.rend(); ++it)
	{
		num *= 2;
		if (*it == '#')
			num += 1;
	}
	return num;
}
int buttons_to_num(const vector<int>& buttons)
{
	int num = 0;
	for (auto b : buttons)
		num |= (1 << b);
	return num;
}
vector<int> buttons_to_nums(const vector<vector<int>>& buttons)
{
	vector<int> nums;
	for (auto& b : buttons)
		nums.push_back(buttons_to_num(b));
	return nums;
}
bool click_buttons(int pattern, int state, vector<int> nums, int clicks)
{
	if (state == pattern)
		return true;
	if (clicks == 0)
		return false;
	for (auto n : nums)
		if (click_buttons(pattern, state ^ n, nums, clicks - 1))
			return true;
	return false;
}
int turn_on(int pattern, vector<int> nums)
{
	for (int clicks = 1; clicks <= 30; ++clicks)
		if (click_buttons(pattern, 0, nums, clicks))
			return clicks;
	return -1;
}
int turn_on(const machine& ma)
{
	int pattern = pattern_to_num(ma.indicator);
	vector<int> nums = buttons_to_nums(ma.buttons);
	return turn_on(pattern, nums);
}

long long solve1(const input_t& input)
{
	return accumulate(input.begin(), input.end(), 0LL, [](long long sum, const machine& ma) { return sum + turn_on(ma); });
}

vector<int> buttons_to_jolts(const vector<int>& buttons, size_t size)
{
	vector<int> res(size, 0);
	for (auto b : buttons)
		res[b] = 1;
	return res;
}
int min_diff(const vector<int>& cur, const vector<int>& buttons, const vector<int>& jolts)
{
	int mind = 99999999;
	for (auto b : buttons)
	{
		int diff = abs(jolts[b] - cur[b]);
		if (diff < mind)
			mind = diff;
	}
	return mind;
}
long long solve2(const input_t& input)
{
	long long sum = 0;
	return sum;
}

int main()
{
	test();
	ifstream is("Day10.txt");
	input_t input = read_input(is);

	cout << "Day10 Answer1: " << solve1(input) << endl;
	cout << "Day10 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
