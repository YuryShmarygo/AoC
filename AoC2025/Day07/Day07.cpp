#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
		res.push_back(line);
	return res;
}

long long split_count(input_t& field)
{
	long long sum = 0;
	for (int y = 0; y < field.size() - 1; ++y)
	{
		for (int x = 0; x < field[y].size(); ++x)
		{
			if (field[y][x] == 'S' || field[y][x] == '|')
			{
				if (field[y + 1][x] == '.')
					field[y + 1][x] = '|';
				else if (field[y + 1][x] == '^')
				{
					++sum;
					if (x > 0)
						field[y + 1][x - 1] = '|';
					if (x < field[y].size() - 1)
						field[y + 1][x + 1] = '|';
				}
			}
		}
	}
	return sum;
}
long long solve1(const input_t& input)
{
	input_t field = input;
	return split_count(field);
}
long long solve2(const input_t& input)
{
	input_t field = input;
	split_count(field);
	long long sum = 0;
	vector<vector<long long>> split(field.size(), vector<long long>(field[0].size(), 0));
	for (int y = split.size() - 1; y >= 0; --y)
	{
		for (int x = 0; x < split[y].size(); ++x)
		{
			if (field[y][x] == 'S')
			{
				sum = split[y + 1][x];
			}
			else if (field[y][x] == '|')
			{
				if (y == split.size() - 1)
					split[y][x] = 1;
				else
					split[y][x] = split[y + 1][x];
			}
			else if (field[y][x] == '^')
			{
				split[y][x] = (x > 0 ? split[y + 1][x - 1] : 0) + (x < split[y].size() - 1 ? split[y + 1][x + 1] : 0);
			}
		}
	}
	return sum;
}

int main()
{
	test();
	ifstream is("Day07.txt");
	input_t input = read_input(is);

	cout << "Day7 Answer1: " << solve1(input) << endl;
	cout << "Day7 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
