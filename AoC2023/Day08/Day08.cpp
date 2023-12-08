#include "../common.h"

struct node
{
	string name;
	string left;
	string right;
};
istream& operator>>(istream& is, node& n) 
{ 
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex(R"((...) = \((...), (...)\))")))
	{
		n.name = m[1].str();
		n.left = m[2].str();
		n.right = m[3].str();
	}
	return is;
}
struct instructions
{
	string istr;
	vector<node> nodes;
};
istream& operator>>(istream& is, instructions& i)
{
	is >> i.istr;
	string line;
	getline(is, line);
	getline(is, line);

	istream_iterator<node> start(is), end;
	i.nodes = vector<node>(start, end);
	return is;
}
map<string, const node*> build_map(const instructions& ins)
{
	map<string, const node*> m;
	for (int i = 0; i < ins.nodes.size(); ++i)
		m[ins.nodes[i].name] = &ins.nodes[i];
	return m;
}
long long solve1(const instructions& ins)
{
	auto m = build_map(ins);
	long long res = 0;
	string cur = "AAA";
	int i = 0;
	while (cur != "ZZZ")
	{
		const node* n = m[cur];
		auto cur_ins = ins.istr[i];
		cur = cur_ins == 'L' ? n->left : n->right;

		i = (i + 1) % ins.istr.size();
		++res;
	}
	return res;
}
bool ends_with(const vector<string>& v, char c)
{
	return r::all_of(v, [c](auto& s) { return s[2] == c; });
}
vector<string> find_a(const instructions& ins)
{
	vector<string> res;
	for (auto& node : ins.nodes)
		if (node.name[2] == 'A')
			res.push_back(node.name);
	return res;
}
struct period
{
	long long prefix;
	long long period;
};
long long lcm(const vector<period>& periods)
{
	long long res = 1;
	for (auto& p : periods)
		res = lcm(res, p.period);
	return res;
}
long long solve2(const instructions& ins)
{
	auto m = build_map(ins);
	vector<string> cur = find_a(ins);
	vector<period> periods(cur.size(), { 0,0 });
	vector<vector<string>> path;
	for (auto& s : cur) path.push_back(vector<string>(1, s));
	int cur_i = 0;
	long long res = 0;
	while (!r::all_of(periods, [](auto& p) {return p.period > 0; }))
	{
		for (int i = 0; i < cur.size(); ++i)
		{
			if (periods[i].period > 0)
				continue;
			auto& name = cur[i];
			const node* n = m[name];
			auto cur_ins = ins.istr[cur_i];
			name = cur_ins == 'L' ? n->left : n->right;
			cur[i] = name;
			if (name[2] == 'Z')
			{
				if (periods[i].prefix == 0)
					periods[i].prefix = res + 1;
				else if (periods[i].period == 0)
					periods[i].period = res + 1 - periods[i].prefix;
			}
			path[i].push_back(name);
		}
		cur_i = (cur_i + 1) % ins.istr.size();
		++res;
	}
	//cout << endl; for (auto& v : path) r::copy(v, ostream_iterator<string>(cout, "->")), cout << endl;
	//cout << endl;  for (auto& p : periods) cout << p.prefix << " " << p.period << endl;
	return lcm(periods);
}
int main()
{
	test();
	ifstream is("Day08.txt");
	instructions ins;
	is >> ins;

	auto t_start = high_resolution_clock::now();
	cout << "Day8 Answer1: " << solve1(ins) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day8 Answer2: " << solve2(ins) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";
	auto test2 = R"(LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ))";
	auto test3 = R"(LR

11A = (11B, XXX)
11B = (XXX, 11Z)
11Z = (11B, XXX)
22A = (22B, XXX)
22B = (22C, 22C)
22C = (22Z, 22Z)
22Z = (22B, 22B)
XXX = (XXX, XXX))";

	auto is = stringstream(test1);
	instructions ins;
	is >> ins;

	cout << "Test1: " << solve1(ins) << endl;

	is = stringstream(test2);
	is >> ins;
	cout << "Test2: " << solve1(ins) << endl;

	is = stringstream(test3);
	is >> ins;
	cout << "Test3: " << solve2(ins) << endl;
}
