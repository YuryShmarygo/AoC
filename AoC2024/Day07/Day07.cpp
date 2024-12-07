#include "../common.h"

struct test_t
{
	long long res;
	vector<int> num;
};
typedef vector<test_t> input_t;
test_t read_test(string line)
{
	test_t t;
	smatch m;
	if (regex_match(line, m, regex(R"((\d+):(.+)$)")))
	{
		t.res = stoll(m[1].str());
		line = m[2].str();
		smatch n;
		while (regex_search(line, n, regex(R"((\d+))")))
		{
			int num = stoi(n[1].str());
			t.num.push_back(num);
			line = n.suffix();
		}
	}
	return t;
}
input_t read_input(istream& is) 
{
	input_t input;
	string line;
	while (getline(is, line) && !line.empty())
	{
		test_t t = read_test(line);
		input.push_back(t);
	}
	return input;
}

typedef function<long long(long long, long long)> op_t;
long long mult(long long n1, long long n2) { return n1 * n2; }
long long add(long long n1, long long n2) { return n1 + n2; }
long long join(long long n1, long long n2)
{
	long long n = n2;
	while (n > 0)
	{
		n1 *= 10;
		n /= 10;
	}
	return n1 + n2;
}
long long calc(vector<int> num, vector<op_t> ops)
{
	long long res = num[0];
	for (int i = 0; i < num.size() - 1; ++i)
		res = ops[i](res, num[i + 1]);
	return res;
}
vector<op_t> all_ops = { mult,add,join };
vector<op_t> num_to_ops(long long num, int len, int base)
{
	vector<op_t> res(len, all_ops[0]);
	int i = 0;
	while (num > 0)
	{
		int cur = num % base;
		res[i++] = all_ops[cur];
		num /= base;
	}
	
	return res;
}
long long calc(const test_t& t, int base)
{
	long long nums = static_cast<long long>(pow(base, t.num.size() - 1));
	for (long long i = 0; i < nums; ++i)
		if (calc(t.num, num_to_ops(i, static_cast<int>(t.num.size() - 1), base)) == t.res)
			return t.res;
	return 0;
}
long long solve1(const input_t& input)
{
	long long sum = 0;
	for (auto& t : input)
		sum += calc(t,2);
	return sum;
}

long long solve2(const input_t& input)
{
	long long sum = 0;
	for (auto& t : input)
		sum += calc(t,3);
	return sum;
}

int main()
{
	test();
	ifstream is("Day07.txt");
	input_t input = read_input(is);

	cout << "Day7 Answer1: " << solve1(input) << endl;
	cout << "Day7 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
