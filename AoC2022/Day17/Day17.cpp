#include "../common.h"


struct point
{
	point() : x(0), y(0) {}
	point(int _x, int _y) : x(_x), y(_y) {}
	point(const point&) = default;
	point& operator=(const point&) = default;
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
ostream& operator<<(ostream& os, const point& p) { return os << p.x << "," << p.y; }
vector<vector<string>> patterns{
{"####"},{".#.","###",".#."},{"..#","..#","###"},{"#","#","#","#"},{"##","##"} };

struct shape
{
	point pos;
	vector<string>& s;
	explicit shape(int i) : pos(2, 3), s(patterns[i]) {}
	shape(const point& p, vector<string>& s) : pos(p), s(s){}
};

ostream& operator<<(ostream& os, const list<string>& v) { copy(v.rbegin(), v.rend(), ostream_iterator<string>(os, "\n")); return os; }
ostream& to_stream(ostream& os, const list<string>& c, list<string>::reverse_iterator itc, const shape& s)
{
	//auto& [c, itc, s] = o;

	for (auto it = c.rbegin(); it != itc; ++it) os << *it << endl;
	auto its = s.s.begin();
	for (; its != s.s.end() && itc !=c.rend(); ++itc, ++its)
	{
		auto itc_c = itc->begin();
		auto its_c = its->begin();
		for (int x = 0; x < s.pos.x; ++x)
			os << *itc_c++;
		for (; itc_c != itc->end() && its_c != its->end(); ++itc_c, ++its_c)
			os << (*its_c == '#' ? '@' : *itc_c);
		for (; itc_c != itc->end(); ++itc_c)
			os << *itc_c;
		os << endl;
	}
	for (auto it = itc; it != c.rend(); ++it) os << *it << endl;
	return os;
}
bool can_fit(list<string>& c, list<string>::reverse_iterator itc, const shape& s)
{
	if (s.pos.x < 0 || s.pos.x + static_cast<int>(s.s[0].size()) > 7) return false;
	auto its = s.s.begin();
	for (; its != s.s.end() && itc != c.rend(); ++its, ++itc)
	{
		auto its_c = its->begin();
		auto itc_c = itc->begin() + s.pos.x;
		for (; its_c != its->end() && itc_c != itc->end(); ++its_c, ++itc_c)
			if (*its_c == '#' && *itc_c == '#')
				return false;
	}
	return true;
}
bool fall(list<string>& c, shape& s, char w)
{
	auto itc = c.rbegin();
	for (int i = 0; i < 3 - s.pos.y; ++i) ++itc;
//	cout << "fall wind: " << w << " s.pos: " << s.pos << endl; to_stream(cout, c, itc, s) << endl;
	int dx = (w == '<' ? -1 : 1);
	if (can_fit(c, itc, { point(s.pos.x + dx,s.pos.y), s.s}))
		s.pos.x += dx;

	if (can_fit(c, next(itc), { point(s.pos.x, s.pos.y - 1), s.s }))
	{
		++itc;
		--s.pos.y;
//		cout << "fall wind: " << w << endl;  to_stream(cout, c, itc, s) << endl;
		return true;
	}

	for (auto its = s.s.begin(); its != s.s.end(); ++its, ++itc)
	{
		auto its_c = its->begin();
		auto itc_c = itc->begin() + s.pos.x;
		for (; its_c != its->end() && itc_c != itc->end(); ++its_c, ++itc_c)
			if (*its_c == '#')
				*itc_c = '#';
	}

//	cout << "fall wind: " << w << endl << c << endl;
	return false;
}

int solve1(const string& wind, int steps)
{
	list<string> c{"#######"};
	int cur_w = 0;
	int height = 0;
	for (int i = 0; i < steps; ++i)
	{
//		cout << "i: " << i << endl << c << endl;
		int cur_s = i % 5;
		shape s(cur_s);
		for (int i = 0; i < 3 + static_cast<int>(s.s.size()); ++i)
			c.emplace_back(".......");
		while (fall(c, s, wind[cur_w]))
			cur_w = (cur_w + 1) % static_cast<int>(wind.size());
		while (c.back() == ".......") c.pop_back();
//		cout << "i : " << i << endl << c << endl;
		cur_w = (cur_w + 1) % static_cast<int>(wind.size());

		auto itd = c.rend();
		int dy = 0;
		for (auto it = next(c.rbegin()); it != c.rend() && itd == c.rend(); ++it, ++dy)
		{
			if (*it == c.back())
				itd = it;
		}
		++dy;
		cout << "found duplicates: " << dy << endl;

		for (auto it = c.rbegin(); dy > 0 && itd != c.rend() && *it == *itd; --dy, ++it, ++itd) {}
		cout << "all compare: " << dy << endl;

	}
//	cout << c << endl;
	return static_cast<int>(c.size()) - 1;
}

int main()
{
	test();// return 0;
	ifstream is("Day17.txt");
	string w;
	is >> w;

	cout << "Day17 answer1: " << solve1(w, 2022) << endl;
	cout << "Day17 answer2: " << 1 << endl;
}
void test()
{
	stringstream is(R"(>>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>)");
	string w;
	is >> w;
	cout << "test1: " << solve1(w,12) << endl;
	cout << "test2: " << solve1(w, 2022) << endl;
	cout << "test3: " << 1 << endl;
}
