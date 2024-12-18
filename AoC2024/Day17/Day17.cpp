#include "../common.h"

struct reg_t
{
	int a;
	int b;
	int c;
};
struct input_t
{
	reg_t r;
	string i;
};
int read_reg(string line)
{
	smatch m;
	if (regex_match(line, m, regex(R"(Register [A|B|C]: (\d+))")))
		return stoi(m[1].str());
	return 0;
}
string read_i(string line)
{
	smatch m;
	string s;
	if (regex_match(line, m, regex(R"(^Program: (.+)$)")))
		s = m[1].str();
	string i;
	for (auto c : s)
		if (c != ',')
			i.push_back(c);
	return i;
}
input_t read_input(istream& is) 
{
	input_t input;
	string line;
	getline(is, line);
	input.r.a = read_reg(line);
	getline(is, line);
	input.r.b = read_reg(line);
	getline(is, line);
	input.r.c = read_reg(line);
	getline(is, line); getline(is, line);
	input.i = read_i(line);
	return input;
}
struct machine_t
{
	vector<string> run(const input_t& input)
	{
		//ofstream log("day17.out", ios_base::app);
		int a = input.r.a;
		int b = input.r.b;
		int c = input.r.c;
		int i = 0;
		vector<string> res;
		while (i >= 0 && i < input.i.size())
		{
			char ins = input.i[i];
			char ops = input.i[i+1];

			long long op = 0;
			switch (ops)
			{
			case '0':
			case '1':
			case '2':
			case '3':
				op =  ops - '0';
				break;
			case '4':
				op = a;
				break;
			case '5':
				op = b;
				break;
			case '6':
				op = c;
				break;
			case '7':
			default:
				cout << "invalid op" << endl;
				break;
			};

			switch (ins)
			{
			case '0':
				op = 1LL << op;
				a = (int)(a / op);
				i += 2;
				break;
			case '1':
				b = b ^ (int)op;
				i += 2;
				break;
			case '2':
				b = op % 8;
				i += 2;
				break;
			case '3':
				if (a == 0)
					i += 2;
				else
					i = (int)op;
				break;
			case '4':
				b = b ^ c;
				i += 2;
				break;
			case '5':
				res.push_back(S(op % 8));
				i += 2;
				break;
			case '6':
				op = 1LL << op;
				b = (int)(a / op);
				i += 2;
				break;
			case '7':
				op = 1LL << op;
				c = (int)(a / op);
				i += 2;
				break;
			default:
				cout << "invalid op" << endl;
				break;
			}
		}
		return res;
	}
	int back_run(const input_t& input)
	{
		//ofstream log("day17.out", ios_base::app);
		int a = input.r.a;
		int b = input.r.b;
		int c = input.r.c;
		int i = 0;
		vector<string> res;
		while (i >= 0 && i < input.i.size())
		{
			char ins = input.i[i];
			char ops = input.i[i + 1];

			long long op = 0;
			switch (ops)
			{
			case '0':
			case '1':
			case '2':
			case '3':
				op = ops - '0';
				break;
			case '4':
				op = a;
				break;
			case '5':
				op = b;
				break;
			case '6':
				op = c;
				break;
			case '7':
			default:
				cout << "invalid op" << endl;
				break;
			};

			switch (ins)
			{
			case '0':
				op = 1LL << op;
				a = (int)(a / op);
				i += 2;
				break;
			case '1':
				b = b ^ (int)op;
				i += 2;
				break;
			case '2':
				b = op % 8;
				i += 2;
				break;
			case '3':
				if (a == 0)
					i += 2;
				else
					i = (int)op;
				break;
			case '4':
				b = b ^ c;
				i += 2;
				break;
			case '5':
				res.push_back(S(op % 8));
				i += 2;
				break;
			case '6':
				op = 1LL << op;
				b = (int)(a / op);
				i += 2;
				break;
			case '7':
				op = 1LL << op;
				c = (int)(a / op);
				i += 2;
				break;
			default:
				cout << "invalid op" << endl;
				break;
			}
		}
		return 0;
	}
};
string solve1(const input_t& input)
{
	machine_t m;
	auto mres = m.run(input);
	string res;
	for (auto s : mres)
	{
		res.append(s);
		res.append(",");
	}
	return res;
}

long long solve2(const input_t& input)
{
	return 0;
}

int main()
{
	//test();
	ifstream is("Day17.txt");
	input_t input = read_input(is);

	cout << "Day17 Answer1: " << solve1(input) << endl;
	cout << "Day17 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
