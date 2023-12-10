#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y }; }
map<char, vector<point>> directions = { 
	{'|',{{0,-1},{0,1}}},
	{'-',{{-1,0},{1,0}}},
	{'L',{{0,-1},{1,0}}},
	{'J',{{0,-1},{-1,0}}},
	{'7',{{-1,0},{0,1}}},
	{'F',{{0,1},{1,0}}} };
struct p_point
{
	char c;
	point p;
};
point find_s(const vector<string>& input)
{
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == 'S')
				return point{ x,y };
	cout << "warning: S not found" << endl;
	return point{ 0,0 };
}
p_point find_adj(const vector<string>& input, const point& start)
{
	for (int y = start.y - 1; y <= start.y + 1; y = y + 2)
	{
		if (y >= 0 && y < input.size() && input[y][start.x] != '.')
		{
			char c = input[y][start.x];
			point p{ start.x, y };
			auto& dd = directions[c];
			for (auto& d : dd)
				if (p + d == start)
					return { c, p };
		}
	}
	for (int x = start.x - 1; x <= start.x + 1; x = x + 2)
	{
		if (x >= 0 && x != start.x && x < input[start.y].size() && input[start.y][x] != '.')
		{
			char c = input[start.y][x];
			point p{ x, start.y };
			auto& dd = directions[c];
			for (auto& d : dd)
				if (p + d == start)
					return { c,p };
		}
	}
	cout << "warning: adj not found" << endl;
	return { '.',{ 0,0} };
}
vector<point> build_path(const vector<string>& input)
{
	vector<point> path;
	point start = find_s(input);
	path.push_back(start);

	p_point cur = find_adj(input, start);
	while (cur.c != 'S')
	{
		path.push_back(cur.p);
		auto&& d = directions[cur.c];
		point next = cur.p + d[0];
		if (path[path.size() - 2] == next)
			next = cur.p + d[1];
		cur = { input[next.y][next.x], next };
	}
	return path;
}
int solve1(const vector<string>& input)
{
	vector<point> path = build_path(input);
	return (path.size()+1) / 2;
}
point inflate(const point& p) { return point{ p.x * 3 + 1, p.y * 3 + 1 }; }
vector<point> inflate(const vector<point>& o)
{
	vector<point> res; res.reserve(o.size() * 3);
	res.push_back(inflate(o[0]));
	for (int i = 0; i < o.size(); ++i)
	{ 
		point start = inflate(o[i]);
		point end = inflate(o[i+1 < o.size() ? i + 1 : 0]);
		for (int y = min(start.y, end.y); y <= max(start.y, end.y); ++y)
		{
			for (int x = min(start.x, end.x); x <= max(start.x, end.x); ++x)
			{
				point cur{ x, y };
				if (res.back() != cur)
					res.push_back(cur);
			}
		}
	}
	return res;
}

int solve2(const vector<string>& input_o)
{
	vector<string> input(input_o.size() + 2, string(input_o[0].size() + 2, '.'));
	for (int y = 0; y < input_o.size(); ++y)
		copy(input_o[y].begin(), input_o[y].end(), next(input[y + 1].begin()));
	//cout << endl;  r::copy(input, ostream_iterator<string>(cout, "\n"));
	vector<point> path = inflate(build_path(input));

	vector<string> field{ input.size() * 3, string(input[0].size() * 3, '.')};
	for (auto& p : path)
		field[p.y][p.x] = 'P';

	vector<point> all_dir{ {-1,0},{0,-1},{1,0},{0,1} };
	set<point> cur;
	for (int y = 0; y < field.size(); ++y)
	{
		cur.insert({ 0, y });
		cur.insert({ (int)field[0].size() - 1, y });
	}
	for (int x = 0; x < field[0].size(); ++x)
	{
		cur.insert({ x, 0 });
		cur.insert({ x, (int)field.size() - 1 });
	}
	int visited = path.size();
	int all = field.size() * field[0].size();
	while (!cur.empty())
	{
		visited += cur.size();
		set<point> next;
		for (auto& p : cur)
			field[p.y][p.x] = 'O';
		for (auto& p : cur)
		{
			for (auto& d : all_dir)
			{
				point np = p + d;
				if (np.x >=0 && np.x < field[0].size() && np.y >= 0 && np.y < field.size() && field[np.y][np.x] == '.')
					next.insert(np);
			}
		}
		swap(cur, next);
	}
	//cout << endl;  r::copy(field, ostream_iterator<string>(cout, "\n"));
	int res = 0;
	for (int y = 1; y < field.size(); y += 3)
		for (int x = 1; x < field[y].size(); x += 3)
			if (field[y][x] == '.')
				++res;
	return res;
}
int main()
{
	test();
	ifstream is("Day10.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day10 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day10 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(.....
.S-7.
.|.|.
.L-J.
.....))";
	auto test2 = R"(-L|F7
7S-7|
L|7||
-L-J|
L|-JF)";
	auto test3 = R"(7-F7-
.FJ|7
SJLL7
|F--J
LJ.LJ)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "Test1: " << solve1(input) << endl;

	is = stringstream(test2);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test2: " << solve1(input) << endl;

	is = stringstream(test3);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test3: " << solve1(input) << endl;

	auto test4 = R"(...........
.S-------7.
.|F-----7|.
.||.....||.
.||.....||.
.|L-7.F-J|.
.|..|.|..|.
.L--J.L--J.
...........)";
	auto test5 = R"(..........
.S------7.
.|F----7|.
.||....||.
.||....||.
.|L-7F-J|.
.|..||..|.
.L--JL--J.
..........)";
	auto test6 = R"(.F----7F7F7F7F-7....
.|F--7||||||||FJ....
.||.FJ||||||||L7....
FJL7L7LJLJ||LJ.L-7..
L--J.L7...LJS7F-7L7.
....F-J..F7FJ|L7L7L7
....L7.F7||L7|.L7L7|
.....|FJLJ|FJ|F7|.LJ
....FJL-7.||.||||...
....L---J.LJ.LJLJ...)";
	auto test7 = R"(FF7FSF7F7F7F7F7F---7
L|LJ||||||||||||F--J
FL-7LJLJ||||||LJL-77
F--JF--7||LJLJ7F7FJ-
L---JF-JLJ.||-FJLJJ7
|F|F-JF---7F7-L7L|7|
|FFJF7L7F-JF7|JL---7
7-L-JL7||F7|L7F-7F7|
L.L7LFJ|||||FJL7||LJ
L7JLJL-JLJLJL--JLJ.L)";

	is = stringstream(test4);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test4: " << solve2(input) << endl;
	is = stringstream(test5);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test5: " << solve2(input) << endl;
	is = stringstream(test6);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test6: " << solve2(input) << endl;
	is = stringstream(test7);
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test7: " << solve2(input) << endl;
}
