#include "../common.h"

typedef map<string, set<string>> input_t;
tuple<string, string> read_line(string line)
{
	smatch m;
	if (regex_match(line, m, regex(R"(^([^-]+)-(.+)$)")))
		return { m[1].str(), m[2].str() };
	cout << endl << "failed to parse line" << endl;
	return { "","" };
}
input_t read_input(istream& is)
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
	{
		auto [s1, s2] = read_line(line);
		res[s1].insert(s2);
		res[s2].insert(s1);
	}
	return res;
}
typedef set<string> party_t;
vector<party_t> find_triples(const input_t& input)
{
	input_t conn(input);
	set<party_t> res;
	for (auto c : conn)
	{
		auto str1 = c.first;
		for (auto it = c.second.begin(); it != c.second.end(); ++ it)
		{
			auto str2 = *it;
			for (auto it2 = next(it); it2 != c.second.end(); ++it2)
			{
				auto str3 = *it2;
				auto itc = conn.find(str2);
				if (itc != conn.end())
				{
					auto it3 = r::find(itc->second, str3);
					if (it3 != itc->second.end())
					{
						party_t t = { str1,str2,str3 };
						res.insert(t);
					}
				}

			}
		}
	}
	return vector<party_t>(res.begin(), res.end());
}
long long solve1(const input_t& input)
{
	auto triples = find_triples(input);
	long long res = 0;
	for (auto& t : triples)
	{
		if (r::any_of(t,[](auto& s){return s[0]=='t';}))
			++res;
	}
	return res;
}
bool is_connected(string str, const party_t& p, input_t& conn)
{ return r::all_of(p, [&](auto& s){return conn[s].contains(str);}); }
party_t intersect(const party_t& p1, const party_t& p2)
{
	party_t res;
	r::set_intersection(p1, p2, inserter(res, res.begin()));
	return res;
}
long long solve2(const input_t& input)
{
	input_t conn(input);
	int max_conn = r::max_element(conn, [](auto& c1, auto& c2) {return c1.second.size() > c2.second.size(); })->second.size();
	party_t best;
	auto comp_conn = [&](string s1, string s2) {return conn[s1].size() > conn[s2].size(); };
	typedef priority_queue<string, vector<string>, decltype(comp_conn)> cqueue_t;
	cqueue_t compf(comp_conn);
	for (auto& c : conn)
		compf.push(c.first);
	for (; !compf.empty(); compf.pop())
	{
		auto str = compf.top();
		if (conn[str].size() <= best.size())
			continue;
		
	}
	set<party_t> parties;
	for (auto c : conn)
	{
		auto str1 = c.first;
		for (auto it = c.second.begin(); it != c.second.end(); ++it)
		{
			auto str2 = *it;
			for (auto it2 = next(it); it2 != c.second.end(); ++it2)
			{
				auto str3 = *it2;
				auto itc = conn.find(str2);
				if (itc != conn.end())
				{
					auto it3 = r::find(itc->second, str3);
					if (it3 != itc->second.end())
					{
						party_t t = { str1,str2,str3 };
						parties.insert(t);
					}
				}

			}
		}
	}
	//return vector<party_t>(res.begin(), res.end());	
	int res = 0;
	return res;
}

int main()
{
	test();
	ifstream is("Day23.txt");
	input_t input = read_input(is);

	cout << "Day23 Answer1: " << solve1(input) << endl;
	cout << "Day23 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(kh-tc
qp-kh
de-cg
ka-co
yn-aq
qp-ub
cg-tb
vc-aq
tb-ka
wh-tc
yn-cg
kh-ub
ta-co
de-co
tc-td
tb-wq
wh-td
ta-ka
td-qp
aq-cg
wq-ub
ub-vc
de-ta
wq-aq
wq-vc
wh-yn
ka-de
kh-ta
co-tc
wh-qp
tb-vc
td-yn)";


	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1 (expect 7): " << solve1(input) << endl;
	//is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve2(input) << endl;
}
