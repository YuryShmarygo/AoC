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

struct SItem
{
	long long num;
	char op;
};
bool plus_before = false;
void eval(SItem& op, long long num)
{
	if (op.op == ' ')
		op.num = num;
	else if (op.op == '+')
	{
		op.num = op.num + num;
		op.op = ' ';
	}
	else if (op.op == '*')
	{
		op.num = op.num * num;
		op.op = ' ';
	}
}
long long eval(string str, int& pos, bool plus = false)
{
	SItem op{ 0, ' ' };
	for (; pos < str.size(); ++pos)
	{
		char c = str[pos];
		if (c == ' ')
			continue;
		if (c >= '0' && c <= '9')
		{
			long long num = c - '0';
			eval(op, num);
		}
		else if (c == '(')
		{
			++pos;
			long long num = eval(str, pos);
			eval(op, num);
		}
		else if (c == ')')
		{
			if (plus)
				--pos;
			return op.num;
		}
		else
		{
			op.op = c;
			if (plus_before && c == '*')
			{
				++pos;
				long long num = eval(str, pos, true);
				eval(op, num);
			}
		}
	}
	return op.num;
}
long long eval(string str, bool plus = false)
{
	plus_before = plus;
	int pos = 0;
	return eval(str, pos);
}

long long sum(const vector<string> input, bool plus = false)
{
	return accumulate(input.begin(), input.end(), (long long)0, [&](auto sum, auto& s) {return sum + eval(s, plus); });
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day18.txt");
	vector<string> input;
	string line;
	while (is && getline(is, line) && !line.empty())
		input.push_back(line);

	cout << "Day18 Answer1: " << sum(input) << endl;
	cout << "Day18 Answer2: " << sum(input, true) << endl;
}

void Test()
{
	vector<string> tests = { "1 + 2 * 3 + 4 * 5 + 6",
	"1 + (2 * 3) + (4 * (5 + 6))",
	"2 * 3 + (4 * 5)",
	"5 + (8 * 3 + 9 + 3 * 4 * 3)",
	"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))",
	"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2" };
	
	for (int i = 0; i < tests.size(); ++i)
		cout << "Test1 " << i << " " << eval(tests[i]) << endl;
	for (int i = 0; i < tests.size(); ++i)
		cout << "Test2 " << i << " " << eval(tests[i], true) << endl;
	//cout << "Test 2: " << hcount6(input) << endl;
}