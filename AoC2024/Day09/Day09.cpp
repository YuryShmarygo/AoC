#include "../common.h"

typedef string input_t;
input_t read_input(istream& is) 
{
	string s;
	is >> s;
	return s;
}
typedef vector<int> fmap_t;
fmap_t build_map(const input_t& input)
{
	fmap_t res; res.reserve(input.size() * 9);
	for (int i = 0; i < input.size(); ++i)
	{
		int id = i % 2 == 0 ? i / 2 : -1;
		res.insert(res.end(), input[i]-'0', id);
	}
	return res;
}
long long solve1(const input_t& input)
{
	fmap_t m = build_map(input);
	auto it = prev(m.end());
	while (it != m.begin() && *it == -1)
		--it;
	auto itf = find(m.begin(), m.end(), -1);
	while (itf < it)
	{
		*itf = *it;
		*it = -1;
		--it;
		++itf;
		while (it != itf && *it == -1)
			--it;
		itf = find(itf, it, -1);
	}
	long long sum = 0;
	for (int i = 0; i < m.size() && m[i] != -1; ++i)
	{
		sum += i * m[i];
	}
	return sum;
}
fmap_t::iterator find_free(fmap_t::iterator its, fmap_t::iterator ite, int d)
{
	fmap_t::iterator it = its;
	while (it != ite && *it != -1)
		++it;
	while (it != ite)
	{
		auto iti = it;
		while (iti != ite && *iti == -1 && iti - it + 1 < d)
			++iti;
		if (iti != ite && *iti == -1 && iti - it + 1 == d)
			return it;
		it = iti;
		while (it != ite && *it != -1)
			++it;
	}
	return ite;
}
long long solve2(const input_t& input)
{
	fmap_t m = build_map(input);
	auto it = prev(m.end());
	while (it != m.begin() && *it == -1)
		--it;
	while (it > m.begin())
	{
		auto it2 = it;
		while (it2 != m.begin() && *it2 == *it)
			--it2;
		int d = it - it2;
		auto itf = find_free(m.begin(), it, d);
		if (itf != it)
		{
			while (d > 0)
			{
				*itf = *it;
				*it = -1;
				--d;
				++itf;
				--it;
			}
		}
		else
		{
			while (d > 0)
			{
				--d;
				--it;
			}
		}
		while (it != m.begin() && *it == -1)
			--it;
	}
	long long sum = 0;
	for (int i = 0; i < m.size(); ++i)
	{
		sum += m[i] != -1 ? i * m[i] : 0;
	}
	return sum;
}

int main()
{
	test();
	ifstream is("Day09.txt");
	input_t input = read_input(is);

	cout << "Day9 Answer1: " << solve1(input) << endl;
	//6398097037768 too high
	cout << "Day9 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(2333133121414131402)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
