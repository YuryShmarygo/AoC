#include "../common.h"

struct valve
{
	string name;
	int pr;
	list<string> valves;
};
istream& operator>>(istream& is, valve& v)
{
	string line; getline(is, line);
	if (line.empty()) return is;
	stringstream il(line);
	string str;
	il >> str >> v.name >> str >> str >> str;
	v.pr = stoi(str.substr(5, str.size() - 6));
	il >> str >> str >> str >> str;
	v.valves.clear();
	while (il >> str)
		v.valves.push_back(str.substr(0, 2));
	return is;
}
struct path
{
	int pr;
	list<string> valves;
	set<string> opened;
};
ostream& operator<<(ostream& os, const list<string>& v) { r::copy(v, ostream_iterator<string>(os,",")); return os; }
ostream& operator<<(ostream& os, const set<string>& o) { r::copy(o, ostream_iterator<string>(os, ",")); return os; }
ostream& operator<<(ostream& os, const path& p) { return os << "pressure: " << p.pr << " valves: " << p.valves << " opened: " << p.opened; }
ostream& operator<<(ostream& os, const list<path>& p) { r::copy(p, ostream_iterator<path>(os, "\n")); return os; }
ostream& operator<<(ostream& os, const map<string, int>& v) { for (auto& p : v ) os << "[" << p.first << "," << p.second << "], "; return os; }

int solve1(vector<valve>& input)
{
	map<string, valve*> vmap;
	map<string, int> visited_closed;
	map<string, int> visited_open;
	for (auto& v : input)
	{
		vmap[v.name] = &v;
		visited_closed[v.name] = -1;
		visited_open[v.name] = -1;
	}
	path start{ 0, {"AA"}, {}};
	list<path> cur{ start };
	for (int i = 1; i <= 30; ++i)
	{
		//cout << "minute: " << i << endl << "visited closed: " << visited_closed << endl << "visited open: " << visited_open << endl << cur << endl;
		list<path> next;
		for (auto& p : cur)
		{
			if (!p.opened.contains(p.valves.back()))
			{
				int pr = p.pr + vmap[p.valves.back()]->pr * (30 - i);

				auto tail = vmap[p.valves.back()];

				if (auto it = visited_open.find(tail->name); it->second < pr)
				{
					it->second = pr;
					auto add_p(p);
					add_p.opened.insert(tail->name);
					add_p.pr = pr;
					add_p.opened.insert(tail->name);
					next.push_back(add_p);
				}
			}
			for (auto tail = vmap[p.valves.back()]; auto& t : tail->valves)
			{
				auto& add_v = vmap[t];
				auto& visited = p.opened.contains(add_v->name) ? visited_open : visited_closed;
				if (auto it = visited.find(add_v->name); it->second < p.pr)
				{
					it->second = p.pr;
					auto add_p(p);
					add_p.valves.push_back(add_v->name);
					add_p.pr = p.pr;
					next.push_back(add_p);
				}
			}
		}
		cur = next;
	}
	return r::max_element(visited_open, [](auto& v1, auto& v2) {return v1.second < v2.second; })->second;
}

string to_key(string v1, bool o1, string v2, bool o2)
{ return v1 < v2 || v1 == v2 && (!o1 || o2) ? v1 + (o1 ? "1" : "0") + v2 + (o2 ? "1" : "0") : v2 + (o2 ? "1" : "0") + v1 + (o1 ? "1" : "0"); }
struct path2
{
	int pr;
	pair<string, string> v;
	set<string> opened;
};
int solve2(vector<valve>& input)
{
	map<string, valve*> vmap;
	map<string, int> visited;
	for (auto& v : input)
		vmap[v.name] = &v;
	for (auto& v : vmap)
		for (auto& v2 : vmap)
			visited[to_key(v.first, false, v2.first, false)] = visited[to_key(v.first, false, v2.first, true)] = visited[to_key(v.first, true, v2.first, true)] = visited[to_key(v.first, true, v2.first, false)] = -1;
	path2 start{ 0, {"AA", "AA"}, {} };
	list<path2> cur{ start };
	for (int i = 1; i <= 26; ++i)
	{
		//cout << "minute: " << i << endl << "visited closed: " << visited_closed << endl << "visited open: " << visited_open << endl << cur << endl;
		list<path2> next;
		for (auto& p : cur)
		{
			if (!p.opened.contains(p.v.first))
			{
				for (auto& t2 : vmap[p.v.second]->valves)
				{
					auto tail = vmap[p.v.first];
					int pr = p.pr + tail->pr * (26 - i);
					auto it = visited.find(to_key(p.v.first, true, t2, p.opened.contains(t2)));
					if (it->second < pr)
					{
						auto add_p(p);
						add_p.v.second = t2;
						add_p.pr = pr;
						next.push_back(add_p);
					}
				}
			}
			if (!p.opened.contains(p.v.second))
			{
				for (auto& t1 : vmap[p.v.first]->valves)
				{
					auto tail = vmap[p.v.second];
					int pr = p.pr + tail->pr * (26 - i);
					auto it = visited.find(to_key(t1, p.opened.contains(t1), p.v.second, true));
					if (it->second < pr)
					{
						auto add_p(p);
						add_p.v.first = t1;
						add_p.pr = pr;
						next.push_back(add_p);
					}
				}
			}
			for (auto& t1 : vmap[p.v.first]->valves)
			{
				for (auto & t2 : vmap[p.v.second]->valves)
				{
					auto it = visited.find(to_key(t1, p.opened.contains(t1), t2, p.opened.contains(t2)));
					if (it->second < p.pr)
					{
						if (it->second < p.pr)
							it->second = p.pr;
						auto add_p(p);
						add_p.v.first = t1;
						add_p.v.second = t2;
						add_p.pr = p.pr;
						next.push_back(add_p);
					}
				}
			}
		}
		cur = next;
	}
	return r::max_element(visited, [](auto& v1, auto& v2) {return v1.second < v2.second; })->second;
}

int main()
{
	test(); return 0;
	ifstream is("Day16.txt");
	istream_iterator<valve> start(is), end;
	vector<valve> input(start, end);

	cout << "Day16 answer1: " << solve1(input) << endl;
	cout << "Day16 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II)");
	istream_iterator<valve> start(is), end;
	vector<valve> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
