#include "../common.h"

map<char, long long> c2n{ {'2',2},{'1',1},{0,0},{'-',-1},{'=',-2} };
map<int, char> n2c{ {2,'2'},{1,'1'},{0,'0'},{-1,'-'},{-2,'='} };
long long to_num(string str)
{
	long long res = 0;
	long long pow = 1;
	for (auto it = str.rbegin(); it != str.rend(); ++it, pow *= 5)
		res += c2n[*it] * pow;
	return res;
}
string to_5(long long num)
{
	string res;
	while (num > 0)
	{
		int r = static_cast<int>(num  - (num / 100) * 100) % 5;
		if (r > 2)
		{
			num += 5;
			r -= 5;
		}
		num = num / 5;
		res.insert(res.begin(), n2c[r]);
	}
	return res;
}
string solve1(const vector<string>& input)
{
	long long sum = accumulate(input.begin(), input.end(), (long long)0, [](auto sum, auto str) { return sum + to_num(str); });
//	cout << endl << "sum: " << sum << " to_5: " << to_5(sum) << " back:" << to_num(to_5(sum)) << endl;
	return to_5(sum);
}
int solve2(const vector<string>& input)
{
	return 2;
}
int main()
{
	test(); //return 0;
	ifstream is("Day25.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "Day25 answer1: " << solve1(input) << endl;
}
void test()
{
	stringstream is(R"(1=-0-2
 12111
  2=0=
    21
  2=01
   111
 20012
   112
 1=-1=
  1-12
    12
    1=
   122)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
}
