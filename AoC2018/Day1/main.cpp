#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
	//read input file
	ifstream is("input.txt");
	istream_iterator<long> start(is), end;
	vector<long> input(start, end);

	cout << "Read " << input.size() << " numbers" << endl;

	set<long> results;
	long sum = 0;

	// First task is just sum all numbers and we need to remember intermediate results for second task
	for_each(input.begin(), input.end(), [&](auto i) {sum += i; results.insert(sum); });
	cout << "Day1 Answer1: " << sum << endl;

	// Second task is continue summing input numbers until reached the same intermediate result
	while (true)
	{
		for (auto it = input.begin(); it != input.end(); ++it)
		{
			sum += *it;
			if (results.find(sum) != results.end())
			{
				cout << "Day1 Answer2: " << sum << endl;
				return 0;
			}
			results.insert(sum);
		}
	}
}
