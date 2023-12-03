#include "../common.h"

typedef map<string, int> game_set;
ostream& operator<<(ostream& os, const game_set& gs) { os << "["; for (auto& p : gs) os << p.first << ":" << p.second << ","; os << "]"; return os; }
ostream& operator<<(ostream& os, const vector<game_set>& gset) { for (auto& p : gset) os << p << ","; return os; }

struct game
{
	game() : id(0), s() {}
	int id;
	vector<game_set> s;
};
ostream& operator<<(ostream& os, const game& g){ return os << "Game " << g.id << ": " << g.s; }

istream& operator>>(istream& is, game& g)
{
	g.s.clear();
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex(R"(Game (\d+):(.+)$)")))
	{
		g.id = stoi(m[1].str());
		string sets_str = m[2].str();
		smatch s;
		while (regex_search(sets_str, s, regex(R"(^;? ([^;]+))")))
		{
			string gset_str = s[1].str();
			smatch ss;
			game_set gset;
			while (regex_search(gset_str, ss, regex(R"(^(\d+) (green|red|blue)(?:, )?)")))
			{
				gset[ss[2]] = stoi(ss[1].str());
				gset_str = ss.suffix();
			}
			g.s.push_back(gset);
			sets_str = s.suffix();
		}
	}

	return is;
}

int possible(const vector<game>& input, const game_set& r)
{
	auto p_s = [](const game_set& s, const game_set& r) { return all_of(s.begin(), s.end(), [&](auto& k) {return k.second <= r.find(k.first)->second; }); };
	auto p_ss = [&](const game& g, const game_set& r) { return all_of(g.s.begin(), g.s.end(), [&](auto& s) { return p_s(s, r); }); };
	auto p_num = [&](const game& g, const game_set& r) { return p_ss(g, r) ? g.id : 0; };
	return accumulate(input.begin(), input.end(), (int) 0, [&](int sum, auto& g) { return sum + p_num(g, r); });
}
int possible(const vector<game>& input)
{
	game_set r({ {"red", 12},{"green", 13},{"blue",14} });
	return possible(input, r);
}

long long p_s(const game_set& s) { return s.find("red")->second * s.find("green")->second * s.find("blue")->second; }
int c_max(const game_set& s, string c) { auto it = s.find(c); return it != s.end() ? it->second : 0; }
int s_max(const vector<game_set>& ss, string c) { int r = 0; for (auto& s : ss) r = max(r, c_max(s, c)); return r; }
game_set min_s(const vector<game_set>& ss) { game_set m; m["red"] = s_max(ss, "red"); m["green"] = s_max(ss, "green"); m["blue"] = s_max(ss, "blue");  return m; }

long long task2(const vector<game>& gg)
{
	return accumulate(gg.begin(), gg.end(), (long long)0, [](auto sum, auto& g) { return sum + p_s(min_s(g.s)); });
}

void test();
int main()
{
	test();
	ifstream is("Day02.txt");
	istream_iterator<game> start(is), end;
	vector<game> input(start, end);

	cout << "Day2 Answer1: " << possible(input) << endl;
	cout << "Day2 Answer2: " << task2(input) << endl;
}
void test()
{
	auto test1 = R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";

	auto is = stringstream(test1);
	istream_iterator<game> start(is), end;
	vector<game> input(start, end);
	//cout << input.size() << endl;	r::copy(input, ostream_iterator<game>(cout, "\n"));
	cout << "Test1: " << possible(input) << endl;
	cout << "Test2: " << task2(input) << endl;
}
