#include "../common.h"

bool check_aj(const vector<string>& input, int y, int x, function<bool(char)> check)
{
	int my = (int)input.size() - 1;
	int mx = (int)input[0].size() - 1;
	return x > 0 && y > 0 && check(input[y - 1][x - 1])
		|| y > 0 && check(input[y - 1][x])
		|| y > 0 && x < mx && check(input[y - 1][x + 1])
		|| x > 0 && check(input[y][x - 1])
		|| x < mx && check(input[y][x + 1])
		|| x > 0 && y < my && check(input[y + 1][x - 1])
		|| y < my && check(input[y + 1][x])
		|| x < mx && y < my && check(input[y + 1][x + 1]);
}
bool is_sym(const char c) { return c != '.' && (c < '0' || c > '9'); }
bool check_aj(const vector<string>& input, int y, int x) { return check_aj(input, y, x, is_sym); }
long long solve1(const vector<string>& input)
{
	long long sum = 0;

	for (int y = 0; y < input.size(); ++y)
	{
		auto& line = input[y];
		long long cur = 0;
		bool is_num = false;
		bool is_aj = false;
		for (int x = 0; x < line.size(); ++x)
		{
			auto& c = line[x];
			if (c >= '0' && c <= '9')
			{
				is_aj = is_aj || check_aj(input, y, x);
				if (is_num)
				{
					cur *= 10;
				}
				else
				{
					is_num = true;
					cur = 0;
				}
				cur += c - '0';
			}
			else
			{
				if (is_aj)
					sum += cur;
				is_num = false;
				is_aj = false;
			}
		}
		if (is_aj)
			sum += cur;
	}

	return sum;
}

struct pos
{
	int x;
	int y;
};
pos operator+(const pos& p1, const pos& p2) { return { p1.x + p2.x, p1.y + p2.y }; }
bool operator==(const pos& p1, const pos& p2) { return p1.x == p2.x && p1.y == p2.y; }
bool operator<(const pos& p1, const pos& p2) { return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y; }
struct num
{
	int n;
	set<pos> g;
};
set<pos>& operator+=(set<pos>& l, const set<pos>& r) { l.insert(r.begin(), r.end()); return l; }
set<pos> find_g(const vector<string>& input, const pos& p)
{
	static vector<pos> moves = { {-1,-1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

	int my = (int)input.size() - 1;
	int mx = (int)input[0].size() - 1;

	set<pos> res;
	for (auto& m : moves)
	{
		pos cur = p + m;
		if (cur.x >= 0 && cur.x <= mx && cur.y >= 0 && cur.y <= my && input[cur.y][cur.x] == '*')
			res.insert(cur);
	}

	return res;
}
long long solve2(const vector<string>& input)
{
	vector<num> nums;
	for (int y = 0; y < input.size(); ++y)
	{
		auto& line = input[y];
		num cur = { 0, set<pos>() };
		bool is_num = false;
		for (int x = 0; x < line.size(); ++x)
		{
			auto& c = line[x];
			if (c >= '0' && c <= '9')
			{
				if (is_num)
				{
					cur.n *= 10;
				}
				else
				{
					is_num = true;
					cur = { 0, set<pos>() };
				}
				cur.n += c - '0';
				cur.g += find_g(input, pos({ x, y }));
			}
			else
			{
				if (!cur.g.empty())
					nums.push_back(cur);
				cur = { 0, set<pos>() };
				is_num = false;
			}
		}
		if (!cur.g.empty())
			nums.push_back(cur);
	}

	map<pos, vector<int>> gears;
	for (auto& n : nums)
	{
		for (auto& g : n.g)
		{
			auto& v = gears[g];
			v.push_back(n.n);
		}
	}
	return accumulate(gears.begin(), gears.end(), (long long)0, [](auto sum, auto& g) 
		{
			return sum + (g.second.size() != 2 ? 0 : (long long)(g.second[0]) * g.second[1]); 
		});
}

void test();
int main()
{
	test();
	ifstream is("Day03.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day3 Answer1: " << solve1(input) << endl;
	cout << "Day3 Answer2: " << solve2(input) << endl;
}
vector<string> read_strings(istream& is)
{
	istream_iterator<string> start(is), end;
	return vector<string>(start, end);
}
void test()
{
	auto test1 = R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..)";

	auto is = stringstream(test1);
	vector<string> input = read_strings(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test1: " << solve2(input) << endl;
}
