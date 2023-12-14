#include "../common.h"

void move_n(vector<string>& p)
{
	for (int x = 0; x < p[0].size(); ++x)
	{
		int y = 0;
		while (y < p.size())
		{
			for (; y < p.size() && p[y][x] != '.'; ++y);
			if (y < p.size())
			{
				int ys = y; for (; ys < p.size() && p[ys][x] == '.'; ++ys);
				if (ys < p.size() && p[ys][x] == 'O')
					swap(p[y][x], p[ys][x]);
				++y;
			}
		}
	}
}
void move_e(vector<string>& p)
{
	for (int y = 0; y < (int)p.size(); ++y)
	{
		int x = (int)p[y].size() - 1;
		while (x >= 0)
		{
			for (; x >= 0 && p[y][x] != '.'; --x);
			if (x >= 0)
			{
				int xs = x; for (; xs >= 0 && p[y][xs] == '.'; --xs);
				if (xs >= 0 && p[y][xs] == 'O')
					swap(p[y][x], p[y][xs]);
				--x;
			}
		}
	}
}
void move_s(vector<string>& p)
{
	for (int x = (int)p[0].size() - 1; x >= 0; --x)
	{
		int y = (int)p.size() - 1;
		while (y >= 0)
		{
			for (; y >= 0 && p[y][x] != '.'; --y);
			if (y >= 0)
			{
				int ys = y; for (; ys >= 0 && p[ys][x] == '.'; --ys);
				if (ys >= 0 && p[ys][x] == 'O')
					swap(p[y][x], p[ys][x]);
				--y;
			}
		}
	}
}
void move_w(vector<string>& p)
{
	for (int y = 0; y < p.size(); ++y)
	{
		int x = 0;
		while (x < p[y].size())
		{
			for (; x < p[y].size() && p[y][x] != '.'; ++x);
			if (x < p[y].size())
			{
				int xs = x; for (; xs < p[y].size() && p[y][xs] == '.'; ++xs);
				if (xs < p[y].size() && p[y][xs] == 'O')
					swap(p[y][x], p[y][xs]);
				++x;
			}
		}
	}
}

void tilt(vector<string>& p)
{
	move_n(p);
}
long long calc(const vector<string> p)
{
	long long res = 0;
	for (int y = 0; y < p.size(); ++y)
		for (int x = 0; x < p[y].size(); ++x)
			if (p[y][x] == 'O')
				res += p.size() - y;
	return res;
}
long long solve1(vector<string> p)
{
	tilt(p);
	return calc(p);
}
void spin(vector<string>& p)
{
	move_n(p); move_w(p); move_s(p); move_e(p);
}
bool operator==(const vector<string>& p1, const vector<string>& p2)
{
	bool res = true;
	for (int y = 0; y < p1.size() && res; ++y)
		res = res && p1[y] == p2[y];
	return res;
}
long long solve2(vector<string> p)
{
	vector<vector<string>> prev(1, p);
	int m = 0, m2 = 0;
	for (int i = 0; m2 == 0; ++i)
	{
		spin(p);
		prev.push_back(p);
		for (int j = m; j < i; ++j)
			if (p == prev[j])
				if (m == 0)
					m = j;
				else if (p == prev[m])
					m2 = j;
	}
	//cout << m << " " << m2 << endl;
	return calc(prev[m + (1000000000 - m) % (m2-m)]);
}

int main()
{
	test();
	ifstream is("Day14.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day14 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day14 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
	//cout << "Test2: " << solve2(input, 100) << endl;
}
