#include "../common.h"

typedef vector<int> input_t;
input_t read_input(istream& is) 
{
	istream_iterator<int> start(is), end;
	return input_t(start, end);
}
list<int> get_digits(long long num)
{
	list<int> res;
	while (num > 0)
	{
		int d = num % 10;
		res.insert(res.begin(), d);
		num /= 10;
	}
	return res;
}
long long get_num(list<int>::const_iterator start, list<int>::const_iterator end)
{
	long long res = 0;
	for (; start != end; ++start)
	{
		res *= 10;
		res += *start;
	}
	return res;
}
tuple<long long, long long> split(const list<int>& digits)
{
	int s = (int)digits.size() / 2;
	auto it = digits.begin();
	auto ite = it;
	for (; s > 0; --s)
		++ite;
	return { get_num(it, ite), get_num(ite, digits.end()) };
}
void blink(list<long long>& stones)
{
	for (auto it = stones.begin(); it != stones.end(); ++it)
	{
		if (*it == 0)
			*it = 1;
		else
		{
			auto digits = get_digits(*it);
			if (digits.size() % 2 == 0)
			{
				auto [n1, n2] = split(digits);
				*it = n2;
				it = stones.insert(it, n1);
				++it;
			}
			else
			{
				*it *= 2024;
			}
		}
	}
}
list<long long> get_stones(const input_t& input)
{
	return list<long long>(input.begin(), input.end());
}
long long solve(const input_t& input, int n)
{
	map<long long, long long> stones;
	map<long long, list<long long>> cache;
	for (auto i : input)
		++stones[i];
	ofstream log; log.open("day11.out");
	for (int i = 0; i < n; ++i)
	{
		map<long long, long long> step;
		for (auto s : stones)
		{
			auto it = cache.find(s.first);
			if (it == cache.end())
			{
				list<long long> l = { s.first };
				blink(l);
				auto [itn, b] = cache.insert(make_pair(s.first, l));
				it = itn;
			}
			for (auto n : it->second)
				step[n] += s.second;
		}
		swap(step, stones);
	}
	long long num = 0;
	
	for (auto s : stones)
	{
		num += s.second;
		log << s.first << "\t" << s.second << endl;
	}
	return num;
}
long long solve1(const input_t& input)
{
	/*
	auto stones = get_stones(input);
	for (int i = 0; i < 25; ++i)
	{
		blink(stones);
	}
	return stones.size();
	*/
	return solve(input, 25);
}

long long solve2(const input_t& input)
{
	return solve(input, 75);
}

int main()
{
	test();
	ifstream is("Day11.txt");
	input_t input = read_input(is);

	cout << "Day11 Answer1: " << solve1(input) << endl;
	cout << "Day11 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(0 1 10 99 999)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	auto stones = get_stones(input);
	blink(stones);
	cout << "Test1: "; r::copy(stones, ostream_iterator<long long>(cout, " ")); cout << endl;

	stones = { 0, 1, 10, 99, 999, 1234};
	blink(stones);
	cout << "Test4: "; r::copy(stones, ostream_iterator<long long>(cout, " ")); cout << endl;

	//cout << "Test2: " << solve2(input) << endl;

	auto test2 = R"(125 17)";
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test3: " << solve1(input) << endl;
}
