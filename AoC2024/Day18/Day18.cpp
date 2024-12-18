#include "../common.h"

typedef vector<point> input_t;
istream& operator>>(istream& is, point& p) { char c; return is >> p.x >> c >> p.y; }
input_t read_input(istream& is) 
{
	istream_iterator<point> start(is), end;
	return input_t(start, end);
}
typedef vector<point> path_t;
vector<string> fall(const input_t& input, int size, int nfall)
{
	vector<string> res(size, string(size, '.'));
	for (int i = 0; i < nfall; ++i)
		if (input[i].y < size && input[i].x < size)
			res[input[i].y][input[i].x] = '#';
	return res;
}
vector<point> all_dir = { {-1,0}, {0,-1}, {1,0},{0,1} };
long long find_path(vector<string>& field)
{
	int size = (int)field.size();
	map<point, path_t> best = { {{size - 1,size - 1}, path_t()} };
	list<path_t> paths;
	paths.push_back({ { size - 1,size - 1 } });
	while (!paths.empty())
	{
		path_t path = paths.front();
		paths.erase(paths.begin());
		if (path.back() == point{ 0, 0 })
			continue;

		point p = path.back();
		for (auto& d : all_dir)
		{
			point n = p + d;
			if (n.x < 0 || n.x >= size || n.y < 0 || n.y >= size || field[n.y][n.x] == '#')
				continue;
			path_t new_path(path);
			new_path.push_back(n);
			auto it = best.find(n);
			if (it == best.end() || new_path.size() < it->second.size())
			{
				best[n] = new_path;
				paths.push_back(new_path);
			}
		}
	}
	return best[point{ 0, 0 }].size() - 1;
}
long long solve1(const input_t& input, int size, int nfall)
{
	auto field = fall(input, size, nfall);
	return find_path(field);
}

string solve2(const input_t& input, int size, int nfall)
{
	auto field = fall(input, size, nfall);
	int i = nfall;
	for (; i < input.size() && find_path(field) > 0; ++i)
		field[input[i].y][input[i].x] = '#';
	return S(input[i-1].x << "," << input[i-1].y);
}

int main()
{
	test();
	ifstream is("Day18.txt");
	input_t input = read_input(is);

	cout << "Day18 Answer1: " << solve1(input, 71, 1024) << endl;
	cout << "Day18 Answer2: " << solve2(input, 71, 1024) << endl;
}
void test()
{
	auto test1 = R"(5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input, 7, 12) << endl;
	cout << "Test2: " << solve2(input, 7, 12) << endl;
}
