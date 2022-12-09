#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
	void move(char d)
	{
		if (d == 'U')
			y -= 1;
		else if (d == 'D')
			y += 1;
		else if (d == 'R')
			x += 1;
		else if (d == 'L')
			x -= 1;
	}
	void move_to(const point& h)
	{
		point s{ 0,0 };
		point d = point{ h.x - x, h.y - y };
		if (d.x > 1 || d.x > 0 && abs(d.y) > 1)
			s.x = 1;
		if (d.x < -1 || d.x < 0 && abs(d.y) > 1)
			s.x = -1;
		if (d.y > 1 || d.y > 0 && abs(d.x) > 1)
			s.y = 1;
		if (d.y < -1 || d.y < 0 && abs(d.x) > 1)
			s.y = -1;
		x += s.x; y += s.y;
	}
};
point dist(const point& p1, const point& p2)
{ return point{p2.x - p1.x, p2.y - p1.y}; }
struct movet
{
	char dir;
	int num;
};
istream& operator>>(istream& is, movet& m)
{ return is >> m.dir >> m.num; }

struct rope
{
	vector<point> knots;
	rope(int n) : knots(n, point{0,0}){}
	set<point> visited;
	void move(char dir)
	{
		knots[0].move(dir);
		for (int i = 1; i < knots.size(); ++i)
			knots[i].move_to(knots[i - 1]);
		visited.insert(knots.back());
	}
	void move(const movet& m)
	{
		for (int i = 0; i < m.num; ++i)
			move(m.dir);
	}
	void move(const vector<movet>& moves)
	{
		for (auto m : moves)
			move(m);
	}
};

int main()
{
	test();
	ifstream is("Day9.txt");
	istream_iterator<movet> start(is), end;
	vector<movet> input(start, end);
	rope r(2);
	r.move(input);
	rope r10(10);
	r10.move(input);
	cout << "Day9 answer1: " << r.visited.size() << endl;
	cout << "Day9 answer2: " << r10.visited.size() << endl;
}
void test()
{
	stringstream is(R"(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2)");
	istream_iterator<movet> start(is), end;
	vector<movet> input(start, end);
	rope r(2);
	r.move(input);
	cout << "test1: " << r.visited.size() << endl;

	stringstream is2(R"(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20)");
	istream_iterator<movet> start2(is2), end2;
	vector<movet> input2(start2, end2);
	rope r2(10);
	r2.move(input2);
	cout << "test2: " << r2.visited.size() << endl;
}
