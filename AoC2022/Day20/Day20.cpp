#include "../common.h"


list<long long>::iterator get_it(list<long long>& sh, list<long long>::iterator it, long long num)
{
//	int num = *it;
	num = num > 0 ? num % (long long)sh.size() : -(-num % (long long)sh.size());
	for (; num > 0; --num)
	{
		if (it == sh.end())
			it = sh.begin();
		it = next(it);
	}
	for (; num < 0; ++num)
	{
		if (it == sh.begin())
			it = sh.end();
		it = prev(it);
	}
	return it;
}
long long solve1(const vector<long long>& input, int steps = 1)
{
	list<long long> sh(input.begin(), input.end());
	list<list<long long>::iterator> order;
	for (auto it = sh.begin(); it != sh.end(); ++it) { order.push_back(it); }
	for (; steps > 0; --steps)
	{
		for (auto& it : order)
		{
			long long num = *it;
			it = sh.erase(it);
			auto itn = get_it(sh, it, num);
			it = sh.insert(itn, num);
//			r::copy(sh, ostream_iterator<int>(cout, ",")); cout << endl;
		}
	}
	auto it = r::find_if(sh, [](auto v) {return v == 0; });
	return *get_it(sh, it, 1000) + *get_it(sh, it, 2000) + *get_it(sh, it, 3000);
}
long long solve2(vector<long long> input)
{
	for (auto& v : input) v *= 811589153;
	return solve1(input, 10);
}
int main()
{
	test(); //return 0;
	ifstream is("Day20.txt");
	istream_iterator<long long> start(is), end;
	vector<long long> input(start, end);

	cout << "Day20 answer1: " << solve1(input) << endl;
	cout << "Day20 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(1
2
-3
3
-2
0
4)");
	istream_iterator<long long> start(is), end;
	vector<long long> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
