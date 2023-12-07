#include "../common.h"

struct hand
{
	string h;
	long long b;
};
istream& operator>>(istream& is, hand& h) { return is >> h.h >> h.b; }
ostream& operator<<(ostream& os, const hand& h) { return os << h.h << ' ' << h.b; }
ostream& operator<<(ostream& os, const vector<hand>& h) { for (auto& hh : h) os << hh << endl; return os; }
map<char, int> c_s{ {'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1}, {'1', 0} };
struct match
{
	char c;
	int n;
};
int strength(string h)
{
	map<char, int> m;
	for (auto c : h)
		m[c] = r::count(h, c);

	auto it1 = r::max_element(m, [](auto& m1, auto& m2) {return m1.first == '1' || m2.first != '1' && m1.second < m2.second; });
	auto it2 = r::max_element(m, [it1](auto& m1, auto& m2) {return m1.first == it1->first || m1.first == '1' || m2.first != it1->first && m2.first != '1' && m1.second < m2.second; });

	auto itj = m.find('1');
	if (itj != m.end() && it1->first != '1')
		it1->second += itj->second;

	return pow(10, it1->second) + pow(10, it2->second);
}
bool operator<(const hand& h1, const hand& h2)
{
	int s1 = strength(h1.h);
	int s2 = strength(h2.h);
	bool is_high = false;
	if (s1 == s2)
		for (int i = 0; i < h1.h.size(); ++i)
		{
			if (c_s[h1.h[i]] == c_s[h2.h[i]])
				continue;
			is_high = c_s[h1.h[i]] > c_s[h2.h[i]];
			break;
		}
	return s1 < s2 || s1 == s2 && !is_high;
}
long long solve1(vector<hand> hands)
{
	r::sort(hands, less<hand>());
//	cout << hands << endl;
	long long res = 0;
	for (int i = 0; i < hands.size(); ++i)
		res += (long long)hands[i].b * (i+1);
	return res;
}
long long solve2(vector<hand> hands)
{
	for (auto& h : hands)
		replace(h.h.begin(), h.h.end(), 'J', '1');
	return solve1(hands);
}
int main()
{
	test();
	ifstream is("Day07.txt");
	istream_iterator<hand> start(is), end;
	vector<hand> hands(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day7 Answer1: " << solve1(hands) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day7 Answer2: " << solve2(hands) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";

	auto is = stringstream(test1);
	istream_iterator<hand> start(is), end;
	vector<hand> hands(start, end);

	cout << "Test1: " << solve1(hands) << endl;
	cout << "Test2: " << solve2(hands) << endl;
}
