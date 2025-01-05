#include "../common.h"

struct state_t
{
	string name;
	int val;
};
struct gate_t
{
	string i1;
	string i2;
	string o;
	function<int(int, int)> op;
};
map<string, function<int(int, int)>> op_map = {
	{"AND", [](int a, int b) {return a & b; }},
	{"OR", [](int a, int b) { return a | b; }},
	{"XOR", [](int a, int b) {return a ^ b; }}
};
state_t read_state(string line)
{
	smatch m;
	if (regex_match(line, m, regex(R"(^([^:]+): (\d)$)")))
		return { m[1].str(), stoi(m[2].str()) };
	return { "",0 };
}
gate_t read_gate(string line)
{
	smatch m;
	if (regex_match(line, m, regex(R"(^([^ ]+) (AND|OR|XOR) ([^ ]+) -> ([^ ]+)$)")))
		return { m[1].str(), m[3].str(), m[4].str(), op_map[m[2].str()] };
	return { "","","",op_map["AND"] };
}
struct input_t
{
	vector<state_t> state;
	vector<gate_t> gates;
};
input_t read_input(istream& is)
{
	input_t res;
	string line;
	while(getline(is, line) && !line.empty())
		res.state.push_back(read_state(line));
	while (getline(is, line) && !line.empty())
		res.gates.push_back(read_gate(line));
	return res;
}
long long get_num(const map<string, int> results)
{
	long long num = 0;
	for (auto it = results.rbegin(); it != results.rend(); ++it)
		if (it->first.starts_with("z"))
			num = num * 2 + it->second;
	return num;
}
long long solve1(const input_t& input)
{
	map<string, int> results;
	for (auto& s : input.state)
		results[s.name] = s.val;
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (auto& g : input.gates)
		{
			auto it1 = results.find(g.i1);
			auto it2 = results.find(g.i2);
			auto it3 = results.find(g.o);
			if (it1 != results.end() && it2 != results.end() && it3 == results.end())
			{
				changed = true;
				results[g.o] = g.op(it1->second, it2->second);
			}
		}
	}

	return get_num(results);
}
long long solve2(const input_t& input)
{
	//map<int, set<gate_t>> dep_i;
	map<gate_t, vector<string>> inp;
	map<string, vector<string>> dep;
	map<string, vector<string>> dep_z;
	int dx = r::count_if(input.state, [](auto& s) {return s.name.starts_with("x"); });
	set<string> cur;
	r::transform(input.state, inserter(cur, cur.begin()), [](auto& s) { return s.name; });
	while((int)dep_z.size() < dx +1)
	{
		for (auto& g : input.gates)
		{
			if (cur.contains(g.i1) && cur.contains(g.i2))
			{
				dep[g.o] = dep[g.i1];

			}
		}
	}
/*	for (auto it = results.rbegin(); it != results.rend(); ++it)
		if (it->first.starts_with("z"))
			num = num * 2 + it->second;

	*/
	int res = 0;
	return res;
}

int main()
{
	test();
	ifstream is("Day24.txt");
	input_t input = read_input(is);

	cout << "Day24 Answer1: " << solve1(input) << endl;
	cout << "Day24 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(x00: 1
x01: 1
x02: 1
y00: 0
y01: 1
y02: 0

x00 AND y00 -> z00
x01 XOR y01 -> z01
x02 OR y02 -> z02)";

	auto test2 = R"(x00: 1
x01: 0
x02: 1
x03: 1
x04: 0
y00: 1
y01: 1
y02: 1
y03: 1
y04: 1

ntg XOR fgs -> mjb
y02 OR x01 -> tnw
kwq OR kpj -> z05
x00 OR x03 -> fst
tgd XOR rvg -> z01
vdt OR tnw -> bfw
bfw AND frj -> z10
ffh OR nrd -> bqk
y00 AND y03 -> djm
y03 OR y00 -> psh
bqk OR frj -> z08
tnw OR fst -> frj
gnj AND tgd -> z11
bfw XOR mjb -> z00
x03 OR x00 -> vdt
gnj AND wpb -> z02
x04 AND y00 -> kjc
djm OR pbm -> qhw
nrd AND vdt -> hwm
kjc AND fst -> rvg
y04 OR y02 -> fgs
y01 AND x02 -> pbm
ntg OR kjc -> kwq
psh XOR fgs -> tgd
qhw XOR tgd -> z09
pbm OR djm -> kpj
x03 XOR y03 -> ffh
x00 XOR y04 -> ntg
bfw OR bqk -> z06
nrd XOR fgs -> wpb
frj XOR qhw -> z04
bqk OR frj -> z07
y03 OR x01 -> nrd
hwm AND bqk -> z03
tgd XOR rvg -> z12
tnw OR pbm -> gnj)";


	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1 (expect 4): " << solve1(input) << endl;
	is = stringstream(test2); input = read_input(is);
	cout << "Test2 (expect 2024): " << solve1(input) << endl;
}
