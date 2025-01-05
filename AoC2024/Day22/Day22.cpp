#include "../common.h"

typedef vector<int> input_t;
input_t read_input(istream& is)
{
	istream_iterator<int> start(is), end;
	return input_t(start, end);
}
long long mix(long long secret, long long val)
{
	return secret ^ val;
}
long long prune(long long secret)
{
	return secret % 16777216;
}
long long calc(long long secret)
{
	secret = prune(mix(secret, secret * 64));
	secret = prune(mix(secret, secret / 32)); //TODO might need fload rounding
	return prune(mix(secret, secret * 2048));
}
long long solve1(const input_t& input)
{
	long long res = 0;
	for (long long n : input)
	{
		for (int i = 0; i < 2000; ++i)
			n = calc(n);
		res += n;
	}
	return res;
}
typedef tuple<int, int, int, int> seq_t;
typedef map<seq_t, int> sell_t;
int get_price(long long num) { return num % 10; }
sell_t get_sells(long long num)
{
	sell_t res;
	vector<int> d; d.reserve(2000);
	int p = get_price(num);
	for (int i = 0; i < 2000; ++i)
	{
		d.push_back(get_price(num = calc(num)) - p);
		p = get_price(num);
		if (i >= 3)
		{
			auto seq = make_tuple(d[i - 3], d[i - 2], d[i - 1], d[i]);
			auto it = res.find(seq);
			if (it == res.end())
				res[seq] = p;
		}
	}
	return res;
}
typedef vector<sell_t> sells_t;
int get_rank(const seq_t& seq, const sells_t& sells)
{
	int res = 0;
	for (auto& sell : sells)
	{
		auto it = sell.find(seq);
		if (it != sell.end())
			res += it->second;
	}
	return res;
}
long long solve2(const input_t& input)
{
	//auto t_start = high_resolution_clock::now();
	sells_t sells; sells.reserve(input.size());
	sell_t freq;
	sell_t cache;
	for (auto n : input)
	{
		auto sell = get_sells(n);
		sells.push_back(sell);
		for (auto& s : sell)
			++freq[s.first];
	}
	vector<tuple<seq_t, int>> freq_count(freq.begin(), freq.end());
	sort(freq_count.begin(), freq_count.end(), [](auto& f1, auto& f2) { return get<1>(f1) > get<1>(f2); });
	//cout << endl << "building sells tool: " << ElapsedMs(t_start) << endl;

	int res = 0;
	int i = 0;
	for (auto it = freq_count.begin(); it != freq_count.end() && i < 2000; ++it, ++i)
	{
		auto itc = cache.find(get<0>(*it));
		if (itc == cache.end())
		{
			int rank = get_rank(get<0>(*it), sells);
			if (rank > res)
				res = rank;
			cache[get<0>(*it)] = rank;
		}

	}
	return res;
}

int main()
{
	test();
	ifstream is("Day22.txt");
	input_t input = read_input(is);

	cout << "Day22 Answer1: " << solve1(input) << endl;
	cout << "Day22 Answer2: " << solve2(input) << endl;
}
void test()
{
	long long num = 123;
	cout << "test " << num << endl;
	for (int i = 0; i < 10; ++i)
		cout << (num = calc(num)) << endl;

	auto test1 = R"(1
10
100
2024)";
	auto test2 = R"(1
2
3
2024)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1 (expect 37327623): " << solve1(input) << endl;
	is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve2(input) << endl;
}
