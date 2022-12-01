#include "../common.h"

struct elf
{
	vector<long> calories;
	long sum;
};

istream& operator>>(istream& is, elf& e)
{
	e.calories.clear();
	string line;
	while (getline(is,line) && !line.empty())
		e.calories.push_back(stol(line));
	e.sum = accumulate(e.calories.begin(), e.calories.end(), 0);
	return is;
}
void test();
int main()
{
	test();
	ifstream is("Day1.txt");
	istream_iterator<elf> start(is), end;
	vector input(start, end);
	cout << "Read " << input.size() << " Elfs" << endl;

	ranges::sort(input, [](auto& e1, auto& e2) {return e1.sum > e2.sum; });

	cout << "Day1 Answer1: " << input[0].sum << endl;
	cout << "Day1 Answer2: " << input[0].sum + input[1].sum + input[2].sum << endl;
}

void test()
{
	stringstream is(R"(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000)");

	istream_iterator<elf> start(is), end;
	vector<elf> input(start, end);
	cout << "Read " << input.size() << " Elfs" << endl;

	ranges::sort(input, [](auto& e1, auto& e2) {return e1.sum > e2.sum; });

	cout << "Test1: " << input[0].sum << endl;
	cout << "Test2: " << input[0].sum + input[1].sum + input[2].sum << endl;
}
