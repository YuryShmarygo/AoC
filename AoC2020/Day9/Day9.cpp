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

bool check_code(int pos, const vector<long long>& codes)
{
	for (int i = pos - 25; i < pos; ++i)
		for (int j = i + 1; j < pos; ++j)
			if (codes[i] != codes[j] && codes[pos] == codes[i] + codes[j])
				return true;
	return false;
}
long long find_code(const vector<long long>& codes)
{
	for (int i = 25; i < codes.size(); ++i)
		if (!check_code(i, codes))
			return codes[i];
	return 0;
}
long long find_weak(const vector<long long>& codes, long long target)
{
	for (int i = 0; i < codes.size(); ++i)
	{
		long long sum = codes[i];
		int j = i + 1;
		for (; j < codes.size() && sum < target; ++j)
			sum += codes[j];
		if (sum == target && j - i > 2)
		{
			long long max=0, min=(long long)1e15;
			for (int k = i; k <= j; ++k)
			{
				if (codes[k] > max)
					max = codes[k];
				if (codes[k] < min)
					min = codes[k];
			}
			return min + max;
		}
	}
	return 0;
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day9.txt");
	istream_iterator<long long> start(is), end;
	vector<long long> codes(start, end);
	cout << "Read " << codes.size() << " codes" << endl;

	auto code = find_code(codes);
	cout << "Day9 Answer1: " << code << endl;

	cout << "Day9 Answer2: " << find_weak(codes, code) << endl;
}

void Test()
{
}