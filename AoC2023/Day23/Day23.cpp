#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y }; }
point& operator+=(point& p1, const point& p2) { p1.x += p2.x; p1.y += p2.y; return p1; }
struct path_t
{
	point pos;
	set<point> visited;
};
bool operator<(const path_t& p1, const path_t& p2) { return p1.visited.size() > p2.visited.size(); }
point find_s(const vector<string>& input)
{ for (int x = 0; x < input[0].size(); ++x) if (input[0][x] == '.') { return { x, 0 }; }}
point find_e(const vector<string>& input)
{ for (int x = 0; x < input[0].size(); ++x) if (input[input.size()-1][x] == '.') { return {x, (int)input.size() - 1 }; } }
vector<point> all_dir{ {-1,0},{0,-1},{1,0},{0,1} };
map<char, point> slope_dir{ {'>',{1, 0}}, {'<', {-1,0}}, {'^', {0,-1}}, {'v', {0,1}} };
bool can_move(const vector<string>& input, const point& p) { return p.x >= 0 && p.x < input[0].size() && p.y >= 0 && p.y < input.size() && input[p.y][p.x] != '#'; }
vector<point> get_all_moves(const vector<string>& input, const path_t& path)
{
	vector<point> dest;
	char cur = input[path.pos.y][path.pos.x];
	for (auto& d : all_dir)
	{
		point next_p = path.pos + d;
		if (!path.visited.contains(next_p) && can_move(input, next_p))
			dest.push_back(next_p);
	}
	return dest;
}

vector<point> get_slope_moves(const vector<string>& input, const path_t& path)
{
	char cur = input[path.pos.y][path.pos.x];
	if (cur == '.')
		return get_all_moves(input, path);
	point next_p = path.pos + slope_dir[cur];
	if (!path.visited.contains(next_p) && can_move(input, next_p))
		return { next_p };
	return vector<point>();
}

tuple<vector<vector<int>>, vector<vector<vector<point>>>> get_splits(const vector<string>& input, bool can_climb = false)
{
	vector<vector<int>> splits(input.size(), vector<int>(input[0].size(), 0));
	vector<vector<vector<point>>> moves(input.size(), vector<vector<point>>(input[0].size(), vector<point>()));
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[y].size(); ++x)
		{
			if (input[y][x] == '#')
				continue;
			auto dest = can_climb ? get_all_moves(input, { {x, y} }) : get_slope_moves(input, { {x, y} });
			for (auto& d : dest)
			{
				++splits[d.y][d.x];
				moves[y][x].push_back(d);
			}
		}
	}
	return { splits, moves };
}

long long solve1(const vector<string>& input, bool can_climb = false)
{
	/*
	point p_start = find_s(input);
	point p_dest = find_e(input);
	auto [splits_map, moves_map] = get_splits(input, can_climb);
	list<path_t> paths;
	map<point, path_t> longest;
	longest[p_start] = path_t({ p_start, {p_start} });
	while (!paths.empty() || longest.size() > 1 || !longest.empty() && longest.begin()->first != p_dest)
	{
		for (auto itp = paths.begin(); itp != paths.end(); )
		{
			auto& p = itp->pos;
			auto dest = moves_map[p.y][p.x];
			auto cur_moves = itp->visited.size();
			for (auto itd = dest.begin(); itd != dest.end(); ++itd)
			{
				if (itp->visited.contains(*itd))
					continue;
				if (*itd == p_dest || splits_map[p.y][p.x] > 2 && splits_map[itd->y][itd->x] <= 2)
				{
					auto& best = longest[*itd];
					if (itp->visited.size() + 1 > best.visited.size())
					{
						auto visited = itp->visited;
						visited.insert(*itd);
						best = { *itd, visited };
					}
					continue;
				}
				auto iti = itp;
				if (next(itd) != dest.end())
					iti = paths.insert(itp, *itp);

				iti->pos = *itd;
				iti->visited.insert(*itd);

			}
			if (cur_moves == itp->visited.size())
				itp = paths.erase(itp);
			else
				++itp;
		}
		if (paths.empty())
		{
			auto itb = longest.begin();
			for (; itb != longest.end() && itb->first == p_dest; ++itb);
			if (itb != longest.end())
			{
				paths.push_back(move(itb->second));
				longest.erase(itb);
			}
		}
	}
	return longest.begin()->second.visited.size() - 1;
	*/
	point p_start = find_s(input);
	point p_dest = find_e(input);
	path_t best;
	list<path_t> paths({ { p_start, {p_start} } });
	while (!paths.empty())
	{
		for (auto itp = paths.begin(); itp != paths.end(); )
		{
			if (itp->pos == p_dest)
				best = *itp;
			int cur_moves = itp->visited.size();
			auto dest = can_climb ? get_all_moves(input, *itp) : get_slope_moves(input, *itp);
			for (auto itd = dest.begin(); itd != dest.end(); ++itd)
			{
				auto iti = itp;
				if (next(itd) != dest.end())
					iti = paths.insert(itp, *itp);

				iti->pos = *itd;
				iti->visited.insert(*itd);
			}
			if (cur_moves == itp->visited.size())
				itp = paths.erase(itp);
			else
				++itp;
		}
	}
	return best.visited.size() - 1;
	
	/*
	vector<vector<map<point, int>>> visited(input.size(), vector<map<point, int>>(input[0].size(), map<point, int>()));
	point p_start = find_s(input);
	point p_dest = find_e(input);
	path_t best;
	list<path_t> paths;
	paths.push_back({ p_start, {p_start} });
	while (!paths.empty())
	{
		auto& path = paths.front();
		auto dest = get_moves(input, path, can_climb);
		for (auto& d : dest)
		{
			auto& visit = visited[d.y][d.x];
			if (visit[path.pos] <= path.visited.size())
			{
				visit[path.pos] = path.visited.size();
				path_t new_path(path);
				new_path.pos = d;
				new_path.visited.insert(d);
				if (d == p_dest)
					best = new_path;
				else
					paths.push_back(new_path);
			}
		}
		paths.pop_front();
	}
	return best.visited.size() - 1;
	*/
	/*
	vector<vector<map<point, int>>> visited(input.size(), vector<map<point,int>>(input[0].size(), map<point, int>()));
	point p_start = find_s(input);
	point p_dest = find_e(input);
	priority_queue<path_t> paths;
	paths.push({ p_start, {p_start}});
	while (paths.top().pos != p_dest)
	{
		auto path = paths.top(); paths.pop();
		auto dest = get_moves(input, path);
		for (auto& d : dest)
		{
			auto& visit = visited[d.y][d.x];
			if (visit[path.pos] <= path.visited.size())
			{
				visit[path.pos] = path.visited.size();
				path_t new_path(path);
				new_path.pos = d;
				new_path.visited.insert(d);
				paths.push(new_path);
			}
		}
	}
	return paths.top().visited.size()-1;
	*/
}
//unordered_map<pair<point, point>, path_t> cache;
path_t get_longest(const vector<string>& input, point p_start, point p_dest)
{
	path_t best;
	list<path_t> paths({ { p_start, {p_start} } });
	while (!paths.empty())
	{
		for (auto itp = paths.begin(); itp != paths.end(); )
		{
			if (itp->pos == p_dest)
				best = *itp;
			int cur_moves = itp->visited.size();
			auto dest = get_all_moves(input, *itp);
			for (auto itd = dest.begin(); itd != dest.end(); ++itd)
			{
				auto iti = itp;
				if (next(itd) != dest.end())
					iti = paths.insert(itp, *itp);

				iti->pos = *itd;
				iti->visited.insert(*itd);
			}
			if (cur_moves == itp->visited.size())
				itp = paths.erase(itp);
			else
				++itp;
		}
	}
	return best;
}
path_t get_one_path(const vector<string>& input, point p_start)
{
	path_t path({ p_start, {p_start} });
	while (true)
	{
		auto dest = get_all_moves(input, path);
		if (dest.size() > 1)
			break;
		path.pos = dest[0];
		path.visited.insert(dest[0]);
	}
	return path;
}
struct tracking_p : path_t
{
	int id;
	point p_start;
	point p_end;
	static int next_id() { static int num = 0; return ++num; }
};
/*
vector<tracking_p> get_single_paths(const vector<string>& input, point p_start)
{
	set<point> visited;
	vector<tracking_p> res;
	list<path_t> paths({ { p_start, {p_start} } });
	while (!paths.empty())
	{
		for (auto itp = paths.begin(); itp != paths.end(); )
		{
			if (itp->pos == p_dest)
				best = *itp;
			int cur_moves = itp->visited.size();
			auto dest = get_all_moves(input, *itp);
			for (auto itd = dest.begin(); itd != dest.end(); ++itd)
			{
				auto iti = itp;
				if (next(itd) != dest.end())
					iti = paths.insert(itp, *itp);

				iti->pos = *itd;
				iti->visited.insert(*itd);
			}
			if (cur_moves == itp->visited.size())
				itp = paths.erase(itp);
			else
				++itp;
		}
	}
}
*/
long long solve2(const vector<string>& input)
{
	point p_start = find_s(input);
	point p_dest = find_e(input);
	path_t prefix = get_one_path(input, p_start);
	path_t suffix = get_one_path(input, p_dest);
	
	return prefix.visited.size() - 1 + suffix.visited.size() - 1 + get_longest(input, prefix.pos, suffix.pos).visited.size() - 1;
	//	return solve1(input, true);
}
int main()
{
	test();
	ifstream is("Day23.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day23 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day23 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(#.#####################
#.......#########...###
#######.#########.#.###
###.....#.>.>.###.#.###
###v#####.#v#.###.#.###
###.>...#.#.#.....#...#
###v###.#.#.#########.#
###...#.#.#.......#...#
#####.#.#.#######.#.###
#.....#.#.#.......#...#
#.#####.#.#.#########v#
#.#...#...#...###...>.#
#.#.#v#######v###.###v#
#...#.>.#...>.>.#.###.#
#####v#.#.###v#.#.###.#
#.....#...#...#.#.#...#
#.#########.###.#.#.###
#...###...#...#...#.###
###.###.#.###v#####v###
#...#...#.#.>.>.#.>.###
#.###.###.#.###.#.#v###
#.....###...###...#...#
#####################.#)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
