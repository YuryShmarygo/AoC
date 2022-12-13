#include "../common.h"

struct snode
{
	snode() : num(-1) {}
	int num;
	list<snode*> lst;
};
ostream& operator<<(ostream& os, const snode& node)
{
	if (node.lst.empty())
		return os << node.num;
	for (auto it = node.lst.begin(); it != node.lst.end(); ++it)
		os << (it == node.lst.begin() ? "[" : ",") << **it;
	return os << "]";
}
istream& operator>>(istream& is, snode& node);
istream& operator>>(istream& is, list<snode*>& lst)
{
	lst.clear();
	string line; getline(is, line);
	size_t cur = 1;
	while (cur < line.size() && line[cur] != ']')
	{
		snode* node = new snode();
		stringstream(line.substr(cur)) >> *node;
		lst.push_back(node);
		cur += S(*node).size();
		if (line[cur] == ',')
			++cur;
	}
	return is;
}
istream& operator>>(istream& is, snode& node)
{
	string line;
	getline(is, line);
	if (line.empty()) getline(is, line);
	if (line[0] == '[')
	{
		stringstream(line) >> node.lst;
		return is;
	}
	stringstream(line) >> node.num;
	return is;
}
struct slist
{
	snode root;
};
istream& operator>>(istream& is, slist& lst)
{ return is >> lst.root; }
ostream& operator<<(ostream& os, const slist& lst)
{ return os << lst.root; }
istream& operator>>(istream& is, pair<slist, slist>& p)
{ return is >> p.first >> p.second; }
bool operator<(const snode& s1, const snode& s2);
bool operator<(const list<snode*>& l1, const list<snode*>& l2)
{
	auto it1 = l1.begin();
	auto it2 = l2.begin();
	for (; it1 != l1.end() && it2 != l2.end(); ++it1, ++it2)
	{
		if (**it1 < **it2)
			return true;
		if (**it2 < **it1)
			return false;
	}
	return it1 == l1.end() && it2 != l2.end();
}
bool operator<(const snode& s1, const snode& s2)
{
	if (s1.lst.empty() && s2.lst.empty())
		return s1.num < s2.num;
	if (s1.lst.empty())
	{
		if (s1.num == -1 && s2.lst.size() == 1 && s2.lst.front()->num == -1)
			return true;
		return list<snode*>(1, &const_cast<snode&>(s1)) < s2.lst;
	}
	if (s2.lst.empty())
	{
		if (s2.num == -1 && s1.lst.size() == 1 && s1.lst.front()->num == -1)
			return false;
		return s1.lst < list<snode*>(1, &const_cast<snode&>(s2));
	}
	return s1.lst < s2.lst;
}
bool operator<(const slist& l1, const slist& l2)
{ return l1.root < l2.root; }
int sum_right(const vector<pair<slist, slist>>& input)
{
	int sum = 0;
	for (size_t i = 0; i < input.size(); ++i)
		if (input[i].first < input[i].second)
			sum += static_cast<int>(i) + 1;
	return sum;
}
int find_marks(vector<pair<slist, slist>> input)
{
	pair<slist, slist> p;
	stringstream(R"([[2]]
[[6]])") >> p;
	input.push_back(p);
	vector<slist> plain;
	for (auto& [fst, snd] : input)
		plain.insert(plain.end(), { fst, snd });

	sort(plain.begin(), plain.end());
	int res = 1;
	for (size_t i = 0; i < plain.size(); ++i)
	{
		string line = S(plain[i]);
		if (line == "[[2]]" || line == "[[6]]")
			res *= static_cast<int>(i) + 1;
	}
	return res;
}

int main()
{
	test();
	ifstream is("Day13.txt");
	istream_iterator<pair<slist, slist>> start(is), end;
	vector<pair<slist, slist>> input(start, end);

	cout << "Day13 answer1: " << sum_right(input) << endl;
	cout << "Day13 answer2: " << find_marks(input) << endl;
}
void test()
{
	ifstream is("Day13_test.txt");

	string line = "[1,[2,[3,[4,[5,6,7]]]],8,9]";
	slist l;
	stringstream(line) >> l;
	pair<slist, slist> p;
	stringstream(R"([[[]]]
[[]])") >> p;
	cout << p.first << " vs " << p.second << " " << (p.first < p.second) << endl;
	stringstream(R"([[]]
[[[]]])") >> p;
	cout << p.first << " vs " << p.second << " " << (p.first < p.second) << endl;
	stringstream(R"([[1,2],1]
[[1,2],2])") >> p;
	cout << p.first << " vs " << p.second << " " << (p.first < p.second) << endl;
	stringstream(R"([[1,2],2]
[[1,2],1])") >> p;
	cout << p.first << " vs " << p.second << " " << (p.first < p.second) << endl;

	istream_iterator<pair<slist, slist>> start(is), end;
	vector<pair<slist, slist>> input(start, end);

	for (auto& [fst, snd] : input)
		cout << fst << " vs " << snd << " " << (fst < snd) << endl;

	cout << "test1: " << sum_right(input) << endl;

	stringstream(R"([[2]]
[[6]])") >> p;
//	input.push_back(p);
	vector<slist> plain;
	for (auto& [fst, snd] : input)
		plain.insert(plain.end(), { fst, snd });
	sort(plain.begin(), plain.end());
	r::copy(plain, ostream_iterator<slist>(cout, "\n"));

	cout << "test2: " << find_marks(input) << endl;
}
