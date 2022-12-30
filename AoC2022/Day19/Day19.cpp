#include "../common.h"

using namespace std;

struct blue_print
{
	int num;
	map<string, map<string, int>> cost;
};
istream& operator>>(istream& os, blue_print& bp)
{
	bp.cost.clear();
	string line;
	if (!getline(os, line) || line.empty()) return os;
	smatch m;
	regex_match(line, m, regex(R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)"));
	bp.num = stoi(m[1]);
	bp.cost["ore"] = { {"ore", stoi(m[2])} };
	bp.cost["clay"] = { {"ore", stoi(m[3])} };
	bp.cost["obsidian"] = { {"ore", stoi(m[4])}, {"clay", stoi(m[5])} };
	bp.cost["geode"] = { {"ore", stoi(m[6])},{"obsidian", stoi(m[7])} };
	return os;
}
ostream& operator<<(ostream& os, const map<string, int>& m) { os << "["; for (auto& p : m) os << p.first << ":" << p.second << ","; return os << "]"; }
map<tuple<int, map<string, int>, map<string, int>>, int> cache;
int cache_hit = 0;
map<int, int> geodes_max;
void use(map<string, int>& resources, const map<string, int>& cost)
{ for (const auto& [resource, num] : cost) resources[resource] -= num; }
void add(map<string, int>& resources, const map<string, int>& cost)
{ for (const auto& [resource, num] : cost) resources[resource] += num; }
int demand(int time, map<string, int>& resources, map<string, int>& base_demand, pair<string, map<string, int>>& r)
{
//	cout << "[demand(" << r.first << "): base_demand: " << base_demand[r.first] << " - " << resources[r.first] << ": " << (base_demand[r.first] - resources[r.first]) << "] ";
	//return base_demand[r.first] - accumulate(r.second.begin(), r.second.end(), 0, [&](auto sum, auto& cost) {return sum + resources[cost.first]; });
	return base_demand[r.first] - resources[r.first] * time;
}
int maximize_geodes(int time, map<string, int>& resources, map<string, int>& robots,blue_print& blueprint,map<string, int>& base_demand)
{
//	if (robots["geode"] > 0 && time == 0)
//		cout << "time: " << time << ", resources: " << resources << ", robots: " << robots << ", cache size: " << cache.size() << ", cache hit: " << cache_hit << endl;
	if (robots["geode"] * time + resources["geode"] < geodes_max[time]) return 0;
	if (time <= 0) return resources["geode"];
	if (const auto it = cache.find(make_tuple(time, resources, robots)); it != cache.end()) { ++cache_hit; return it->second; }

	int max_geodes = resources["geode"];

	auto r_priority = [&](pair<string, map<string, int>>& r1, pair<string, map<string, int>>& r2)
	{
//		cout << "r_priority(" << r1.first << "," << r2.first << "): " << (demand(resources, base_demand, r1) < demand(resources, base_demand, r2)) << endl;
		//return demand(time, resources, base_demand, r1) < demand(time, resources, base_demand, r2);
		return base_demand[r1.first] - resources[r1.first] - time * robots[r1.first] < base_demand[r2.first] - resources[r2.first] - time * robots[r2.first];
	};
	//priority_queue<pair<string, map<string, int>>> robot_queue(blueprint.cost.begin(), blueprint.cost.end(), r_compare(resources, base_demand));
	priority_queue<pair<string, map<string, int>>, vector<pair<string, map<string, int>>>, decltype(r_priority)> r_queue(r_priority);
	for (auto& cost : blueprint.cost) if (base_demand[cost.first] - resources[cost.first] - time * robots[cost.first] > 0) r_queue.push(cost);
	for (;!r_queue.empty(); r_queue.pop())
	{
		const auto& [robot_type, cost] = r_queue.top();
		if (r::all_of(cost, [&](auto& rc) { return resources[rc.first] >= rc.second; }))
		{
			use(resources, cost);
			add(resources, robots);
			++robots[robot_type];
//			cout << "time: " << time << " new robot: " << robot_type << ", robots: " << robots << endl;
			max_geodes = max(max_geodes, maximize_geodes(time - 1, resources, robots, blueprint, base_demand));
			add(resources, cost);
			--robots[robot_type];
			use(resources, robots);
		}
	}
	add(resources, robots);
	max_geodes = max(max_geodes, maximize_geodes(time - 1, resources, robots, blueprint, base_demand));
	use(resources, robots);
	/*
	for (const auto& [robotType, cost] : blueprint.cost)
	{
		if (r::all_of(cost, [&](auto& rc) { return resources[rc.first] >= rc.second;}))
		{
			use(resources, cost);
			add(resources, robots);
			++robots[robotType];
//			cout << "time: " << "new robot: " << robotType << ", robots: " << robots << endl;
			max_geodes = max(max_geodes, maximize_geodes(time-1, resources, robots, blueprint, base_demand));
			add(resources, cost);
			--robots[robotType];
			use(resources, robots);
		}
	}*/

	cache[make_tuple(time, resources, robots)] = max_geodes;
	geodes_max[time] = robots["geode"] * time + resources["geode"];
	return max_geodes;
}

int solve1(vector<blue_print>& input, int steps)
{
	int max_geodes = 0;
	for (auto& bp : input)
	{
		map<string, int> resources;
		map<string, int> robots{ {"ore",1} };
/*		map<string, int> base_demand{{"geode", 100000},
			{"obsidian", bp.cost["geode"]["obsidian"]},
			{"clay", bp.cost["geode"]["obsidian"] * bp.cost["obsidian"]["clay"]},
			{"ore", bp.cost["geode"]["ore"] + bp.cost["geode"]["obsidian"] * (bp.cost["obsidian"]["ore"] + bp.cost["obsidian"]["clay"] * bp.cost["clay"]["ore"]) + bp.cost["ore"]["ore"]}};
			*/
		map<string, int> base_demand{ {"geode", 100000},
					{"obsidian", bp.cost["geode"]["obsidian"]},
					{"clay", bp.cost["obsidian"]["clay"] * bp.cost["geode"]["obsidian"]},
					{"ore", bp.cost["geode"]["ore"] + bp.cost["obsidian"]["ore"] + bp.cost["clay"]["ore"] + bp.cost["ore"]["ore"]} };
		max_geodes = max(max_geodes, maximize_geodes(steps, resources, robots, bp, base_demand));
		cout << max_geodes << endl;
	}

	return max_geodes;
}
int solve2(const vector<blue_print>& input)
{
	return 2;
}

int main()
{
	test(); return 0;
	ifstream is("Day19.txt");
	istream_iterator<blue_print> start(is), end;
	vector<blue_print> input(start, end);

	cout << "Day19 answer1: " << solve1(input, 24) << endl;
	cout << "Day19 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.)");
	istream_iterator<blue_print> start(is), end;
	vector<blue_print> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input,24) << endl;
	cout << "test2: " << solve2(input) << endl;
}
