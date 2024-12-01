#include "../common.h"

struct pair_i
{
	int first;
	int second;
};
istream& operator>>(istream& is, pair_i& p) { return is >> p.first >> p.second; }
typedef vector<pair_i> input_t;
input_t read_input(istream& is) 
{ 
	istream_iterator<pair_i> start(is), end; 
	input_t input(start, end); 
	return input; 
}

vector<int> to_list(const input_t& input, function<int(const pair_i&)> f)
{
	vector<int> res;
	r::transform(input, back_inserter(res), [f](const pair_i& p) { return f(p); });
	r::sort(res);
	return res;
}
vector<int> to_list_first(const input_t& input)
{ return to_list(input, [](const pair_i& p) {return p.first; }); }
vector<int> to_list_second(const input_t& input)
{ return to_list(input, [](const pair_i& p) {return p.second; }); }


long long solve1(const input_t& input)
{
	auto l1 = to_list_first(input);
	auto l2 = to_list_second(input);
	long long sum = 0;
	for (int i = 0; i < l1.size(); ++i)
		sum += abs(l1[i] - l2[i]);

	return sum;
}
long long solve2(const input_t& input)
{
	auto l1 = to_list_first(input);
	auto l2 = to_list_second(input);
	long long sum = 0;
	for (int i = 0; i < l1.size(); ++i)
		sum += l1[i] * count(l2.begin(), l2.end(), l1[i]);

	return sum;
}

int main()
{
	//test();
	ifstream is("Day01.txt");
	input_t input = read_input(is);

	cout << "Day1 Answer1: " << solve1(input) << endl;
	cout << "Day1 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(3   4
4   3
2   5
1   3
3   9
3   3)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
