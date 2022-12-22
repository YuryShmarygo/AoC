#include "../common.h"

enum e_op {op_num, op_plus, op_minus, op_mult, op_div};
map<char, e_op> c2op{ {'+', op_plus}, {'-', op_minus}, {'*', op_mult}, {'/', op_div} };
map < e_op, function<long long(long long, long long)>> op2op{
{op_plus, [](long long n1, long long n2) {return n1 + n2; }},
{op_minus,[](long long n1, long long n2) {return n1 - n2; }},
{op_mult,[](long long n1, long long n2) {return n1 * n2; }},
{op_div,[](long long n1, long long n2) {return n1 / n2; }}};
struct monkey
{
	string name;
	string raw;
	long long num;
	e_op op;
	bool called;
	vector<string> names;
	map<string, long long> param;
	bool is_ready() { return !called && (op == op_num || param.size() == 2); }
	long long do_op() { called = true; if (op == op_num) return num; return num = op2op[op](param[names[0]], param[names[1]]); }
	long long rev_op(int xn)
	{
		int n = xn == 0 ? 1 : 0;
		if (op == op_plus)
			return param[names[xn]] = num - param[names[n]];
		if (op == op_minus && xn == 1)
			return param[names[1]] = param[names[0]] - num;
		if (op == op_minus && xn == 0)
			return param[names[0]] = param[names[1]] + num;
		if (op == op_mult)
			return param[names[xn]] = num / param[names[n]];
		if (op == op_div && xn == 0)
			return param[names[0]] = num * param[names[1]];
		if (op == op_div && xn == 1)
			return param[names[1]] = param[names[0]] / num;
		cout << "unknown op " << raw << endl;
		return 0;
	}
};
istream& operator>>(istream& is, monkey& m)
{
	m.names.clear(); m.param.clear();
	m.called = false;
	string line;
	if (!getline(is, line) || line.empty()) return is;
	m.raw = line;
	smatch match;
	if (regex_match(line, match, regex(R"(^(\w+): (\d+)$)")))
	{
		m.name = match[1];
		m.op = op_num;
		m.num = stoi(match[2]);
	}
	else if (regex_match(line, match, regex(R"(^(\w+): (\w+) (.) (\w+)$)")))
	{
		m.name = match[1];
		m.op = c2op[match[3].str()[0]];
		m.names.push_back(match[2]);
		m.names.push_back(match[4]);
	}
	return is;
}
ostream& operator<<(ostream& os, const map<string, long long>& param) { for (auto& p : param) os << "[" << p.first << "," << p.second << "] "; return os; }
ostream& operator<<(ostream& os, const set<string>& s) { r::copy(s, ostream_iterator<string>(os, ",")); return os; }
ostream& operator<<(ostream& os, const monkey& m) { return os << m.raw; }
monkey play(vector<monkey>& input)
{
	map<string, vector<monkey>::iterator> all;
	for (auto it = input.begin(); it != input.end(); ++it) all[it->name] = it;
	map<string, set<string>> dep;
	for (auto& m : input) if (m.op != op_num && m.names.size() == 2) for (auto& n : m.names) dep[n].insert(m.name);
	list<string> ready;
	for (auto& m : input) if (m.is_ready()) ready.push_back(m.name);
	while (!ready.empty())
	{
		for (auto& ms : ready)
		{
			auto& m = *all[ms];
			long long res = m.do_op();
			for (auto& d : dep[m.name])
				all[d]->param[m.name] = res;
			if (ms == "root")
				return *all[ms];
		}
		for (auto& m : input) if (m.is_ready()) ready.push_back(m.name);
	}
	return input[0];
}

long long solve1(vector<monkey> input)
{
	return play(input).do_op();
}
long long solve2(vector<monkey> input)
{
	auto r = play(input);
	auto it = r::find_if(input, [](auto& m) {return m.name == "humn"; });

	map<string, vector<monkey>::iterator> all;
	for (auto it = input.begin(); it != input.end(); ++it) all[it->name] = it;
	map<string, set<string>> dep;
	for (auto& m : input) if (m.op != op_num && m.names.size() == 2) for (auto& n : m.names) dep[n].insert(m.name);
	set<string> hdep;
	list<string> cur{ "humn" };
	while (!cur.empty())
	{
		list<string> tmp;
		for (auto& ms : cur)
		{
			auto& m = *all[ms];
			for (auto& d : dep[m.name])
			{
				auto& dm = *all[d];
				hdep.insert(dm.name);
				tmp.push_back(dm.name);
			}
		}
		cur = tmp;
	}
	hdep.insert("humn");
	cout << hdep << endl;

	cur.clear();
	int xn = hdep.contains(r.names[0]) ? 0 : 1;
	int n = (xn + 1) % 2;
	cur.push_back(r.names[xn]);
	all[r.names[xn]]->num = all[r.names[n]]->num;
	while (!cur.empty())
	{
		list<string> tmp;
		for (auto& ms : cur)
		{
			auto& m = *all[ms];
			if (hdep.contains(m.names[0]) && hdep.contains(m.names[1]))
				cout << "both param depends on humn: " << m << endl;
			else if (!hdep.contains(m.names[0]) && !hdep.contains(m.names[1]))
				continue;
			
			int xn = hdep.contains(m.names[0]) ? 0 : 1;
			int n = (xn + 1) % 2;
			tmp.push_back(m.names[xn]);
			all[m.names[xn]]->num = m.rev_op(xn);

			if (m.names[xn] == "humn")
				return all[m.names[xn]]->num;
		}
		cur = tmp;
	}

	return 2;
}
int main()
{
	test(); //return 0;
	ifstream is("Day21.txt");
	istream_iterator<monkey> start(is), end;
	vector<monkey> input(start, end);
	cout << "Day21 answer1: " << solve1(input) << endl;
	cout << "Day21 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(root: pppw + sjmn
dbpl: 5
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
dvpt: 3
lfqf: 4
humn: 5
ljgn: 2
sjmn: drzm * dbpl
sllz: 4
pppw: cczh / lfqf
lgvd: ljgn * ptdq
drzm: hmdt - zczc
hmdt: 32)");
	istream_iterator<monkey> start(is), end;
	vector<monkey> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
