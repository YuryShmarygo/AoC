#include "../common.h"

typedef function<long long(long long)> operation;
typedef function<bool(long long)> decision;
struct monkey
{
	list<long long> items;
	operation op;
	decision choose;
	pair<int, int> where;
};
vector<monkey> input{
{{84, 66, 62, 69, 88, 91, 91},
[](long long old) { return old * 11 % (2 * 7 * 13 * 3 * 19 * 17 * 11 * 5); },
[](long long item) { return item % 2 == 0; },
{4,7}},

{{98, 50, 76, 99},
[](long long old) { return old * old % (2 * 7 * 13 * 3 * 19 * 17* 11 * 5); },
[](long long item) { return item % 7 == 0; },
{3,6}},

{{72, 56, 94},
[](long long old) { return old + 1; },
[](long long item) { return item % 13 == 0; },
{4,0}},

{{55, 88, 90, 77, 60, 67},
[](long long old) { return old + 2; },
[](long long item) { return item % 3 == 0; },
{6,5}},

{{69, 72, 63, 60, 72, 52, 63, 78},
[](long long old) { return old * 13 % (2 * 7 * 13 * 3 * 19 * 17 * 11 * 5); },
[](long long item) { return item % 19 == 0; },
{1,7}},

{{89, 73},
[](long long old) { return old + 5; },
[](long long item) { return item % 17 == 0; },
{2,0}},

{{78, 68, 98, 88, 66},
[](long long old) { return old + 6; },
[](long long item) { return item % 11 == 0; },
{2,5}},

{{70},
[](long long old) { return old + 7; },
[](long long item) { return item % 5 == 0; },
{1,3}},
};
void turn(monkey& m, vector<monkey>& monkeys)
{
	for (auto item : m.items)
	{
		item = m.op(item);
		if (m.choose(item))
			monkeys[m.where.first].items.push_back(item);
		else
			monkeys[m.where.second].items.push_back(item);
	}
}
long long business(vector<monkey> monkeys, function<long long(long long)> worry, int rounds)
{
	vector<int> inspected(monkeys.size(), 0);
	for (int i = 0; i < rounds; ++i)
	{
		for (int j = 0; j < (int)monkeys.size(); ++j)
		{
			monkey& m = monkeys[j];
			inspected[j] += (int)m.items.size();
			for (auto item : m.items)
			{
				item = worry(m.op(item));
				if (m.choose(item))
					monkeys[m.where.first].items.push_back(item);
				else
					monkeys[m.where.second].items.push_back(item);
			}
			m.items.clear();
		}
		//r::copy(inspected, ostream_iterator<int>(cout, " "));		cout << endl;
	}
	r::sort(inspected);
	return (long long)inspected[inspected.size() - 1] * inspected[inspected.size() - 2];
}

int main()
{
	test();
	//ifstream is("Day11.txt");
	auto w3 = [](long long item) {return item / 3; };
	auto w1 = [](long long item) {return item; };
	cout << "Day11 answer1: " << business(input, w3, 20) << endl;
	cout << "Day11 answer2: " << business(input, w1, 10000) << endl;
}
void test()
{
	ifstream is("Day11.txt");
	string line;
	list<string> output;
	list<list<string>::iterator> ops;
	list<string> div;
	while(getline(is, line))
	{
		getline(is, line);
		smatch m;
		regex_match(line, m, regex(".+items: (.+)"));
		output.push_back(S("{{" << m[1] << "},"));
		getline(is, line);
		regex_match(line, m, regex(".+ new = (.+)"));
		ops.push_back(output.insert(output.end(), S("[](long long old){ return (" << m[1] << ") % ")));
		getline(is, line);
		regex_match(line, m, regex(".+ divisible by (\\d+)"));
		div.push_back(m[1]);
		output.push_back(S("[](long long item) { return item % " << m[1] << " == 0;},"));
		getline(is, line);
		regex_match(line, m, regex(".+throw to monkey (\\d)"));
		output.push_back(S("{" << m[1]));
		getline(is, line);
		regex_match(line, m, regex(".+throw to monkey (\\d)"));
		output.back() += S("," << m[1] << "}},");
		getline(is, line);
	}
	stringstream d;
	r::copy(div, ostream_iterator<string>(d, "*"));
	for (auto op : ops)
		*op += S(d.str() << "1;},");
	r::copy(output, ostream_iterator<string>(cout, "\n"));
}
