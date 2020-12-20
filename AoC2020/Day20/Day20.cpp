#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

template<int N> int mod_n(int n)
{ return n < 0 ? N + n : n >= N ? n - N : n; }

struct Conv
{
	function<int(int x, int y)> x;
	function<int(int x, int y)> y;
	static Conv VFlip(size_t size)
	{ return Conv{ [](int x, int y) {return x; }, [size](int x, int y) {return (int)size - 1 - y; } }; }
	static Conv HFlip(size_t size)
	{ return Conv{ [size](int x, int y) {return (int)size - 1 - x; }, [size](int x, int y) {return y; } }; }
	static Conv Right(size_t size)
	{ return Conv{ [size](int x, int y) {return (int)size - 1 - y;}, [](int x, int y){return x; } }; }
	static Conv Left(size_t size)
	{ return Conv{ [](int x, int y) {return y;}, [size](int x, int y){return (int)size - 1 - x; } }; }
	static Conv Full(size_t size)
	{ return Conv{ [size](int x, int y) {return (int)size - 1 - x; }, [size](int x, int y){ return (int)size - 1 - y; } }; }
};
enum EFlip { Ver, Hor};
vector<string> flip(const vector<string>& m, EFlip ftype)
{
	static map<EFlip, function<Conv(size_t size)>> flips{ {Ver, Conv::VFlip}, {Hor, Conv::HFlip} };
	auto conv = flips[ftype](m.size());
	vector<string> res(m);
	for (int y = 0; y < m.size(); ++y)
		for (int x = 0; x < m[y].size(); ++x)
			res[conv.y(x,y)][conv.x(x,y)] = m[y][x];
	return res;
}
enum ERot {Right, Full, Left};
vector<string> rotate(const vector<string>& m, ERot rtype)
{
	static map<ERot, function<Conv(size_t size)>> rotations{ {Right, Conv::Right}, {Left, Conv::Left}, {Full, Conv::Full} };
	auto conv = rotations[rtype](m.size());
	vector<string> res(m);
	for (int y = 0; y < m.size(); ++y)
		for (int x = 0; x < m[y].size(); ++x)
			res[conv.y(x,y)][conv.x(x,y)] = m[y][x];
	return res;
}
typedef vector<string> Image;
struct Tile
{
	int id;
	vector<string> edges = vector<string>(4,string(10,' '));
	vector<int> adj = vector<int>(4,0);
	vector<string> im;
	enum EEdge {top, right, bottom, left};
	string edge(EEdge etype) const
	{ return edges[etype]; }
	void init_edges()
	{
		edges[0] = im[0];
		edges[2] = im[im.size() - 1];
		for (int i = 0; i < im.size(); ++i)
		{
			edges[3][i] = im[i][0];
			edges[1][i] = im[i][im.size() - 1];
		}
	}
	Image image() const
	{
		Image res(im.size() - 2, string(im[0].size() - 2, '.'));
		for (int i = 1; i < im.size() - 1; ++i)
			res[i - 1] = im[i].substr(1, im[i].size() - 1);
		return res;
	}
	void rotate(ERot rtype)
	{
		static vector<int> shifts{ /*Right*/ 1, /*Full*/ 2, /*Left*/ -1 };
		Tile t(*this);
		for (int i = 0; i < 4; ++i)
			adj[mod_n<4>(i + shifts[(int)rtype])] = t.adj[i];
		im = ::rotate(im, rtype);
		init_edges();
	}
	void flip(EFlip ftype)
	{
		Tile t(*this);
		if (ftype == Ver)
			swap(adj[0], adj[2]);
		else
			swap(adj[1], adj[3]);
		im = ::flip(im, ftype);
		init_edges();
	}
	ERot rot_diff(EEdge cur, EEdge targ)
	{
		return (ERot)mod_n<3>(targ - cur);
	}
	void position(map<EEdge, int> p_edges)
	{
		auto it = p_edges.begin();
		if (it->second != adj[it->first])
		{
			auto itc = find(adj.begin(), adj.end(), it->second);
			rotate((ERot)mod_n<3>((int)(it->first - (itc - adj.begin()))));
		}
		it = next(it);
		if (it != p_edges.end() && it->second != adj[it->first])
			flip((EFlip)mod_n<2>(it->first));
	}
};
typedef map<int, Tile> Tiles;

istream& operator>>(istream& is, Tile& tile)
{
	string line;
	getline(is, line);
	smatch m;
	regex_search(line, m, regex("(\\d+)"));
	tile.id = stoi(m[1]);
	while (is && getline(is, line) && !line.empty())
		tile.im.push_back(line);
	tile.init_edges();
	return is;
}
istream& operator>>(istream& is, Tiles& tiles)
{
	while (is)
	{
		Tile tile; is >> tile;
		tiles[tile.id] = tile;
	}
	return is;
}
int find_adj(const Tiles& tiles, int cur, const string& edge)
{
	string redge(edge.rbegin(), edge.rend());
	for (auto& t : tiles)
	{
		if (t.first == cur)
			continue;
		auto it = find_if(t.second.edges.begin(), t.second.edges.end(), [&](auto& e) {return e == edge || e == redge; });
		if (it != t.second.edges.end())
			return t.first;
	}
	return 0;
}
bool match(const vector<string>& image, const vector<string>& mon, int y, int x)
{
	bool matched = true;
	for (int i = 0; i < mon.size() && matched; ++i)
		for (int j = 0; j < mon[i].size() && matched; ++j)
			if (mon[i][j] == '#' && image[y + i][x + j] != '#')
				matched = false;
	return matched;
}
int count_sea(const vector<string>& c_image, const vector<string>& mon)
{
	vector<string> image(c_image);
	bool found = false;
	for (int i = 0; i < image.size() - mon.size(); ++i)
		for (int j = 0; j < image[i].size() - mon[0].size(); ++j)
			if (match(image, mon, i, j))
			{
				found = true;
				for (int y = 0; y < mon.size(); ++y)
					for (int x = 0; x < mon[y].size(); ++x)
						if (mon[y][x] == '#')
							image[i + y][j + x] = 'O';
			}
	return !found ? 0 : accumulate(image.begin(), image.end(), (int)0, [](int s, auto& l) { return s + (int)count(l.begin(), l.end(), '#'); });
}

int count_adj(const Tiles& tiles, const Tile& tile)
{ return accumulate(tile.edges.begin(), tile.edges.end(), 0, [&](auto sum, auto& e) { return sum + (find_adj(tiles, tile.id, e) > 0 ? 1 : 0); }); }
long long task1(const Tiles& tiles)
{ return accumulate(tiles.begin(), tiles.end(), 1LL, [&](auto s, auto& t) { return s * (count_adj(tiles, t.second) == 2 ? t.first : 1); }); }

int task2(const Tiles& input)
{
	Tiles tiles(input);
	vector<string> mon = {
"                  # ",
"#    ##    ##    ###",
" #  #  #  #  #  #   " };
	int size = (int)sqrt(tiles.size());
	int tile_size = (int)tiles.begin()->second.im.size() - 2;
	int im_size = size * tile_size;
	vector<string> image(im_size, string(im_size, ' '));

	vector<vector<int>> pos(size, vector<int>(size, 0));
	for (auto& t : tiles)
	{
		for (int i = 0; i < 4; ++i)
		{
			auto edge = t.second.edge((Tile::EEdge)i);
			t.second.adj[i] = find_adj(tiles, t.first, edge);
		}
	}
	auto it = find_if(tiles.begin(), tiles.end(), [](auto& t) { return 2 == count(t.second.adj.begin(), t.second.adj.end(), 0); });
	it->second.position({ { Tile::EEdge::top, 0 }, { Tile::EEdge::left, 0 } });
	/*
	if (it->second.adj[0] != 0 && it->second.adj[1] != 0)
		it->second.rotate(Right);
	else if (it->second.adj[2] != 0 && it->second.adj[3] != 0)
		it->second.rotate(Left);
	else if (it->second.adj[3] != 0 && it->second.adj[0] != 0)
		it->second.rotate(Full);
		*/
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			pos[i][j] = it->first;
			if (j != size - 1)
			{
				it = tiles.find(it->second.adj[1]);
				it->second.position({ {Tile::EEdge::left, pos[i][j]}, {Tile::EEdge::top, i == 0 ? 0 : pos[i - 1][j + 1]} });
			}
			else if (i != size - 1)
			{
				it = tiles.find(pos[i][0]);
				it = tiles.find(it->second.adj[2]);
				it->second.position({ {Tile::EEdge::left, 0},{Tile::EEdge::top, pos[i][0]} });
			}
		}
	}

	for (int i = 0; i < pos.size(); ++i)
		for (int j = 0; j < pos.size(); ++j)
		{
			auto ti = tiles[pos[i][j]].image();
			for (int y = 0; y < ti.size(); ++y)
				for (int x = 0; x < ti[y].size(); ++x)
					image[i * tile_size + y][j * tile_size + x] = ti[y][x];
		}

	static vector<EFlip> flips{ Ver, Hor };
	static vector<ERot> rotations{ Right, Full, Left };
	int res = 0;
	while (true)
	{
		if ((res = count_sea(image, mon)) > 0)
			return res;
		for (auto& r : rotations)
			if ((res = count_sea(rotate(image, r), mon)) > 0)
				return res;
		for (auto& f : flips)
		{
			auto copy = flip(image, f);
			if ((res = count_sea(copy, mon)) > 0)
				return res;
			for (auto& r : rotations)
				if ((res = count_sea(rotate(copy, r), mon)) > 0)
					return res;
		}
	}
	return 0;
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day20.txt");
	Tiles input; is >> input;
	//cout << "Read " << input.size() << " tiles" << endl;

	cout << "Day20 Answer1: " << task1(input) << endl;
	cout << "Day20 Answer2: " << task2(input) << endl;
}

void Test()
{
	stringstream test(R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...
)");
	Tiles input; test >> input;
	cout << "Test 1: " << task1(input) << endl;
	cout << "Test 2: " << task2(input) << endl;
}