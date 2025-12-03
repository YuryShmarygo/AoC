#include "../common.h"


typedef vector<string> input_t;
input_t read_input(istream& is)
{
	return read_strings(is);
}

long long biggest(const string& s)
{
	string::const_iterator it1 = s.begin();
	for (auto it = s.begin(); it != s.end() - 1; ++it)
	{
		if (*it > *it1)
			it1 = it;
	}
	string::const_iterator it2 = it1 + 1;
	for (auto it = it2; it != s.end(); ++it)
	{
		if (*it > *it2)
			it2 = it;
	}
	return stoll(string() + *it1 + *it2);
}
long long solve1(const input_t& input)
{
	return accumulate(input.begin(), input.end(), 0LL, [](long long sum, const string& s){ return sum + biggest(s); });
}

string add_strings(const string& s1, const string& s2)
{
	string result;
	int carry = 0;
	auto it1 = s1.rbegin();
	auto it2 = s2.rbegin();
	while (it1 != s1.rend() || it2 != s2.rend() || carry != 0)
	{
		int digit1 = it1 != s1.rend() ? (*it1 - '0') : 0;
		int digit2 = it2 != s2.rend() ? (*it2 - '0') : 0;
		int sum = digit1 + digit2 + carry;
		result.push_back((sum % 10) + '0');
		carry = sum / 10;
		if (it1 != s1.rend()) ++it1;
		if (it2 != s2.rend()) ++it2;
	}
	reverse(result.begin(), result.end());
	return result;
}
string biggest2(const string& s)
{
	string result;
	string::const_iterator ite = s.end() - 12;
	string::const_iterator its = s.begin();
	do
	{
		++ite;
		string::const_iterator itc = its;
		for (auto it = its; it != ite; ++it)
		{
			if (*it > *itc)
				itc = it;
		}
		result.push_back(*itc);
		its = itc + 1;
	} while (ite != s.end());
	//cout << result << endl;
	return result;
}
string solve2(const input_t& input)
{
	string result = "0";
	for (auto& s : input)
	{
		result = add_strings(result, biggest2(s));
	}
	return result;
}

int main()
{
	test();
	ifstream is("Day03.txt");
	input_t input = read_input(is);

	cout << "Day3 Answer1: " << solve1(input) << endl;
	cout << "Day3 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(987654321111111
811111111111119
234234234234278
818181911112111)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
