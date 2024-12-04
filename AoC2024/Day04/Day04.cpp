#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{ 
	return read_strings(is);
}
struct point
{
	int x;
	int y;
};

long long solve1(const input_t& input)
{
	long long sum = 0;
	string mas = "MAS";
	vector<vector<point>> dir = {
		{{-1,-1},{-2,-2},{-3,-3}},
		{{ 0,-1},{ 0,-2},{ 0,-3}},
		{{ 1,-1},{ 2,-2},{ 3,-3}},
		{{ 1, 0},{ 2, 0},{ 3, 0}},
		{{ 1, 1},{ 2, 2},{ 3, 3}},
		{{ 0, 1},{ 0, 2},{ 0, 3}},
		{{-1, 1},{-2, 2},{-3, 3}},
		{{-1, 0},{-2, 0},{-3, 0}} };

	for (int i = 0; i < input.size(); ++i)
	{
		for (int j = 0; j < input[0].size(); ++j)
		{
			if (input[i][j] != 'X')
				continue;
			for (auto d : dir)
			{
				for (int c = 0; c < 3; ++c)
				{
					int y = i + d[c].y;
					int x = j + d[c].x;
					if (y < 0 || y >= input.size() || x < 0 || x >= input[0].size() || input[y][x] != mas[c])
						break;
					if (c == 2)
						++sum;
				}
			}
		}
	}
	return sum;
}
long long solve2(const input_t& input)
{
	long long sum = 0;
	for (int i = 1; i < input.size() -1; ++i)
	{
		for (int j = 1; j < input[0].size()-1; ++j)
		{
			if (input[i][j] == 'A'
			    && (input[i - 1][j - 1] == 'M' && input[i + 1][j + 1] == 'S' || input[i - 1][j - 1] == 'S' && input[i + 1][j + 1] == 'M')
				&& (input[i - 1][j + 1] == 'M' && input[i + 1][j - 1] == 'S' || input[i - 1][j + 1] == 'S' && input[i + 1][j - 1] == 'M'))
			{
				++sum;
			}
		}
	}
	return sum;
}

int main()
{
	test();
	ifstream is("Day04.txt");
	input_t input = read_input(is);

	cout << "Day4 Answer1: " << solve1(input) << endl;
	cout << "Day4 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(..X...
.SAMX.
.A..A.
XMAS.S
.X....)";
	auto test2 = R"(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test2: " << solve1(input) << endl;
	cout << "Test3: " << solve2(input) << endl;
}
