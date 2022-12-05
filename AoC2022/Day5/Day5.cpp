#include "../common.h"

istream& operator>>(istream& is, vector<string>& s)
{
	map<size_t, string> r;
	vector<string> input;
	string line;
	while (getline(is, line) && !line.empty())
	{
		for (size_t i = 1; i < line.size(); i += 4)
			if (line[i] >= 'A' && line[i] <= 'Z')
				r[i].insert(r[i].begin(), line[i]);
	}
	r::transform(r, back_inserter(s), [](auto it) {return it.second; });
	return is;
}
struct smove
{
	int n;
	int from;
	int to;
};
istream& operator>>(istream&is, smove& m)
{
	string skip;
	return is >> skip >> m.n >> skip >> m.from >> skip >> m.to;
}
istream& operator>>(istream& is, vector<smove>& moves)
{
	istream_iterator<smove> s(is), e;
	moves.assign(s, e);
	return is;
}
struct cargo
{
	vector<string> stacks;
	vector<smove> moves;
	void config()
	{
		for (auto m : moves)
		{
			for (int i = 0; i < m.n; ++i)
			{
				stacks[m.to - 1].push_back(stacks[m.from - 1].back());
				stacks[m.from - 1].pop_back();
			}
		}
	}
	void config2()
	{
		for (auto m : moves)
		{
			auto s = stacks[m.from - 1].begin() + stacks[m.from - 1].size() - m.n;
			auto e = stacks[m.from - 1].end();
			stacks[m.to - 1].insert(stacks[m.to - 1].end(), s, e);
			stacks[m.from - 1].erase(s, e);
		}
	}
	string top()
	{
		string res;
		for (auto s : stacks)
			res.push_back(s.back());
		return res;
	}
};
istream& operator>>(istream& is, cargo& c)
{ return is >> c.stacks >> c.moves; }

string solve1(cargo c)
{
	c.config();
	return c.top();
}
string solve2(cargo c)
{
	c.config2();
	return c.top();
}

int main()
{
	ifstream is("Day5.txt");
	cargo c;
	is >> c;

	cout << "Day5 answer1: " << solve1(c) << endl;
	cout << "Day5 answer2: " << solve2(c) << endl;
}
