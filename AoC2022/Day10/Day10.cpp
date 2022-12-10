#include "../common.h"

struct op
{
	int cycles;
	int num;
};
istream& operator>>(istream& is, op& o)
{
	string str;
	is >> str;
	if (str == "noop")
	{
		o.cycles = 1;
		o.num = 0;
	}
	else
	{
		o.cycles = 2;
		is >> o.num;
	}
	return is;
}
int calculate(const vector<op>& input)
{
	int x = 1;
	int cycle = 1;
	int sum = 0;
	for (auto o : input)
	{
		if (cycle == 20 || cycle == 19 && o.cycles == 2 || cycle > 20 && ((cycle-20) % 40 == 0 || o.cycles == 2 && (cycle - 19) % 40 == 0))
		{
			sum += ((cycle == 20 || (cycle-20) % 40 == 0) ? cycle : (cycle + 1)) * x;
		}
		x += o.num;
		cycle += o.cycles;
		if (cycle > 220)
			break;
	}
	return sum;
}
void render(const vector<op>& input)
{
	int x = 1;
	int cycle = 1;
	for (auto o : input)
	{
		if ((cycle - 1) % 40 == 0)
			cout << endl;
		int pos = (cycle-1) % 40;
		cout << ((pos >= x - 1 && pos <= x + 1) ? '#' : '.');
		if (o.cycles == 2)
		{
			pos = cycle % 40;
			if (cycle % 40 == 0)
				cout << endl;
			cout << ((pos >= x - 1 && pos <= x + 1) ? '#' : '.');
		}
		x += o.num;
		cycle += o.cycles;
	}
}
int main()
{
	test();
	ifstream is("Day10.txt");
	istream_iterator<op> start(is), end;
	vector<op> input(start, end);
	cout << "Day10 answer1: " << calculate(input) << endl;
	render(input);
	cout << endl;
}
void test()
{
	ifstream is("Day10_test.txt");
	istream_iterator<op> start(is), end;
	vector<op> input(start, end);
	cout << "test1: " << calculate(input) << endl;
	render(input);
	cout << endl;
}
