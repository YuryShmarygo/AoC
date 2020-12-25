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

int get_loop_size(int key)
{
	int enc = 1;
	int res = 0;
	while (enc != key)
	{
		++res;
		enc *= 7;
		enc = enc % 20201227;
	}
	return res;
}
int encrypt(int sub, int loop)
{
	long long res = 1;
	for (; loop > 0; --loop)
	{
		res *= (long long)sub;
		res = res % 20201227;
	}
	return (int)res;
}
int task1(vector<int> p_keys)
{
	int c_size = get_loop_size(p_keys[0]);
	return encrypt(p_keys[1], c_size);
}

void Test();
int main()
{
	Test();// return 0;
	vector<int> p_keys = { 17773298,15530095 };
	cout << "Day25 Answer1: " << task1(p_keys) << endl;
}

void Test()
{
	vector<int> test = { 5764801, 17807724 };
	cout << "Test 1: " << task1(test) << endl;
}