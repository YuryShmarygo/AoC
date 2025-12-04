#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is)
{
	return read_strings(is);
}

bool is_less4(int x, int y, const input_t& input)
{
	int count = ((x > 0 && y > 0 && input[y - 1][x - 1] == '@') ? 1 : 0)
		+ ((y > 0 && input[y - 1][x] == '@') ? 1 : 0)
		+ ((x < input[0].size() - 1 && y > 0 && input[y - 1][x + 1] == '@') ? 1 : 0)
		+ ((x > 0 && input[y][x - 1] == '@') ? 1 : 0)
		+ ((x < input[0].size() - 1 && input[y][x + 1] == '@') ? 1 : 0)
		+ ((x > 0 && y < input.size() - 1 && input[y + 1][x - 1] == '@') ? 1 : 0)
		+ ((y < input.size() - 1 && input[y + 1][x] == '@') ? 1 : 0)
		+ ((x < input[0].size() - 1 && y < input.size() - 1 && input[y + 1][x + 1] == '@') ? 1 : 0);
	return count < 4;
}
long long solve1(const input_t& input)
{
	long long sum = 0;
	
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[0].size(); ++x)
		{
			if (input[y][x] == '@' && is_less4(x, y, input))
				++sum;
		}
	}

	return sum;
}

long long solve2(const input_t& input)
{
	input_t rolls = input;
	long long sum = 0;

	bool removed = true;
	while(removed)
	{
		removed = false;
		for (int y = 0; y < rolls.size(); ++y)
		{
			for (int x = 0; x < rolls[0].size(); ++x)
			{
				if (rolls[y][x] == '@' && is_less4(x, y, rolls))
				{
					rolls[y][x] = '.';
					removed = true;
					++sum;
				}
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
	auto test1 = R"(..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
