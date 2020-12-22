#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

typedef string Cards;
istream& operator>>(istream& is, Cards& cards)
{
	string line;
	while (is && getline(is, line) && !line.empty())
	{
		if (line[0] == 'P')
			continue;
		cards.push_back(stoi(line));
	}
	return is;
}
ostream& operator<<(ostream& os, const Cards& cards)
{ copy(cards.begin(), cards.end(), ostream_iterator<int>(cout, " ")); return os; }
int score(const Cards& cards)
{
	int i = 1;
	return accumulate(cards.rbegin(), cards.rend(), (int)0, [&](auto sum, auto card) {return sum + card * (i++); });
}
int play(const vector<Cards>& cards)
{
	vector<Cards> p(cards);
	while (!p[0].empty() && !p[1].empty())
	{
		auto c1 = p[0][0]; p[0].erase(0, 1);
		auto c2 = p[1][0]; p[1].erase(0, 1);
		if (c1 > c2)
			p[0].insert(p[0].end(), { c1, c2 });
		else
			p[1].insert(p[1].end(), { c2, c1 });
	}
	auto& win = p[0].empty() ? p[1] : p[0];
	return score(win);
}
struct Game
{
	Cards p[2];
	set<string> played[2];
};
int rplay(const vector<Cards>& cards)
{
	list<Game> games{ {{cards[0],cards[1]}} };
	auto it = games.begin();
	int w = -1;
	while (games.size() > 1 || !games.front().p[0].empty() && !games.front().p[1].empty())
	{
		auto& cur = *it;
		if (cur.played[0].end() != cur.played[0].find(cur.p[0]) || cur.played[1].end() != cur.played[1].find(cur.p[1]))
		{
			w = 0;
			if (games.size() > 1)
			{
				games.pop_back();
				it = prev(games.end());
			}
			continue;
		}
		if (cur.p[0].empty() || cur.p[1].empty())
		{
			w = cur.p[0].empty() ? 1 : 0;
			if (games.size() > 1)
			{
				games.pop_back();
				it = prev(games.end());
			}
			continue;
		}
		auto c1 = cur.p[0].front();
		auto c2 = cur.p[1].front();
		if (c1 < cur.p[0].size() && c2 < cur.p[1].size() && w == -1)
		{
			Cards n1;
			for (auto it = next(cur.p[0].begin()); c1 > 0; --c1, ++it)
				n1.push_back(*it);
			Cards n2;
			for (auto it = next(cur.p[1].begin()); c2 > 0; --c2, ++it)
				n2.push_back(*it);
			games.push_back(Game{ {n1, n2} });
			it = prev(games.end());
			continue;
		}
		w = w == -1 ? (c1 > c2 ? 0 : 1) : w;
		cur.played[0].insert(cur.p[0]);
		cur.played[1].insert(cur.p[1]);
		cur.p[0].erase(0,1); cur.p[1].erase(0,1);
		cur.p[w].insert(cur.p[w].end(), { (w == 0 ? c1 : c2), (w == 0 ? c2 : c1) });
		w = -1;
	}
	return score(games.front().p[0].empty() ? games.front().p[1] : games.front().p[0]);
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day22.txt");
	vector<Cards> cards(2);
	is >> cards[0];
	is >> cards[1];

	cout << "Day22 Answer1: " << play(cards) << endl;
	cout << "Day22 Answer2: " << rplay(cards) << endl;
}

void Test()
{
	stringstream test(R"(Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10)");
	vector<Cards> cards(2);
	test >> cards[0];
	test >> cards[1];
	cout << "Test 1: " << play(cards) << endl;
	cout << "Test 2: " << rplay(cards) << endl;
}