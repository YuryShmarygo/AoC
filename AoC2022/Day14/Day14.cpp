#include "../common.h"

struct point
{
	point() : x(0), y(0){}
	point(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};
point operator+(const point& p1, const point& p2)
{ return {p1.x + p2.x, p1.y + p2.y};}
istream& operator>>(istream& is, point& p)
{
	char c;
	is >> p.x >> c >> p.y;
	return is;
}
ostream& operator<<(ostream& os, const point& p)
{ return os << p.x << "," << p.y; }
istream& operator>>(istream& is, vector<point>& v)
{
	v.clear();
	string line;
	getline(is, line);
	stringstream st(line);
	while(st)
	{
		point p;
		st >> p;
		v.push_back(p);
		char c;
		st >> c >> c;
	}
	return is;
}
pair<vector<string>, point> build_map(const vector<vector<point>>& input)
{
	point tl(1000, 0), br(-1,-1);
	for (auto& l : input)
	{
		for (auto& p : l)
		{
			br.x = max(br.x, p.x);
			br.y = max(br.y, p.y);
			tl.x = min(tl.x, p.x);
			tl.y = min(tl.y, p.y);
		}
	}
	vector<string> map(br.y - tl.y + 1, string(br.x - tl.x + 1, ' '));
	for (auto& l : input)
	{
		for (auto itp = l.begin(); itp != l.end(); ++itp)
		{
			if (next(itp) == l.end())
				continue;
			auto p1 = *itp;
			auto p2 = *next(itp);
			
			for (int y = min(p1.y, p2.y); y <= max(p1.y, p2.y); ++y)
				map[y - tl.y][p1.x - tl.x] = '#';
			for (int x = min(p1.x, p2.x); x <= max(p1.x, p2.x); ++x)
				map[p1.y - tl.y][x - tl.x] = '#';
		}
	}
	return { map, tl };
}
bool put_sand(vector<string>& map, point sand)
{
	if (map[sand.y][sand.x] != ' ')
		return false;
	while (sand.y < static_cast<int>(map.size()) && sand.x >= 0 && sand.x < static_cast<int>(map[0].size()))
	{
		if (sand.y + 1 == static_cast<int>(map.size()) || map[sand.y + 1][sand.x] == ' ')
			sand.y += 1;
		else if (sand.x - 1 < 0 || sand.y + 1 == static_cast<int>(map.size()) || map[sand.y + 1][sand.x - 1] == ' ')
			sand = { sand.x - 1, sand.y + 1 };
		else if (sand.x + 1 == static_cast<int>(map[0].size()) || sand.y + 1 == static_cast<int>(map.size()) || map[sand.y+1][sand.x + 1] == ' ')
			sand = { sand.x + 1, sand.y + 1 };
		else
		{
			map[sand.y][sand.x] = 'o';
			return true;
		}
	}
	return false;
}
int fill_sand(vector<string> map, const point& sand)
{
	int res = 0;
	while(put_sand(map, sand))
		++res;
//	r::copy(map, ostream_iterator<string>(cout, "\n"));

	return res;
}

int solve1(const vector<vector<point>>& input)
{
	auto [map, tl] = build_map(input);
//	r::copy(map, ostream_iterator<string>(cout, "\n"));
	return fill_sand(map, { 500 - tl.x, -tl.y });
}

pair<vector<string>, point> add_floor(vector<string> map)
{
	size_t h = map.size() + 2;
	size_t w = (h + 100 - map[0].size() /2) * 2;
	int shift = static_cast<int>(w - map[0].size()) / 2;
	vector<string> res(h, string(w, ' '));
	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
			res[y][x + shift] = map[y][x];
	}
	for (size_t x = 0; x < res[0].size(); ++x)
		res[res.size() - 1][x] = '#';
	return { res, {shift, 0}};
}
int solve2(const vector<vector<point>>& input)
{
	auto [map, tl] = build_map(input);
//	r::copy(map, ostream_iterator<string>(cout, "\n"));
	auto [newmap, shift] = add_floor(map);
//	r::copy(newmap, ostream_iterator<string>(cout, "\n"));

	return fill_sand(newmap, { 500 - tl.x + shift.x, -tl.y });
}

int main()
{
	test();
	ifstream is("Day14.txt");
	istream_iterator<vector<point>> start(is), end;
	vector<vector<point>> input(start, end);

	cout << "Day14 answer1: " << solve1(input) << endl;
	cout << "Day14 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)");
	istream_iterator<vector<point>> start(is), end;
	vector<vector<point>> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
