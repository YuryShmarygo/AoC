#include "../common.h"

typedef vector<long long> vseq;
istream& operator>>(istream& is, vseq& s) { istream_iterator<long long> start(is), end; s = vseq(start, end); return is; }
typedef vector<vseq> report;
istream& operator>>(istream& is, report& r)
{
	r.clear();
	string line;
	while (getline(is, line) && !line.empty())
	{
		stringstream iis(line);
		vseq l; iis >> l;
		r.push_back(l);
	}
	return is;
}
vseq diff(const vseq& s)
{
	vseq d;
	for (int i = 0; i < s.size() - 1; ++i)
		d.push_back(s[i + 1] - s[i]);
	return d;
}
report produce(const vseq& s)
{
	report d(1, s);
	while (!r::all_of(d.back(), [](auto& i) { return i == 0; }))
		d.push_back(diff(d.back()));
	return d;
}
long long solve1(const report& r)
{
	long long res = 0;
	for (auto& s : r)
	{
		auto d = produce(s);
		res += accumulate(d.begin(), d.end(), (long long)0, [](auto sum, auto& s) { return sum + s[s.size() - 1]; });
	}
	return res;
}
long long solve2(const report& r)
{
	long long res = 0;
	for (auto& s : r)
	{
		auto d = produce(s);
		res += accumulate(d.rbegin(), d.rend(), (long long)0, [](auto sum, auto& s) { return s[0] - sum; });
	}
	return res;
}
int main()
{
	test();
	ifstream is("Day09.txt");
	report r; is >> r;

	auto t_start = high_resolution_clock::now();
	cout << "Day9 Answer1: " << solve1(r) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day9 Answer2: " << solve2(r) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45))";

	auto is = stringstream(test1);
	report r; is >> r;

	cout << "Test1: " << solve1(r) << endl;
	cout << "Test2: " << solve2(r) << endl;
}
