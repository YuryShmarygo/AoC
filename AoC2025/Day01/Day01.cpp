#include "../common.h"

struct rotation
{
	char dir;
	int num;
};
istream& operator>>(istream& is, rotation& r) { return is >> r.dir >> r.num; }
typedef vector<rotation> input_t;
input_t read_input(istream& is) 
{ 
	istream_iterator<rotation> start(is), end;
	input_t input(start, end); 
	return input; 
}

long long solve1(const input_t& input)
{
	long long sum = 0;

	int pos = 50;
	for (auto& r : input)
	{
		if (r.dir == 'L')
			pos = (pos - r.num) % 100;
		else
			pos = (pos + r.num) % 100;
		sum += pos == 0 ? 1 : 0;
	}

	return sum;
}
long long solve2(const input_t& input)
{
	long long sum = 0;

	int pos = 50;
	for (auto& r : input)
	{
		if (r.dir == 'L')
		{
			if (pos - r.num >= 0)
			{
				pos = pos - r.num;
				if (pos == 0)
					sum += 1;
			}
			else
			{
				int n = pos - r.num;
				int i = abs(n) / 100;
				sum += i + (pos == 0 ? 0 : 1);
				pos = (100 + (n + i * 100)) % 100;
			}
		}
		else
		{
			sum += (pos + r.num) / 100;
			pos = (pos + r.num) % 100;
		}
	}

	return sum;
}

int main()
{
	test();
	ifstream is("Day01.txt");
	input_t input = read_input(is);

	cout << "Day1 Answer1: " << solve1(input) << endl;
	cout << "Day1 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
