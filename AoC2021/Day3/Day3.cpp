#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct Diag
{
	long gamma;
	long epsilon;
};

Diag Decode(const vector<string>& input)
{
	Diag diag{ 0,0 };
	int len = (int)input.front().size();
	for (int i = 0; i < len; ++i)
	{
		auto c = count_if(input.begin(), input.end(), [i](auto& s) {return s[i] == '1'; });
		if (c > (int)input.size() / 2)
			diag.gamma |= 1 << (len - i - 1);
		else if (c < (int)input.size() / 2)
			diag.epsilon |= 1 << (len - i - 1);
		else
			cout << "Same number of bits in position " << i << endl;
	}
	return diag;
}

vector<string> Filter(const vector<string>& input, int i, bool m)
{
	vector<string> res; res.reserve(input.size());
	int c = (int)count_if(input.begin(), input.end(), [i](auto& s) {return s[i] == '1'; });
	bool m1 = c > (int)input.size() - c;
	bool m0 = c < (int)input.size() - c;
	bool eq = !m1 && !m0;
	copy_if(input.begin(), input.end(), back_inserter(res), [&](auto& s){
		bool b1 = s[i] == '1';
		return m && (m1 && b1 || m0 && !b1 || eq && b1)
		|| !m && (m1 && !b1 || m0 && b1 || eq && !b1); });
	return res;
}

long S2L(string s)
{
	long res = 0;
	for_each(s.begin(), s.end(), [&](char c) {res <<= 1; res += c - '0'; });
	return res;
}
long Find(const vector<string>& input, bool m)
{
	int len = (int)input.front().size();
	auto filtered = input;
	for (int i = 0; i < len; ++i)
	{
		filtered = Filter(filtered, i, m);
		if (filtered.size() == 1)
			break;
	}
	if (filtered.size() > 1)
		cout << "Filtered size: " << filtered.size();
	return S2L(filtered.front());
}

Diag Decode2(const vector<string>& input)
{
	Diag diag{ 0,0 };
	diag.gamma = Find(input, true);
	diag.epsilon = Find(input, false);
	return diag;
}

void Test();
int main()
{
	Test();
	ifstream is("Day3.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " lines." << endl;

	auto diag = Decode(input);
	cout << "Day3 Answer1: " << diag.gamma * diag.epsilon << endl;

	auto diag2 = Decode2(input);
	cout << "Day3 Answer2: " << diag2.gamma * diag2.epsilon << endl;
}

void Test()
{
	vector<string> input = {
"11110",
"10110",
"00100",
"10111",
"10101",
"01111",
"00111",
"11100",
"10000",
"11001",
"00010",
"01010" };

	auto diag = Decode(input);
	cout << "Day3 Test1: " << diag.gamma * diag.epsilon << endl;

	auto diag2 = Decode2(input);
	cout << "Day3 Test2: " << diag2.gamma * diag2.epsilon << endl;
}
