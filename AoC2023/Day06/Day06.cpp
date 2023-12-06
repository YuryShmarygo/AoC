#include "../common.h"

struct race
{
	long long time;
	long long record;
};
typedef vector<race> races;

long long solve1(const races& rs)
{
	long long res = 1;
	for (auto& r : rs)
	{
		long long beats = 0;
		for (long long i = 0; i <= r.time; ++i)
		{
			long long dist = (r.time - i) * i;
			if (dist > r.record)
				++beats;
		}
		res *= beats;
	}
	return res;
}
long long solve2(const race& rs)
{
	long long min_t = 0;
	long long cur, count, step;
	count = rs.time / 2;
	while (count > 0)
	{
		cur = min_t;
		step = count / 2;
		cur += step;
		if ((rs.time - cur) * cur < rs.record)
		{
			min_t = ++cur;
			count -= step + 1;
		}
		else
			count = step;
	}
	long long max_t = rs.time / 2;
	count = rs.time / 2;
	while (count > 0)
	{
		cur = max_t;
		step = count / 2;
		cur += step;

		if ((rs.time - cur) * cur > rs.record)
		{
			max_t = ++cur;
			count -= step + 1;
		}
		else
			count = step;
	}
	
	return max_t - min_t;
}
int main()
{
	test();
	//ifstream is("Day06.txt");

	races r = { {53,313},{89,1090},{76,1214},{98,1201} };
	race r2 = { 53897698, 313109012141201 };

	auto t_start = high_resolution_clock::now();
	cout << "Day4 Answer1: " << solve1(r) << " took: " << ElapsedMs(t_start) << endl;
	t_start = high_resolution_clock::now();
	cout << "Day4 Answer2: " << solve2(r2) << " took: " << ElapsedMs(t_start) << endl;
	t_start = high_resolution_clock::now();
	cout << "Day4 Answer2: " << solve1({ r2 }) << " took: " << ElapsedMs(t_start) << endl;
}

void test()
{
	auto test1 = R"(Time:      7  15   30
Distance:  9  40  200)";

	races r = { {7,9},{15,40},{30,200} };
	race r2 = { 71530, 940200 };

	cout << "Test1: " << solve1(r) << endl;
	cout << "Test2: " << solve2(r2) << endl;
}
