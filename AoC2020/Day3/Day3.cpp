#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct Policy
{
	int min;
	int max;
	char s;
	string pwd;
};

istream& operator>>(istream& i, Policy& p)
{
	char separator;
	i >> p.min >> separator >> p.max >> p.s >> separator >> p.pwd;
	return i;
}
ostream& operator<<(ostream& o, const Policy& p)
{
	o << '[' << p.min << '-' << p.max << ' ' << p.s << ':' << p.pwd << ']';
	return o;
}
int countc(string str, char f)
{
	int count = 0;
	for (auto& c : str)
		if (c == f)
			++count;
	return count;
}

int main()
{
	ifstream is("Day3.txt");
	istream_iterator<Policy> start(is), end;
	vector<Policy> input(start, end);
	cout << "Read " << input.size() << " policies" << endl;
	//for (auto& p : input) cout << p << endl;

	int count = 0;
	for (auto& p : input)
	{
		int c = countc(p.pwd, p.s);
		if (c >= p.min && c <= p.max)
			++count;
	}
	cout << "Day3 Answer1: " << count << endl;

	count = 0;
	for (auto& p : input)
	{
		if (p.min <= p.pwd.size() && p.pwd[p.min - 1] == p.s && p.max <= p.pwd.size() && p.pwd[p.max - 1] != p.s
			|| p.min <= p.pwd.size() && p.pwd[p.min - 1] != p.s && p.max <= p.pwd.size() && p.pwd[p.max - 1] == p.s)
		{
			++count;
		}
	}
	cout << "Day3 Answer2: " << count << endl;
}
