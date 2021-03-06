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

typedef vector<char> Scores;
Scores::iterator Move(Scores::iterator it, int shift, Scores& scores)
{
	for (; shift > 0; shift--)
	{
		it = next(it);
		if (it == scores.end())
			it = scores.begin();
	}
	return it;
}
void Print(Scores& scores)
{
	for (auto i : scores)
		cout << (int)i;
	cout << endl;
}
void Solve(Scores& scores, long long n)
{
	scores.reserve(n + 1);
	Scores::iterator elf1 = scores.begin(), elf2 = next(scores.begin());

	while (scores.size() < n)
	{
		int rec = *elf1 + *elf2;
		if (rec >= 10)
			scores.push_back(rec / 10);
		scores.push_back(rec % 10);
		elf1 = Move(elf1, *elf1 + 1, scores);
		elf2 = Move(elf2, *elf2 + 1, scores);
	}
}

void test()
{
	Scores input{ 3, 7 };
	Solve(input, 20);
	Print(input);
}
int main()
{
	test();
	Scores input{ 3,7 };
	Solve(input, 919901 + 10);
	//Print(input);
	cout << "Day14 Answer1: ";
	for (auto it = next(input.begin(), 919901); it != input.end(); it++)
		cout << *it;
	cout << endl;

	long long start = 919901;
	Scores pattern{ 9,1,9,9,0,1 };
	auto it = search(input.begin(), input.end(), pattern.begin(), pattern.end());
	while (it == input.end())
	{
		start *= 10;
		input = { 3,7 };
		Solve(input, start);
		it = search(input.begin(), input.end(), pattern.begin(), pattern.end());
	}
	cout << "Day14 Answer2: " << distance(input.begin(), it) << endl;
	return 0;
}
