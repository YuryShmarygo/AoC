#include "../common.h"

struct connection
{
	string name;
	vector<string> conn;
};
connection read_connection(string line)
{
	connection c;
	smatch m;
	if (regex_match(line, m, regex(R"((\w+):(.+))")))
	{
		c.name = m[1].str();
		string conn_str = m[2].str();
		smatch comp;
		while (regex_search(conn_str, comp, regex(R"(\s*(\w+))")))
		{
			c.conn.push_back(comp[1].str());
			conn_str = comp.suffix();
		}
	}
	return c;
}
vector<connection> read_connections(istream& is)
{
	vector<connection> res;
	string line;
	while (getline(is, line) && !line.empty())
		res.push_back(read_connection(line));
	return res;
}
typedef map<string, set<string>> c_map;
ostream& operator<<(ostream& os, const set<string>& s) { r::copy(s, ostream_iterator<string>(os, " ")); return os; }
ostream& operator<<(ostream& os, const pair<string, set<string>>& p) { return os << p.first << ": " << p.second; }
ostream& operator<<(ostream& os, const map<string, set<string>>& cm) {for (auto& c : cm) os << c.first << ' ', r::copy(c.second, ostream_iterator<string>(os, " ")), os << endl; return os;}
long long check_remove(map<string, set<string>> cm, vector<pair<string, string>> to_remove)
{
	for (auto& r : to_remove)
	{
		cm[r.first].erase(r.second);
		cm[r.second].erase(r.first);
	}
	bool inserted = true;
	while (inserted)
	{
		inserted = false;
		for (auto& c : cm)
		{
			set<string> to_add;
			for (auto& s : c.second)
				to_add.insert(cm[s].begin(), cm[s].end());
			if (to_add.size() == cm.size())
				return 0;
			auto cs = c.second.size();
			c.second.insert(to_add.begin(), to_add.end());
			inserted = inserted || c.second.size() > cs;
		}
	}
	if (cm.begin()->second.size() == cm.size())
		return 0;
	long long res = 1;
	set<string> counted;
	for (auto& c : cm)
	{
		if (!counted.contains(c.first) && c.second.size() != cm.size())
		{ 
			res *= c.second.size();
			counted.insert(c.first);
			for (auto& s : c.second)
				counted.insert(s);
		}
	}
	return res;
}
long long solve1(const vector<connection>& input)
{
	map<string, set<string>> cm;
	for (auto& c : input)
	{
		for (auto& s : c.conn)
		{
			cm[c.name].insert(s);
			cm[s].insert(c.name);
		}
	}
	map<pair<string, string>, set<string>> cl;
	for (auto& c : cm)
		for (auto& s : c.second)
			if (c.first < s)
			{
				cl[{c.first, s}].insert(c.second.begin(), c.second.end());
				cl[{c.first, s}].insert(cm[s].begin(), cm[s].end());
			}
	//set<tuple<pair<string, string>, pair<string, string>, pair<string, string>>> checked;
	while (true)
	{
		vector<pair<pair<string, string>, set<string>>> clv(cl.begin(), cl.end());
		r::sort(clv, [](auto& p1, auto& p2) { return p1.second.size() > p2.second.size(); });
		int maxn = r::count_if(clv, [&](auto& c) { return c.second.size() >= clv[3].second.size(); });
		set<tuple<int, int, int>> checked;
		for (int iend = 2; iend < clv.size() && iend < maxn; ++iend)
		{
			for (int i = 0; i <= iend; ++i)
				for (int j = i + 1; j <= iend; ++j)
					for (int k = j + 1; k <= iend; ++k)
					{
						if (!checked.contains({ i,j,k }))
						//if (!checked.contains({ clv[i].first, clv[j].first, clv[k].first }))
						{
							long long res = check_remove(cm, { clv[i].first, clv[j].first, clv[k].first });
							if (res > input.size())
								return res;
							checked.insert({ i, j, k });
							//checked.insert({ clv[i].first, clv[j].first, clv[k].first });
						}
					}
		}
		for (auto& c : cl)
		{
			set<string> to_add;
			for (auto& s : c.second)
			{
				for (auto& s2 : cm[s])
					to_add.insert(s2);
			}
			c.second.insert(to_add.begin(), to_add.end());
		}
	}
	return 0;
}
int main()
{
	test(); //return 0;
	ifstream is("Day25.txt");
	vector<connection> input = read_connections(is);

	auto t_start = high_resolution_clock::now();
	cout << "Day25 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(jqt: rhn xhk nvd
rsh: frs pzl lsr
xhk: hfx
cmg: qnr nvd lhk bvb
rhn: xhk bvb hfx
bvb: xhk hfx
pzl: lsr hfx nvd
qnr: nvd
ntq: jqt hfx bvb xhk
nvd: lhk
lsr: lhk
rzs: qnr cmg lsr rsh
frs: qnr lhk lsr)";

	auto is = stringstream(test1);
	vector<connection> input = read_connections(is);

	cout << "Test1: " << solve1(input) << endl;
}
