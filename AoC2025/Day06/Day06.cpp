#include "../common.h"

struct prob
{
	vector<long long> n;
	char op;
};
typedef vector<string> input_t;
input_t read_input(istream& is)
{
	input_t input;
	string line;
	while (getline(is, line) && !line.empty())
		input.push_back(line);
	return input;
}
vector<prob> convert(const input_t& m)
{
	vector<prob> input;
	vector<vector<long long>> nums;
	vector<char> ops;
	for (auto line : m)
	{
		if (line.starts_with("+") || line.starts_with("*"))
		{
			for (auto c: line)
				if (!isspace(c))
					ops.push_back(c);
		}
		else
		{
			auto s = stringstream(line);
			istream_iterator<long long> start(s), end;
			vector<long long> n(start, end);
			nums.push_back(n);
		}
	}
	for (int j = 0; j < nums[0].size(); ++j)
	{
		prob p;
		for (int i = 0; i < nums.size(); ++i)
			p.n.push_back(nums[i][j]);
		p.op = ops[j];
		input.push_back(p);
	}

	return input;
}

long long solve(const prob& p)
{
	if (p.op == '+')
		return accumulate(p.n.begin(), p.n.end(), 0LL);
	else
		return accumulate(p.n.begin(), p.n.end(), 1LL, [](long long a, long long b) { return a * b; });
}

long long solve1(const input_t& input)
{
	auto m = convert(input);
	return accumulate(m.begin(), m.end(), 0LL, [](long long sum, const prob& p) { return sum + solve(p); });
}

long long solve2(const vector<string>& m)
{
	vector<prob> probs;
	vector<string> ops;
	string op;
	for (auto c : m[m.size() - 1])
	{
		if (c == '*' || c == '+')
		{
			if (!op.empty())
				ops.push_back(op);
			op.assign(1, c);
		}
		else
		{
			op.push_back(c);
		}
	}
	ops.push_back(op);
	int col = 0;
	for (int j = 0; j < ops.size(); ++j)
	{
		op = ops[j];
		prob np;
		np.op = op[0];
		for (int i = 0; i < op.size(); ++i)
		{
			long long digit = 0;
			for (int l = 0; l < m.size() - 1; ++l)
			{
				if (m[l].size() > col + i && m[l][col + i] != ' ')
				{
					digit *= 10;
					digit += m[l][col + i] - '0';
				}
			}
			if (digit != 0)
				np.n.push_back(digit);
		}
		col += (int)op.size();
		probs.push_back(np);
	}
	return accumulate(probs.begin(), probs.end(), 0LL, [](long long sum, const prob& p) { return sum + solve(p); });

}

int main()
{
	test();
	ifstream is("Day06.txt");
	auto input = read_input(is);
	cout << "Day6 Answer1: " << solve1(input) << endl;
	cout << "Day6 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
