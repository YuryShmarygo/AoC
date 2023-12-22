#include "../common.h"

struct point
{
	int x;
	int y;
	int z;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y, p1.z + p2.z }; }
point& operator+=(point& p1, const point& p2) { p1.x += p2.x; p1.y += p2.y; p1.z += p2.z; return p1; }
//typedef vector<point> brick;
struct brick
{
	int id;
	vector<point> points;
	static int next_id() { static int last_id = 0; return ++last_id; }
};
brick& operator+=(brick& b, const point& p) { for (auto& pb : b.points) pb += p; return b; }
typedef vector<vector<string>> field_t;
typedef vector<brick> brick_v;
brick read_brick(string line)
{
	point p1, p2;
	smatch m;
	if (regex_match(line, m, regex(R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))")))
	{
		p1.x = stoi(m[1]);
		p1.y = stoi(m[2]);
		p1.z = stoi(m[3]);
		p2.x = stoi(m[4]);
		p2.y = stoi(m[5]);
		p2.z = stoi(m[6]);
	}
	brick b;
	if (p2.z < p1.z) swap(p1, p2);
	point dir{ sgn(p2.x - p1.x), sgn(p2.y - p1.y), sgn(p2.z - p1.z) };
	for (point p = p1; p != p2; p += dir)
		b.points.push_back(p);
	b.points.push_back(p2);
	b.id = brick::next_id();
	return b;
}
istream& operator>>(istream& is, brick_v& b)
{
	b.clear();
	string line;
	while (getline(is, line) && !line.empty())
		b.push_back(read_brick(line));
	return is;
}
brick get_min_max(const brick_v& bricks)
{
	brick res({ 0, {bricks[0].points[0], bricks[0].points[0]} });
	for (auto& b : bricks)
	{
		for (auto& p : b.points)
		{
			res.points[0].x = min(res.points[0].x, p.x);
			res.points[0].y = min(res.points[0].y, p.y);
			res.points[0].z = min(res.points[0].z, p.z);
			res.points[1].x = max(res.points[1].x, p.x);
			res.points[1].y = max(res.points[1].y, p.y);
			res.points[1].z = max(res.points[1].z, p.z);
		}
	}
	return res;
}
typedef vector<vector<string>> field_t;
typedef vector<brick> brick_v;
field_t plot(const brick_v& bricks, const brick& mx)
{
	field_t field(mx.points[1].z + 1, vector<string>(mx.points[1].y + 1, string(mx.points[1].x + 1, '.')));
	field[0] = vector<string>(mx.points[1].y + 1, string(mx.points[1].x + 1, 'b'));
	for (auto& b : bricks)
		for (auto& p : b.points)
			field[p.z][p.y][p.x] = 'b';
	return field;
}
bool fall(field_t& field, brick& b)
{
	for (auto& p : b.points) field[p.z][p.y][p.x] = '.';

	int dz = field.size();
	for (auto& p : b.points)
	{
		int z = p.z - 1;
		for (; z > 0 && field[z][p.y][p.x] == '.'; --z);
		dz = min(dz, p.z - z - 1);
	}

	for (auto& p : b.points)
	{
		field[p.z - dz][p.y][p.x] = 'b';
		p.z -= dz;
	}
	return dz > 0;
}
int fall(field_t& field, brick_v& bricks)
{
	set<int> all_moved;
	int moved = 0;
	do
	{
		r::sort(bricks, [](auto& b1, auto& b2) {return min(b1.points[0].z, b1.points[b1.points.size() - 1].z) < min(b2.points[0].z, b2.points[b2.points.size() - 1].z); });
		moved = 0;
		for (auto& b : bricks)
			if (fall(field, b))
				all_moved.insert(b.id), ++moved;
	} while (moved > 0);
	return all_moved.size();
}
int can_fall(field_t field, brick_v bricks, int i)
{
	for (auto& p : bricks[i].points) field[p.z][p.y][p.x] = '.';
	bricks.erase(bricks.begin() + i);
	return fall(field, bricks);
}
tuple<field_t, brick_v, brick> init(const brick_v& input)
{
	brick_v bricks(input);
	brick mx = get_min_max(bricks);
	point shift{ -mx.points[0].x, -mx.points[0].y, 1 - mx.points[0].z };
	mx += shift;
	for (auto& b : bricks)
		b += shift;
	auto field = plot(bricks, mx);
	fall(field, bricks);
	return { field, bricks, mx };
}
long long solve1(const brick_v& input)
{
	auto [field, bricks, mx] = init(input);
	long long res = 0;
	for (int i = 0; i < bricks.size(); ++i)
		if (can_fall(field, bricks, i) == 0)
			++res;
	return res;
}
long long solve2(const brick_v& input)
{
	auto [field, bricks, mx] = init(input);
	long long res = 0;
	for (int i = 0; i < bricks.size(); ++i)
		res += can_fall(field, bricks, i);
	return res;
}
int main()
{
	test();
	ifstream is("Day22.txt");
	brick_v input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day22 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day22 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(1,0,1~1,2,1
0,0,2~2,0,2
0,2,3~2,2,3
0,0,4~0,2,4
2,0,5~2,2,5
0,1,6~2,1,6
1,1,8~1,1,9)";

	auto is = stringstream(test1);
	brick_v input; is >> input;

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
