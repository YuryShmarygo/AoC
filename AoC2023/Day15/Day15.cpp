#include "../common.h"

vector<string> read_inst(istream& is)
{
	vector<string> res;
	string line;
	getline(is, line);
	size_t cur = 0, next = line.find(',');
	while (cur != string::npos && cur < line.size())
	{
		res.push_back(line.substr(cur, next - cur));
		if (next == string::npos)
			break;
		cur = next + 1;
		next = line.find(',', cur);
	}
	return res;
}
int hash_code(const string& s)
{
	int res = 0;
	for (auto c : s)
	{
		res += c;
		res *= 17;
		res = res % 256;
	}
	return res;
}
long long solve1(const vector<string>& input)
{
	return accumulate(input.begin(), input.end(), (long long)0, [](auto sum, auto& s) {return sum + hash_code(s); });
}
struct lens
{
	string label;
	int strength;
};
struct box
{
	int n;
	list<lens> l;
};
long long focal(const box& b)
{
	long long res = 0;
	int i = 1;
	for (auto it = b.l.begin(); it != b.l.end(); ++it, ++i)
		res += (b.n+1) * i * it->strength;
	return res;
}
long long solve2(const vector<string>& input)
{
	vector<box> boxes(256); for (int i = 0; i < boxes.size(); ++i) { boxes[i].n = i; }
	for (auto& s : input)
	{
		smatch m;
		if (regex_match(s, m, regex(R"((\w+)(=|-)(\d*))")))
		{
			string label = m[1].str();
			string instr = m[2].str();
			int strength = instr[0] == '=' ? stoi(m[3].str()) : 0;
			auto& b = boxes[hash_code(label)];
			auto it = r::find_if(b.l, [label](auto& l) {return l.label == label; });
			if (instr[0] == '-' && it != b.l.end())
				b.l.erase(it);
			else if (instr[0] == '=')
				if (it != b.l.end())
					it->strength = strength;
				else
					b.l.push_back({ label, strength });
		}
	}
	return accumulate(boxes.begin(), boxes.end(), (long long)0, [](auto sum, auto& b) { return sum + focal(b); });
}
int main()
{
	test();
	ifstream is("Day15.txt");
	auto input = read_inst(is);

	auto t_start = high_resolution_clock::now();
	cout << "Day15 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day15 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7)";

	auto is = stringstream(test1);
	auto input = read_inst(is);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
	//cout << "Test2: " << solve2(input, 100) << endl;
}
