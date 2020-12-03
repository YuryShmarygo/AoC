#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int count_trees(const vector<string>& input, int dx, int dy)
{
	int x = 0;
	int count = 0;
	int width = input[0].size();
	for (int y = 0; y < input.size(); y += dy)
	{
		if (input[y][x] == '#')
			++count;
		x += dx;
		if (x >= width)
			x -= width;
	}
	return count;
}

int main()
{
	ifstream is("Day3.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " lines." << endl;

	cout << "Day3 Answer1: " << count_trees(input, 3, 1) << endl;

	vector<pair<int, int>> shifts{ {1,1}, {3,1}, {5,1}, {7,1}, {1,2} };
	long long count = 1;
	for (auto shift : shifts)
		count *= count_trees(input, shift.first, shift.second);

	cout << "Day3 Answer2: " << count << endl;
}
