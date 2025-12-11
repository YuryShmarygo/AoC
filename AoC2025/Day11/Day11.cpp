#include "../common.h"

struct connection
{
	string name;
	vector<string> dest;
};
typedef vector<connection> input_t;
input_t read_input(istream& is) 
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
	{
		connection con;
		smatch m;
		regex_search(line, m, regex(R"((...): )"));
		con.name = m[1].str();
		line = m.suffix();
		while (regex_search(line, m, regex(R"((...) ?)")))
		{
			string str = m[1].str();
			con.dest.push_back(str);
			line = m.suffix();
		}
		res.push_back(con);
	}
	return res;
}

long long solve1(const input_t& input)
{
	map<string, vector<string>> m;
	for (auto& c : input)
		m[c.name] = c.dest;

	typedef vector<string> path_t;
	vector<path_t> paths;
	paths.push_back({ "you" });
	while (!r::all_of(paths, [](const auto& p) { return p.back() == "out"; }))
	{
		vector<path_t> new_paths;
		for (auto& p : paths)
		{
			if (p.back() == "out")
			{
				new_paths.push_back(p);
				continue;
			}
			for (auto& d : m[p.back()])
			{
				path_t np = p;
				np.push_back(d);
				new_paths.push_back(np);
			}
		}
		paths = new_paths;
	}
	return paths.size();
}

map<string, long long> find_ways(map<string, vector<string>>& m, string target)
{
	map<string, vector<string>> rm;
	for (auto& [k, v] : m)
	{
		for (auto& d : v)
			rm[d].push_back(k);
	}
	map<string, long long> out_ways;
	out_ways[target] = 1;
	list<string> to_process;
	to_process.insert(to_process.end(), rm[target].begin(), rm[target].end());
	while (!to_process.empty())
	{
		auto p = to_process.front();
		to_process.pop_front();
		if (r::all_of(m[p], [&](const string& c) { return out_ways.find(c) != out_ways.end(); }))
		{
			out_ways[p] = accumulate(m[p].begin(), m[p].end(), 0LL, [&](long long sum, const string& c) { return sum + out_ways[c]; });
		}
		else
		{
			to_process.push_back(p);
			for (auto& ch : m[p])
				if (out_ways.find(ch) == out_ways.end() && find_if(to_process.begin(), to_process.end(), [&](const auto& pr) { return pr == ch; }) == to_process.end())
					to_process.push_front(ch);
		}
		//if (out_ways.size() < m.size())
			for (auto& pa : rm[p])
				if (out_ways.find(pa) == out_ways.end() && find_if(to_process.begin(), to_process.end(), [&](const auto& pr) { return pr == pa; }) == to_process.end())
					to_process.push_front(pa);
	}
	return out_ways;
}
long long solve2(const input_t& input)
{
	map<string, vector<string>> m;
	for (auto& c : input)
		m[c.name] = c.dest;

	// no way from dac to fft. only way is svr -> fft -> dac -> out
	auto out_ways = find_ways(m, "out");
	auto dac_ways = find_ways(m, "dac");
	auto fft_ways = find_ways(m, "fft");

	return fft_ways["svr"] * dac_ways["fft"] * out_ways["dac"];
}


int main()
{
	//test();
	ifstream is("Day11.txt");
	input_t input = read_input(is);

	cout << "Day11 Answer1: " << solve1(input) << endl;
	cout << "Day11 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg
eee: out
fff: out
ggg: out
hhh: ccc fff iii
iii: out)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	auto test2 = R"(svr: aaa bbb
aaa: fft
fft: ccc
bbb: tty
tty: ccc
ccc: ddd eee
ddd: hub
hub: fff
eee: dac
dac: fff
fff: ggg hhh
ggg: out
hhh: out)";
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test2: " << solve2(input) << endl;
}
