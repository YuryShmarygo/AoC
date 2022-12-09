#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
int visiable(vector<string>& input)
{
	set<point> vis;
	int count = (int)input.size() * 2 + ((int)input[0].size() - 2) * 2;
	for (size_t i = 1; i < input.size()-1; ++i)
	{
		char cur = input[i][0];
		for (size_t j = 1; j < input[i].size() - 1; ++j)
		{
			if (input[i][j] > cur)
				vis.insert(point{(int)j,(int)i});
			cur = max(cur,input[i][j]);
		}
	}
	for (size_t i = 1; i < input.size() - 1; ++i)
	{
		char cur = input[i][input[i].size() - 1];
		for (size_t j = input[i].size() -2; j >= 1; --j)
		{
			if (input[i][j] > cur)
				vis.insert(point{ (int)j,(int)i });
			cur = max(cur,input[i][j]);
		}
	}
	for (size_t i = 1; i < input[0].size() - 1; ++i)
	{
		char cur = input[0][i];
		for (size_t j = 1; j < input.size() - 1; ++j)
		{
			if (input[j][i] > cur)
				vis.insert(point{ (int)i,(int)j });
			cur = max(cur,input[j][i]);
		}
	}
	for (size_t i = input[0].size() - 2; i >= 1; --i)
	{
		char cur = input[input.size() - 1][i];
		for (size_t j = input.size() - 2; j >= 1; --j)
		{
			if (input[j][i] > cur)
				vis.insert(point{ (int)i,(int)j });
			cur = max(cur,input[j][i]);
		}
	}
	return count + (int)vis.size();
}
int highest(vector<string>& input)
{
	vector<vector<int>> scores(input.size(),vector<int>(input[0].size()));
	for (int i = 0; i < (int)input.size(); ++i)
		for (int j = 0; j < (int)input[0].size(); ++j)
		{
			int score = 1;
			int count = 0;
			for (int y = i - 1; y >= 0; --y)
			{
				++count;
				if (input[y][j] >= input[i][j])
					break;
			}
			score *= count;
			count = 0;
			for (int y = i + 1; y < (int)input[0].size(); ++y)
			{
				++count;
				if (input[y][j] >= input[i][j])
					break;
			}
			score *= count;
			count = 0;
			for (int x = j - 1; x >= 0; --x)
			{
				++count;
				if (input[i][x] >= input[i][j])
					break;
			}
			score *= count;
			count = 0;
			for (int x = j + 1; x < (int)input.size(); ++x)
			{
				++count;
				if (input[i][x] >= input[i][j])
					break;
			}
			score *= count;
			count = 0;
			scores[i][j] = score;
		}
	vector<int> max_r;
	for (auto r : scores)
		max_r.push_back(r::max(r));

	return r::max(max_r);
}
int main()
{
	test();
	ifstream is("Day8.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	
	cout << "Day8 answer1: " << visiable(input) << endl;
	cout << "Day8 answer2: " << highest(input) << endl;
}
void test()
{
	stringstream is(R"(30373
25512
65332
33549
35390)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "test1: " << visiable(input) << endl;
	cout << "test2: " << highest(input) << endl;
}
