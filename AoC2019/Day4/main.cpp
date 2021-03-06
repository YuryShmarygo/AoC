#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 

using namespace std;

int _x = 271973;
int _y = 785961;

vector<int> GetDigits(int n)
{
	vector<int> numbers;
	numbers.reserve(6);
	while (n > 0)
	{
		numbers.push_back(n % 10);
		n /= 10;
	}
	reverse(numbers.begin(), numbers.end());
	return numbers;
}

int main()
{
	int a = 0;
	int a2 = 0;
	for (int i = _x; i <= _y; ++i)
	{
		auto n = GetDigits(i);
		bool inc = true;
		bool same = false;
		bool same2 = false;
		int g = 1;

		for (int j = 0; j < 5; ++j)
		{
			if (n[j + 1] == n[j])
			{
				same = true;
				++g;
			}
			else if (n[j + 1] < n[j])
			{
				inc = false;
				break;
			}
			else
			{
				if (g == 2)
					same2 = true;
				else
					g = 1;
			}
		}
		if (g == 2)
			same2 = true;
		if (inc && same)
			a++;
		if (inc && same2)
			a2++;
	}

	cout << "Day4 answer1: " << a << endl;
	cout << "Day4 answer2: " << a2 << endl;
	return 0;
}
