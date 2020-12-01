#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

long FindTwo(const vector<long>& input)
{
	for (auto it = input.begin(); it != input.end(); ++it)
		for (auto it2 = next(it); it2 != input.end(); ++it2)
			if (*it + *it2 == 2020)
				return *it * *it2;
	return 0;
}
long FindThree(const vector<long>& input)
{
	for (auto it = input.begin(); it != input.end(); ++it)
		for (auto it2 = next(it); it2 != input.end(); ++it2)
			for (auto it3 = next(it2); it3 != input.end() && *it + *it2 < 2020; ++it3)
				if (*it + *it2 + *it3 == 2020)
					return *it * *it2 * *it3;
	return 0;
}
void Test()
{
	vector<long> input = { 1721, 979, 366, 299, 675, 1456 };
	cout << "Test1: " << FindTwo(input) << endl;
	cout << "Test2: " << FindThree(input) << endl;
}
int main()
{
	Test();
	ifstream is("Day1.txt");
	istream_iterator<long> start(is), end;
	vector<long> input(start, end);
	cout << "Read " << input.size() << " numbers" << endl;

	cout << "Day1 Answer1: " << FindTwo(input) << endl;
	cout << "Day1 Answer2: " << FindThree(input) << endl;
}
