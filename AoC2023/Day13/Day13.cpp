#include "../common.h"

typedef vector<string> pattern;
typedef vector<pattern> v_pattern;
istream& operator>>(istream& is, v_pattern& vp)
{
	vp.clear();
	pattern c;
	string line;
	while (getline(is, line))
	{
		if (line.empty())
		{
			vp.push_back(c);
			c.clear();
		}
		else
		{
			c.push_back(line);
		}
	}
	vp.push_back(c);
	return is;
}
long long process_h(const pattern& p)
{
	for (int y = 0; y < p.size() - 1; ++y)
	{
		if (p[y] == p[y + 1])
		{
			bool matched = true;
			for (int i = y - 1, j = y + 2; i >= 0 && j < p.size() && matched; --i, ++j)
				if (p[i] != p[j])
					matched = false;
			if (matched)
			{
				//cout << "h : " << y << " p.size() = " << p.size() << endl; //r::copy(p, ostream_iterator<string>(cout, "\n"));
				return (y + 1) * 100;
			}
		}
	}
	return 0;
}
long long process_v(const pattern& p)
{
	for (int x = 0; x < p[0].size() - 1; ++x)
	{
		bool matched = true;
		for (int y = 0; y < p.size() && matched; ++y)
		{
			if (p[y][x] != p[y][x + 1])
				matched = false;
		}
		if (matched)
		{
			for (int i = x - 1, j = x + 2; i >= 0 && j < p[0].size() && matched; --i, ++j)
				for (int y = 0; y < p.size() && matched; ++y)
					if (p[y][i] != p[y][j])
						matched = false;
			if (matched)
			{
				//cout << "v : " << x << " p[0].size() = " << p[0].size() << endl;// r::copy(p, ostream_iterator<string>(cout, "\n"));
				return x + 1;
			}
		}
	}
	return 0;
}
long long process(const pattern& p)
{
	//r::copy(p, ostream_iterator<string>(cout, "\n"));
	return process_h(p) + process_v(p);
}
long long solve1(const v_pattern& input)
{
	long long res = 0;
	for (auto& p : input)
		res += process(p);
	return res;
}
pattern transpose(const pattern& o)
{
	pattern res(o[0].size(), string(o.size(), ' '));
	for (int y = 0; y < o.size(); ++y)
		for (int x = 0; x < o[y].size(); ++x)
			res[x][y] = o[y][x];
	return res;
}

struct match
{
	int d;
	pair<int,int> r;
	bool operator==(const match&) const = default;
	auto operator<=>(const match&) const = default;
	bool operator<(const match&) const = default;
};

vector<match> find_matches(const pattern& p)
{
	vector<match> m;
	for (int i = 0; i < p.size(); ++i)
	{
		for (int j = i + 1; j < p.size(); ++j)
		{
			int matches = 0;
			for (auto x = 0; x < p[i].size(); ++x)
				if (p[i][x] == p[j][x])
					++matches;
			m.push_back({ (int)p[i].size() - matches, {i, j} });
		}
	}
	r::sort(m, [](auto& m1, auto& m2) { return m1.d < m2.d; });
	return m;
}

int find_sym(const vector<match>& e, const pattern& p, int prev = -1)
{
	for (auto& cur : e)
	{
		if (cur.r.second - cur.r.first == 1)
		{
			bool matched = true;
			for (int i = cur.r.first - 1, j = cur.r.second + 1; i >= 0 && j < p.size() && matched; --i, ++j)
				if (e.end() == r::find_if(e, [i, j](auto& r) { return r.r.first == i && r.r.second == j; }))
					matched = false;
			if (matched && cur.r.first != prev)
				return cur.r.first;
		}
	}
	return -1;
}
int find_sym(const pattern& p)
{
	vector<match> m = find_matches(p);
	vector<match> e;
	for (auto it = m.begin(); it != m.end() && it->d == 0; ++it)
		e.push_back(*it);
	return find_sym(e, p);
}
int find_sym1(const pattern& p)
{
	vector<match> m = find_matches(p);
	vector<match> e;
	for (auto it = m.begin(); it != m.end(); ++it)
		if (it->d == 0)
			e.push_back(*it);
	vector<match> e1;
	for (auto it = m.begin(); it != m.end(); ++it)
		if (it->d == 1)
			e1.push_back(*it);

	if (e1.empty())
		return -1;

	int prev = find_sym(e, p);
	for (auto& e_1 : e1)
	{
		vector<match> e_n(e);
		e_n.push_back(e_1);
		int res = find_sym(e_n, p, prev);
		if (res != -1)
			return res;
	}
	return -1;
}
long long solve1_new(const v_pattern& input)
{
	long long res = 0;
	for (auto& p : input)
		res += (find_sym(p) + 1) * 100 + find_sym(transpose(p)) + 1;
	return res;
}

long long solve2(const v_pattern& input)
{
	long long res = 0;
	for (auto& p : input)
		res += (find_sym1(p) + 1) * 100 + find_sym1(transpose(p)) + 1;
	return res;
}
int main()
{
	test();
	ifstream is("Day13.txt");
	v_pattern input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day13 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	cout << "Day13 Answer1_new: " << solve1_new(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	//2202 30568
	t_start = high_resolution_clock::now();
	cout << "Day13 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)";

	auto is = stringstream(test1);
	v_pattern input; is >> input;

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve1_new(input) << endl;
	cout << "Test3: " << solve2(input) << endl;

	auto test2 = R"(#.###..#..###
.#...##.####.
.#...##.####.
#.###..#..###
.#######.##.#
.#..##.#.#..#
..#..#.##.#..
##..##..###.#
######.##..#.
######.##....
##..##..###.#
..#..#.##.#..
.#..##.#.#..#)";
	is = stringstream(test2);
	is >> input;
	cout << "Test4: " << solve2(input) << endl;
	//cout << "Test2: " << solve2(input, 100) << endl;
}
