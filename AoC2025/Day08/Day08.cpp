#include "../common.h"

typedef vector<point3> input_t;
input_t read_input(istream& is) 
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
	{
		point3 p;
		stringstream s(line);
		char c;
		s >> p.x >> c >> p.y >> c >> p.z;
		res.push_back(p);
	}
	return res;
}

double distance(const point3& p1, const point3& p2)
{
	return sqrt(((double)p1.x - p2.x) * (p1.x - p2.x)
		+ ((double)p1.y - p2.y) * (p1.y - p2.y)
		+ ((double)p1.z - p2.z) * (p1.z - p2.z));
}

vector<pair<double, pair<point3, point3>>> distances(const input_t& input)
{
	vector<pair<double, pair<point3, point3>>> distances;
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		for (auto it2 = next(it); it2 != input.end(); ++it2)
		{
			double dist = distance(*it, *it2);
			distances.push_back({ dist, { *it, *it2 } });
		}
	}
	r::sort(distances, [](const auto& p1, const auto& p2) { return p1.first < p2.first; });
	return distances;
}

long long solve1(const input_t& input, int n)
{
	vector<set<point3>> circuits;
	for (auto& p : input)
		circuits.push_back({ p });

	auto dist = distances(input);

	auto it = dist.begin();
	for (int i = 0; i < n; ++i)
	{
		auto itc1 = r::find_if(circuits,[&](const set<point3>& c) { return c.contains(it->second.first); });
		auto itc2 = r::find_if(circuits, [&](const set<point3>& c) { return c.contains(it->second.second); });
		if (itc1 == circuits.end() || itc2 == circuits.end())
			cout << "Error!" << endl;
		if (itc1 != itc2)
		{
			itc1->insert(itc2->begin(), itc2->end());
			circuits.erase(itc2);
		}
		++it;
	}
	vector<pair<int, set<point3>>> b_circuits;
	for (auto& c : circuits)
		b_circuits.push_back({ (int)c.size(),  c });
	sort(b_circuits.begin(), b_circuits.end(), [](const pair<int, set<point3>>& p1, const pair<int, set<point3>>& p2) { return p1.first > p2.first; });
	return (long long)b_circuits[0].first * b_circuits[1].first * b_circuits[2].first;
}
long long solve2(const input_t& input)
{
	vector<set<point3>> circuits;
	for (auto& p : input)
		circuits.push_back({ p });

	auto dist = distances(input);

	auto it = dist.begin();
	while(circuits.size() > 1)
	{
		auto itc1 = r::find_if(circuits, [&](const set<point3>& c) { return c.contains(it->second.first); });
		auto itc2 = r::find_if(circuits, [&](const set<point3>& c) { return c.contains(it->second.second); });
		if (itc1 != itc2)
		{
			itc1->insert(itc2->begin(), itc2->end());
			circuits.erase(itc2);
		}
		++it;
	}
	--it;
	return ((long long)(it->second.first.x)) * it->second.second.x;
}

int main()
{
	test();
	ifstream is("Day08.txt");
	input_t input = read_input(is);

	cout << "Day8 Answer1: " << solve1(input, 1000) << endl;
	cout << "Day8 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input, 10) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
