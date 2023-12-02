#include "../common.h"

auto first_digit = [&](const string& s) {return find_if(s.begin(), s.end(), [](char c) {return c >= '0' && c <= '9'; }); };
auto last_digit = [&](const string& s) {return find_if(s.rbegin(), s.rend(), [](char c) {return c >= '0' && c <= '9'; }); };
long long calibrate(const vector<string>& input)
{
	return accumulate(input.begin(), input.end(), (long long) 0, [&](long long sum, const string& s) 
		{ return sum + 10 * (*first_digit(s) - '0') + (*last_digit(s) - '0'); });
}
map<string, int> nums({
	{"0",0},{"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},{"9",9},
	{"one",1}, {"two",2}, {"three",3}, {"four",4}, {"five",5}, {"six",6}, {"seven",7}, {"eight",8}, {"nine",9} });
int first_num(const string& s)
{
	auto pos = s.size();
	auto num = nums.begin();
	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		auto p = s.find(it->first);
		if (p != string::npos && p < pos)
		{
			num = it;
			pos = p;
		}
	}
	//cout << s << " first: " << num->first << endl;
	return num->second;
}
int last_num(const string& s)
{
	size_t pos = 0;
	auto num = nums.begin();
	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		auto p = s.rfind(it->first);
		if (p != string::npos && p >= pos)
		{
			num = it;
			pos = p;
		}
	}
	//cout << s << " last: " << num->first << endl;
	return num->second;
}
long long calibrate2(const vector<string>& input)
{
	return accumulate(input.begin(), input.end(), (long long)0, [&](long long sum, string s) { return sum + 10 * first_num(s) + last_num(s); });
}

void test();
int main()
{
	//test();
	ifstream is("Day01.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day1 Answer1: " << calibrate(input) << endl;
	cout << "Day1 Answer2: " << calibrate2(input) << endl;
}
vector<string> read_strings(istream& is)
{
	istream_iterator<string> start(is), end;
	return vector<string>(start, end);
}
void test()
{
	auto test1 = R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)";
	auto test2 = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";
	auto test3 = R"(eightree)";
	auto test4 = R"(sevenine)";

	auto is = stringstream(test1);
	cout << "Test1: " << calibrate(read_strings(is)) << endl;
	is = stringstream(test2);
	cout << "Test2: " << calibrate2(read_strings(is)) << endl;
	is = stringstream(test3);
	cout << "Test3: " << calibrate2(read_strings(is)) << endl;
	is = stringstream(test4);
	cout << "Test4: " << calibrate2(read_strings(is)) << endl;
}
