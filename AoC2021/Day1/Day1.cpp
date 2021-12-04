#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

long CountIncrease(const vector<long>& input)
{
	long count = 0;
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		auto it2 = next(it);
		count += it2 != input.end() && *it2 > *it ? 1 : 0;
	}
	return count;
}

long CountSliding(const vector<long>& input)
{
	vector<long> sliding;
	sliding.reserve(input.size() / 3);
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		auto it2 = next(it);
		auto it3 = it2 != input.end() ? next(it2) : input.end();
		if (it2 != input.end() && it3 != input.end())
			sliding.push_back(*it + *it2 + *it3);
	}
	return CountIncrease(sliding);
}

int main()
{
	ifstream is("Day1.txt");
	istream_iterator<long> start(is), end;
	vector<long> input(start, end);
	cout << "Read " << input.size() << " numbers" << endl;

	cout << "Day1 Answer1: " << CountIncrease(input) << endl;
	cout << "Day1 Answer2: " << CountSliding(input) << endl;
}
