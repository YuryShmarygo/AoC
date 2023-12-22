#include "../common.h"

struct instr_t
{
	char dir;
	int n;
	string code;
};
istream& operator>>(istream& is, instr_t& i)
{
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex(R"((.) (\d+) \(#(......)\))")))
	{
		i.dir = m[1].str()[0];
		i.n = stoi(m[2].str());
		i.code = m[3].str();
	}
	return is;
}
vector<instr_t> read_instr(istream& is)
{
	istream_iterator<instr_t> start(is), end;
	return vector<instr_t>(start, end);
}
struct point
{
	long long x;
	long long y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y }; }
map<char, point> all_dirrections = {
	{'U', { 0,-1}},
	{'D', { 0, 1}},
	{'L', {-1, 0}},
	{'R', { 1, 0}}
};
vector<point> dig(const vector<instr_t>& input)
{
	vector<point> res(1, { 0, 0 });
	point cur{ 0, 0 };
	for (auto& instr : input)
	{
		auto d = all_dirrections[instr.dir];
		for (int i = 0; i < instr.n; ++i)
		{ 
			cur = cur + d;
			res.push_back(cur);
		}
	}
	return res;
}
vector<point> norm(const vector<point>& tr)
{
	auto min_x = r::min_element(tr, [](auto& p1, auto& p2) {return p1.x < p2.x; })->x;
	auto min_y = r::min_element(tr, [](auto& p1, auto& p2) {return p1.y < p2.y; })->y;
	point shift{ -min_x, -min_y };
	vector<point> res;
	for (auto& p : tr)
		res.push_back(p + shift);
	return res;
}
point min_max(const vector<point>& tr)
{
	auto max_x = r::max_element(tr, [](auto& p1, auto& p2) {return p1.x < p2.x; })->x;
	auto max_y = r::max_element(tr, [](auto& p1, auto& p2) {return p1.y < p2.y; })->y;
	return { max_x, max_y };
}
vector<string> plot(const vector<point>& tr)
{
	auto dim = min_max(tr);
	vector<string> res(dim.y + 1, string(dim.x + 1, '.'));
	for (auto& p : tr)
		res[p.y][p.x] = '#';
	return res;
}
ostream& operator<<(ostream& os, vector<string>& field) { r::copy(field, ostream_iterator<string>(os, "\n")); return os; }
long long solve1(const vector<instr_t>& input)
{
	auto tr = norm(dig(input));
	auto dim = min_max(tr) + point({ 1, 1});
	auto field = plot(tr);
	//cout << endl << field << endl;

	vector<point> all_dir{ {-1,0},{0,-1},{1,0},{0,1} };
	set<point> cur;
	for (int y = 0; y < dim.y; ++y)
	{
		if (field[y][0] == '.')
			cur.insert({ 0, y });
		if (field[y][dim.x-1] == '.')
			cur.insert({ dim.x - 1, y });
	}
	for (int x = 0; x < dim.x; ++x)
	{
		if (field[0][x] == '.')
			cur.insert({ x, 0 });
		if (field[dim.y - 1][x] == '.')
			cur.insert({ x, dim.y - 1 });
	}
	while (!cur.empty())
	{
		set<point> next;
		for (auto& p : cur)
			field[p.y][p.x] = 'O';
		for (auto& p : cur)
		{
			for (auto& d : all_dir)
			{
				point np = p + d;
				if (np.x >= 0 && np.x < field[0].size() && np.y >= 0 && np.y < field.size() && field[np.y][np.x] == '.')
					next.insert(np);
			}
		}
		swap(cur, next);
	}
	//cout << endl << field << endl;

	long long res = 0;
	for (int y = 0; y < dim.y; ++y)
		for (int x = 0; x < dim.x; ++x)
			if (field[y][x] == '.' || field[y][x] == '#')
				++res;
	return res;
}
instr_t convert(const instr_t& instr)
{
	instr_t res;
	stringstream s(instr.code.substr(0, 5));
	s >> hex >> res.n;
	int c;
	s = stringstream(instr.code.substr(5));
	s >> hex >> c;
	map<int, char> n2dir = {
		{0, 'R'},
		{1, 'D'},
		{2, 'L'},
		{3, 'U'}
	};
	res.dir = n2dir[c];
	return res;
}
vector<instr_t> convert(const vector<instr_t>& input)
{
	vector<instr_t> res;
	for (auto& i : input)
		res.push_back(convert(i));
	return res;
}
long long find_area(const vector<point>& tr)
{
	long long res = 0;
	long long p = 0;
	for (auto it = tr.begin(); it != tr.end(); ++it)
	{
		auto itn = next(it) == tr.end() ? tr.begin() : next(it);
		res += (it->y + itn->y) * (it->x - itn->x) / 2;
		p += abs(it->y - itn->y) + abs(it->x - itn->x) - 1;
	}
	return abs(res) + p;
}
long long solve2(const vector<instr_t>& input)
{
	auto tr = norm(dig(convert(input)));
	return find_area(tr);
}
int main()
{
	test();
	ifstream is("Day18.txt");
	auto input = read_instr(is);

	auto t_start = high_resolution_clock::now();
	cout << "Day18 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day18 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(R 6 (#70c710)
D 5 (#0dc571)
L 2 (#5713f0)
D 2 (#d2c081)
R 2 (#59c680)
D 2 (#411b91)
L 5 (#8ceee2)
U 2 (#caa173)
L 1 (#1b58a2)
U 2 (#caa171)
R 2 (#7807d2)
U 3 (#a77fa3)
L 2 (#015232)
U 2 (#7a21e3))";

	auto is = stringstream(test1);
	auto input = read_instr(is);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
	cout << "Test3: " << find_area(norm(dig(input))) << endl;
}
