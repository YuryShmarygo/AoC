#include "../common.h"

struct card
{
	int id;
	vector<int> w;
	vector<int> n;
};
istream& operator>>(istream& is, vector<int>& n)
{
	n.clear();
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	while (regex_search(line, m, regex(R"(^ *(\d+))")))
	{
		n.push_back(stoi(m[1].str()));
		line = m.suffix();
	}
	return is;
}
istream& operator>>(istream& is, card& c)
{
	c = card({ 0, vector<int>(), vector<int>() });
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex(R"(Card *(\d+):(.+)$)")))
	{
		c.id = stoi(m[1].str());
		string card_str = m[2].str();
		smatch m2;
		if (regex_match(card_str, m2, regex(R"((.+)\|(.+))")))
		{
			string w_str = m2[1].str();
			auto w_s = stringstream(w_str);
			w_s >> c.w;
			string n_str = m2[2].str();
			auto w_n = stringstream(n_str);
			w_n >> c.n;
		}
	}
	return is;
}

long long solve1(const vector<card>& input)
{
	long long res = 0;
	for (auto& c : input)
	{
		long long cur = 0;
		for (auto& n : c.n)
		{
			if (find(c.w.begin(), c.w.end(), n) != c.w.end())
				cur = cur == 0 ? 1 : cur + cur;
		}
		res += cur;
	}
	return res;
}

struct wcard
{
	int m;
	int n;
};
long long solve2(const vector<card>& input)
{
	vector<wcard> w;
	for (auto& c : input)
	{
		int cur = 0;
		for (auto& n : c.n)
		{
			if (find(c.w.begin(), c.w.end(), n) != c.w.end())
				++cur;
		}
		w.push_back(wcard({ cur, 1 }));
	}
	for (int i = 0; i < w.size(); ++i)
	{
		for (int j = i + 1; j <= i + w[i].m && j < w.size(); ++j)
			w[j].n += w[i].n;
	}

	long long res = 0;
	for (auto& c : w)
		res += c.n;

	return res;
}

void test();
int main()
{
	test();
	ifstream is("Day04.txt");
	istream_iterator<card> start(is), end;
	vector<card> input(start, end);

	cout << "Day4 Answer1: " << solve1(input) << endl;
	cout << "Day4 Answer2: " << solve2(input) << endl;
}

void test()
{
	auto test1 = R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";

	auto is = stringstream(test1);
	istream_iterator<card> start(is), end;
	vector<card> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
