#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{
	return read_strings(is);
}

vector<point> find_heads(const input_t& input)
{
	vector<point> res;
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == '0')
				res.push_back({ x, y });
	return res;
}

list<vector<point>> build_trais(const input_t& input)
{
	vector<point> all_dir = { {1,0},{0,1},{-1,0},{0,-1} };
	auto heads = find_heads(input);
	list<vector<point>> paths;
	for (auto h : heads)
		paths.push_back(vector<point>(1, h));
	for (auto itp = paths.begin(); itp != paths.end() && !r::all_of(paths, [&](auto& p) {return input[p.back().y][p.back().x] == '9'; });)
	{
		auto p = *itp;
		if (input[p.back().y][p.back().x] == '9')
		{
			++itp;
			continue;
		}
		itp = paths.erase(itp);
		vector<point> moves;
		for (auto d : all_dir)
		{
			point cur = p.back();
			point next = cur + d;
			if (next.y >= 0 && next.y < input.size() && next.x >= 0 && next.x < input[0].size() && input[next.y][next.x] - input[cur.y][cur.x] == 1)
			{
				vector<point> path(p);
				path.push_back(next);
				paths.push_back(path);
			}
		}
		if (itp == paths.end())
			itp = paths.begin();
	}
	return paths;
}
long long solve1(const input_t& input)
{
	auto paths = build_trais(input);
	map<point, set<point>> scores;
	for (auto p : paths)
	{
		auto head = p.front();
		auto end = p.back();
		scores[head].insert(end);
	}
	int res = 0;
	for (auto s : scores)
		res += (int)s.second.size();
	return res;
}
long long solve2(const input_t& input)
{
	auto paths = build_trais(input);
	return paths.size();
}

int main()
{
	test();
	ifstream is("Day10.txt");
	input_t input = read_input(is);

	cout << "Day10 Answer1: " << solve1(input) << endl;
	cout << "Day10 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;

	auto test2 = R"(0123
1234
8765
9876)";
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test3: " << solve1(input) << endl;
}
