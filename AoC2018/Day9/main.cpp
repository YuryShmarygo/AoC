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
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

list<int>::iterator Advance(list<int>::iterator it, int op, list<int>::iterator start, list<int>::iterator end)
{
	for (; op > 0; --op) it = next(it) == end ? start : next(it);
	for (; op < 0; ++op) it = it == start ? prev(end) : prev(it);
	return it;
}

long long Solve(int players, int maxM)
{
	list<int> circle{ 0 };
	vector<long long> scores(players, 0);
	list<int>::iterator it = circle.begin();

	for (int cur = 1; cur <= maxM; cur++)
	{
		if (cur % 23 == 0)
		{
			it = Advance(it, -7, circle.begin(), circle.end());
			scores[(cur - 1) % players] += *it + cur;
			it = circle.erase(it);
			if (it == circle.end()) it = circle.begin();
		}
		else
		{
			it = Advance(it, 2, circle.begin(), circle.end());
			it = circle.insert(it, cur);
		}
	}
	return *(max_element(scores.begin(), scores.end()));
}

void test()
{
	struct Input { int p; int m; };
	vector<Input> input{ {9,25}, {10, 1618}, {13, 7999}, {17, 1104}, {21, 6111}, {30, 5807} };
	for (auto i : input)
		cout << i.p << " " << i.m << ": " << Solve(i.p, i.m) << endl;
}
int main()
{
	test();
	cout << "Day9 Answer1: " << Solve(468, 71010) << endl;
	cout << "Day9 Answer2: " << Solve(468, 7101000) << endl;

	return 0;
}
