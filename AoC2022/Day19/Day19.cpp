#include "../common.h"

enum res_type
{
	ore,
	clay,
	obsidian,
	geode
};
typedef map<res_type, int> build_cost;
struct blue_print
{
	int num;
	map<res_type, build_cost> bc;
};
istream& operator>>(istream& os, blue_print& bp)
{
	bp.bc.clear();
	string line;
	if (!getline(os, line) || line.empty()) return os;
	smatch m;
	regex_match(line, m, regex("Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian."));
	bp.num = stoi(m[1]);
	bp.bc[ore] = { {ore, stoi(m[2])} };
	bp.bc[clay] = { {ore, stoi(m[3])} };
	bp.bc[obsidian] = { {ore, stoi(m[4])}, {clay, stoi(m[5])}};
	bp.bc[geode] = { {ore, stoi(m[6])},{obsidian, stoi(m[7])} };
	return os;
}
struct requirement
{
	res_type res;
	int demand;
	bool operator<(const requirement& req) const { return res < req.res; }
};
void collect(const map<res_type, int>& robots, map<res_type, int>& resources)
{
	for (auto& r : robots)
		resources[r.first] += r.second;
}
priority_queue<requirement> rebalance(const map<res_type, int>& robots, const map<res_type, int>& resources, priority_queue<requirement>& req)
{
	priority_queue<requirement> res;
	while (res.size() < 4)
	{
		auto r = req.top();
		req.pop();
	}
	return res;
}
int build(const blue_print& bp, int steps)
{
	map<res_type, int> resources;
	map<res_type, int> robots;
	priority_queue<requirement> demand;
	demand.push({ geode, steps });
	for (int i = 0; i < steps; ++i)
	{
		collect(robots, resources);
		priority_queue<requirement> rebalance;


	}
	return 1;
}
int solve1(const vector<blue_print>& input, int steps)
{
	map<res_type, int> resources;
	map<res_type, int> robots;
	priority_queue<requirement> demand;
	for (int i = 0; i < steps; ++i)
	{
		
	}
	return 1;
}
int solve2(const vector<blue_print>& input)
{
	return 2;
}

int main()
{
	test();
	ifstream is("Day19.txt");
	istream_iterator<blue_print> start(is), end;
	vector<blue_print> input(start, end);

	cout << "Day19 answer1: " << solve1(input, 26) << endl;
	cout << "Day19 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.)");
	istream_iterator<blue_print> start(is), end;
	vector<blue_print> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input,26) << endl;
	cout << "test2: " << solve2(input) << endl;
}
