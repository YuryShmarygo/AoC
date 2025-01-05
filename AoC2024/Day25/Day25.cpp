#include "../common.h"

typedef vector<vector<string>> input_t;
input_t read_input(istream& is)
{
	input_t res;
	string line;
	vector<string> next;
	while (getline(is, line))
	{
		if (line.empty())
		{
			res.push_back(next);
			next.clear();
		}
		else
			next.push_back(line);
	}
	res.push_back(next);
	return res;
}
typedef vector<int> lkey_t;
lkey_t to_lkey(const vector<string>& strs)
{
	lkey_t res(strs[0].size(), 0);
	for (auto& s : strs)
		for (int i = 0; i < s.size(); ++i)
			res[i] += (s[i] == '#' ? 1 : 0);
	return res;
}
bool fits(const lkey_t& l, const lkey_t& k, int max_h)
{
	for (int i = 0; i < l.size(); ++i)
		if (l[i] + k[i] > max_h)
			return false;
	return true;
}
long long solve1(const input_t& input)
{
	vector<lkey_t> locks;
	vector<lkey_t> keys;
	string all_row(input[0][0].size(), '#');
	for (auto& strs : input)
		if (strs[0] == all_row)
			locks.push_back(to_lkey(strs));
		else
			keys.push_back(to_lkey(strs));
	int max_h = (int)input[0].size();
	long long res = 0;
	for (auto& l : locks)
		for (auto& k : keys)
			res += (fits(l, k, max_h) ? 1 : 0);

	return res;
}
long long solve2(const input_t& input)
{
	int res = 0;
	return res;
}

int main()
{
	test();
	ifstream is("Day25.txt");
	input_t input = read_input(is);

	//24019 too high
	//10810 too high
	cout << "Day25 Answer1: " << solve1(input) << endl;
	cout << "Day25 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(#####
.####
.####
.####
.#.#.
.#...
.....

#####
##.##
.#.##
...##
...#.
...#.
.....

.....
#....
#....
#...#
#.#.#
#.###
#####

.....
.....
#.#..
###..
###.#
###.#
#####

.....
.....
.....
#....
#.#..
#.#.#
#####)";


	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1 (expect 7): " << solve1(input) << endl;
	//is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve2(input) << endl;
}
