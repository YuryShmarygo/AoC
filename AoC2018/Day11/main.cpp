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

long long Sum(vector<vector<long long>>&input, int x, int y, int si, int sj)
{
	long long sum = 0;
	for (int i = x; i < si; i++)
		for (int j = y; j < sj; j++)
			sum += input[i][j];
	return sum;
}

int main()
{
	int sn = 2187;
	vector<vector<long long>> input(300, vector<long long>(300));
	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 0; j < input.size(); j++)
		{
			long long n = ((j + 11)*(i + 1) + sn) * (j + 11) /100;
			input[i][j] = n - (n / 10) * 10 - 5;
		}
	}
	long long maxX = 0, maxY = 0, maxS = -9999999;
	for (int i = 0; i < input.size() - 2; i++)
	{
		for (int j = 0; j < input[i].size() - 2; j++)
		{

			long long sum = Sum(input, i, j, i + 3, j + 3);
			if (sum > maxS)
			{
				maxS = sum;
				maxX = j;
				maxY = i;
			}
		}
	}
	cout << "Day11 Answer1: " << maxX+1 << "," << maxY+1 << endl;

	long long maxN = 0;
	maxX = 0; maxY = 0; maxS = -9999999;
	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 0; j < input[i].size(); j++)
		{
			int mn = min((int)input.size() - i, (int)input[i].size() - j);
			for (int n = 1; n <= mn; n++)
			{
				long long sum = Sum(input, i, j, i + n, j + n);
				if (sum > maxS)
				{
					maxS = sum;
					maxX = j;
					maxY = i;
					maxN = n;
				}
			}
		}
	}
	cout << "Day11 Answer2: " << maxX + 1 << "," << maxY + 1  << "," << maxN << endl;

	return 0;
}
