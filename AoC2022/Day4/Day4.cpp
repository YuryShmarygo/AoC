#include "../common.h"

typedef pair<set<int>, set<int>> cc;
typedef vector<cc> vecc;

set<int> gen(int s, int e)
{
	set<int> res;
	while (s <= e)
	{
		res.insert(s);
		++s;
	}
	return res;
}
cc from_string(string str)
{
	cc c;
	smatch m;
	if (regex_match(str, m, regex("(\\d+)-(\\d+),(\\d+)-(\\d+)")))
	{
		c.first = gen(stoi(m[1]), stoi(m[2]));
		c.second = gen(stoi(m[3]), stoi(m[4]));
	}
	return c;
}
vecc from_string(vector<string>& input)
{
	vecc res;
	for (auto str : input)
		res.push_back(from_string(str));
	return res;
}
int count_intersect(set<int>& s1, set<int>& s2)
{ return (int)r::count_if(s1, [&](auto e) {return s2.contains(e);}); }
int count_includes(vector<string>& input)
{
	vecc pairs = from_string(input);
	return (int)r::count_if(pairs, [](auto& p)
		{ return (int)min(p.first.size(), p.second.size()) == count_intersect(p.first, p.second); });
}
int count_overlap(vector<string>& input)
{
	vecc pairs = from_string(input);
	return (int)r::count_if(pairs, [](auto& p)
		{ return 0 < count_intersect(p.first, p.second); });
}

int main()
{
	ifstream is("Day4.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " commands" << endl;

	cout << "Day4 answer1: " << count_includes(input) << endl;
	cout << "Day4 answer2: " << count_overlap(input) << endl;
}
