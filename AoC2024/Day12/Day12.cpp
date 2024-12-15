#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{ return read_strings(is); }
struct region
{
	char plant;
	set<point> pots;
	int perimeter;

	bool operator==(const region&) const = default;
};

vector<point> all_dir = { {1,0}, {0,1}, {-1,0},{0,-1} };

vector<region> build_regions(const input_t& input)
{
	list<region> regions;
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[0].size(); ++x)
		{
			point p = { x , y };
			char plant = input[y][x];
			vector<point> adj;
			for (auto d : all_dir)
			{
				point a = p + d;
				if (a.y >= 0 && a.y < input.size() && a.x >= 0 && a.x < input[0].size())
					adj.push_back(a);
			}
			int perimeter = 4 - (int)adj.size();
			vector<list<region>::iterator> adjr;
			for (auto a : adj)
			{
				if (input[a.y][a.x] != plant)
					++perimeter;
				for (auto itr = regions.begin(); itr != regions.end(); ++itr)
				{
					if (itr->plant == plant && itr->pots.contains(a) && find(adjr.begin(), adjr.end(), itr) == adjr.end())
						adjr.push_back(itr);
				}
			}
			if (adjr.size() == 0)
			{
				regions.push_back({ plant, {p}, perimeter });
			}
			else
			{
				auto& fr = *adjr.front();
				fr.perimeter += perimeter;
				fr.pots.insert(p);
				for (auto it = next(adjr.begin()); it != adjr.end(); ++it)
				{
					auto& r = **it;
					fr.perimeter += r.perimeter;
					fr.pots.insert(r.pots.begin(), r.pots.end());
					regions.erase(*it);
				}
			}
		}
	}
	return vector<region>(regions.begin(), regions.end());
}
long long solve1(const input_t& input)
{
	auto regions = build_regions(input);
	long long sum = 0;
	for (auto r : regions)
		sum += r.perimeter * r.pots.size();
	return sum;
}

long long solve2(const input_t& input)
{
	vector<point> b_dir = { {1,1},{1,-1},{-1,1},{-1,-1} };
	b_dir.insert(b_dir.end(), all_dir.begin(), all_dir.end());
	auto regions = build_regions(input);
	long long total_adj = 0;
	for (auto r : regions)
	{
		list<point> border;
		for (auto p : r.pots)
			if (r::any_of(b_dir, [&](auto d) { return !r.pots.contains(p + d); }))
				border.push_back(p);

		list<point> path;
		auto it = r::find_if(border, [&](auto& p) { return r::count_if(all_dir, [&](auto d) { return !r.pots.contains(p + d); }) > 2; });
		path.push_back(*it);
		border.erase(it);
		while (!border.empty())
		{
			auto it = r::find_if(border, [&](auto& p) { return 
				r::any_of(all_dir, [&](auto d) { return 
					p + d == border.back()
					&& (border.size() == 1 || p + d != *(prev(prev(border.end())))); }); });
			path.push_back(*it);
			border.erase(it);
		}
		int total = 0;
		point d = path.front() - path.back();
		int adj = 0;
		for (auto it = path.begin(); it != path.end(); ++it)
		{
			auto itn = next(it);
			if (itn != path.end())
				break;
			point dir = *itn - *it;
			if (dir == d)
				++adj;
			else
			{
				total += adj + 1;
				adj = 0;
				d = dir;
			}
		}
		total_adj += total;
	}
	long long sum = 0;
	for (auto r : regions)
		sum += r.perimeter * r.pots.size();
	return sum - total_adj;
}

int main()
{
	test();
	ifstream is("Day12.txt");
	input_t input = read_input(is);

	cout << "Day12 Answer1: " << solve1(input) << endl;
	cout << "Day12 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(AAAA
BBCD
BBCC
EEEC)";
	auto test2 = R"(OOOOO
OXOXO
OOOOO
OXOXO
OOOOO)";
	auto test3 = R"(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test1.2 (80): " << solve2(input) << endl;
	is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve1(input) << endl;
	cout << "Test2.2 (436): " << solve2(input) << endl;
	is = stringstream(test3); input = read_input(is);
	cout << "Test3: " << solve1(input) << endl;
	cout << "Test3.2 (1206): " << solve2(input) << endl;

	auto test4 = R"(EEEEE
EXXXX
EEEEE
EXXXX
EEEEE)";
	is = stringstream(test3); input = read_input(is);
	cout << "Test4 (236): " << solve2(input) << endl;
	auto test5 = R"(AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA)";
	is = stringstream(test3); input = read_input(is);
	cout << "Test5 (368): " << solve2(input) << endl;
}
