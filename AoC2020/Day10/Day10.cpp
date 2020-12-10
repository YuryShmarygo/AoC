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

int find_diff(const vector<int>& input)
{
	vector<int> copy(input);
	copy.push_back(0);
	sort(copy.begin(), copy.end());
	int diffs[3] = { 0,0,1 };
	for (int i = 1; i < copy.size(); ++i)
		++diffs[copy[i] - copy[i - 1] - 1];
	return diffs[0] * diffs[2];
}

long long get_comb(int rank)
{
	if (rank < 3)
		return 1;
	else if (rank == 3)
		return 2;

	return ((long long)1 << (rank - 2)) - rank + 4;
}

long long find_comb(const vector<int>& input)
{
	vector<int> copy(input);
	copy.push_back(0);
	sort(copy.begin(), copy.end());

	long long comb = 1;
	for (int i = 0; i < copy.size();)
	{
		int j = i+1;
		for (; j < copy.size() && copy[j] - copy[j - 1] == 1; ++j)
			;
		comb *= get_comb(j - i);
		i = j;
	}
	return comb;
}

void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day10.txt");
	istream_iterator<int> start(is), end;
	vector<int> input(start, end);

	cout << "Day10 Answer1: " << find_diff(input) << endl;

	cout << "Day10 Answer2: " << find_comb(input) << endl;
}

void Test()
{
	vector<int> test = { 16,10,15,5,1,11,7,19,6,12,4 };
	cout << "test1: " << find_diff(test) << endl;
	cout << "test2: " << find_comb(test) << endl;
	test = { 28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3 };
	cout << "test3: " << find_comb(test) << endl;
}