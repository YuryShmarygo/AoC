#include "../common.h"

struct cube
{
	int x;
	int y;
	int z;
	bool operator==(const cube&) const = default;
	auto operator<=>(const cube&) const = default;
	bool operator<(const cube&) const = default;
};
istream& operator>>(istream& is, cube& c) { char ch; return is >> c.x >> ch >> c.y >> ch >> c.z; }
bool adjacent(const cube& c1, const cube& c2)
{
	return abs(c1.x - c2.x) == 1 && c1.y == c2.y && c1.z == c2.z
		|| abs(c1.y - c2.y) == 1 && c1.x == c2.x && c1.z == c2.z
		|| abs(c1.z - c2.z) == 1 && c1.x == c2.x && c1.y == c2.y;
}
int solve1(const vector<cube>& input)
{
	int sides = static_cast<int>(input.size()) * 6;
	for (size_t i = 0; i < input.size(); ++i)
		for (size_t j = i + 1; j < input.size(); ++j)
			sides -= adjacent(input[i], input[j]) ? 2 : 0;

	return sides;
}
bool check(const set<cube>& all, const cube& min_c, const cube& max_c, set<cube>& checked, const cube& c)
{
	if (checked.contains(c))
		return true;
	checked.insert(c);
	return !all.contains(c)
		&& (all.contains({ c.x + 1, c.y, c.z }) || c.x + 1 < max_c.x && check(all, min_c, max_c, checked, { c.x + 1,c.y,c.z }))
		&& (all.contains({ c.x - 1, c.y, c.z }) || c.x - 1 > min_c.x && check(all, min_c, max_c, checked, { c.x - 1,c.y,c.z }))
		&& (all.contains({ c.x, c.y + 1, c.z }) || c.y + 1 < max_c.y && check(all, min_c, max_c, checked, { c.x,c.y + 1,c.z }))
		&& (all.contains({ c.x, c.y - 1, c.z }) || c.y - 1 > min_c.y && check(all, min_c, max_c, checked, { c.x,c.y - 1,c.z }))
		&& (all.contains({ c.x, c.y, c.z + 1 }) || c.z + 1 < max_c.z && check(all, min_c, max_c, checked, { c.x,c.y,c.z + 1 }))
		&& (all.contains({ c.x, c.y, c.z - 1 }) || c.z - 1 > min_c.z && check(all, min_c, max_c, checked, { c.x,c.y,c.z - 1 }));
}
int solve2(const vector<cube>& input)
{
	set<cube> all;
	for (auto& c : input) all.insert(c);

	const auto [minx, maxx] = r::minmax_element(input, [](auto& c1, auto& c2) {return c1.x < c2.x; });
	const auto [miny, maxy] = r::minmax_element(input, [](auto& c1, auto& c2) {return c1.y < c2.y; });
	const auto [minz, maxz] = r::minmax_element(input, [](auto& c1, auto& c2) {return c1.z < c2.z; });

	set<cube> inner;
	for (int x = minx->x; x <= maxx->x; ++x)
		for (int y = miny->y; y <= maxy->y; ++y)
			for (int z = minz->z; z <= maxz->z; ++z)
			{
				cube c{ x,y,z };
				if (!all.contains(c) && !inner.contains(c))
				{
					set<cube> checked;
					if (check(all, { minx->x, miny->y, minz->z }, { maxx->x, maxy->y, maxz->z }, checked, c))
						inner.insert(checked.begin(), checked.end());
				}
			}
	return solve1(input) - solve1(vector<cube>(inner.begin(), inner.end()));
}

int main()
{
	test();
	ifstream is("Day18.txt");
	istream_iterator<cube> start(is), end;
	vector<cube> input(start, end);

	cout << "Day18 answer1: " << solve1(input) << endl;
	cout << "Day18 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5)");
	istream_iterator<cube> start(is), end;
	vector<cube> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
