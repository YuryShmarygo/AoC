#include "../common.h"

int str2p(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;
	return c - 'A' + 27;
}
int find_priority(string str)
{
	auto size = str.size() / 2;
	for (size_t i = 0; i < size; ++i)
	{
		auto it = find(str.begin() + size, str.end(), str[i]);
		if (it != str.end())
			return str2p(*it);
	}
	return 0;
}
int find_badge(vector<string>::iterator it_start)
{
	for (size_t i = 0; i < it_start->size(); ++i)
	{
		auto f1 = find((it_start + 1)->begin(), (it_start+1)->end(), (*it_start)[i]);
		auto f2 = find((it_start + 2)->begin(), (it_start + 2)->end(), (*it_start)[i]);
		if (f1 != (it_start + 1)->end() && f2 != (it_start + 2)->end())
			return str2p((*it_start)[i]);
	}
	return 0;
}
int badges(vector<string>& input)
{
	int sum = 0;
	for (auto it = input.begin(); it != input.end(); it += 3)
		sum += find_badge(it);
	return sum;
}
int main()
{
	ifstream is("Day3.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " commands" << endl;

	cout << "Day3 answer1: " << accumulate(input.begin(), input.end(), 0, [](int sum, auto str) {return sum + find_priority(str); }) << endl;
	cout << "Day3 answer2: " << badges(input) << endl;
}
