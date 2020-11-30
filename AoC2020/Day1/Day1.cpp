#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
	ifstream is("Day1.txt");
	istream_iterator<long> start(is), end;
	vector<long> input(start, end);
	cout << "Read " << input.size() << " numbers" << endl;

	long sum = 0;
	for_each(input.begin(), input.end(), [&](auto i) {sum += i / 3 - 2; });
	cout << "Day1 Answer1: " << sum << endl;

	sum = 0;
	for_each(input.begin(), input.end(), [&](auto i)
		{
			long fuel = i / 3 - 2;
			while (fuel > 0)
			{
				sum += fuel;
				fuel = fuel / 3 - 2;
			}
		});
	cout << "Day1 Answer2: " << sum << endl;
}
