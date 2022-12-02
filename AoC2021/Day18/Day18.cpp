#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()


struct snode
{
	int num;
	snode* parent;
	snode* left;
	snode* right;
	snode() : num(0), parent(nullptr), left(nullptr), right(nullptr){}
};
istream& operator>>(istream& is, snode& p)
{
	char c;
	is >> c;
	if (c >= '0' && c <= '9')
	{
		p.left = p.right = nullptr;
		p.num = c - '0';
	}
	else if (c == '[')
	{
		snode* left = new snode();
		left->parent = &p;
		p.left = left;
		is >> *left;
		is >> c;
		snode* right = new snode();
		right->parent = &p;
		p.right = right;
		is >> *right;
		is >> c;
	}
	return is;
}
ostream& operator<<(ostream& os, const snode& p)
{
	if (p.left == nullptr && p.right == nullptr)
		os << p.num;
	else
		os << '[' << *p.left << ',' << *p.right << ']';
	return os;
}
struct snum
{
	snode* root;
	snum() : root(new snode()) {}
	snum(string str) : root(new snode())
	{
		stringstream s(str);
		s >> *root;
	}
	~snum()
	{
		ranges::for_each(all_nodes(), [](snode* node) {delete node; });
	}
	typedef function<bool(snode*, int)> t_func;
	bool traverse_internal(snode* cur, int level, t_func call_back)
	{

		bool cont = call_back(cur, level);
		if (!cont) return cont;
		snode* res = nullptr;

		if (cur->left != nullptr)
			cont = traverse_internal(cur->left, level + 1, call_back);

		if (!cont) return cont;
		if (cur->right != nullptr)
			cont = traverse_internal(cur->right, level + 1, call_back);

		return cont;
	}
	void traverse_tree(t_func call_back)
	{
		traverse_internal(root, 0, call_back);
	}
	vector<snode*> all_nodes()
	{
		vector<snode*> res;
		traverse_tree([&](snode* node, int) { res.push_back(node); return true; });
		return res;
	}
	vector<snode*> all_leafs()
	{
		vector<snode*> res;
		traverse_tree([&](snode* node, int)
		{
			if (node->left == nullptr && node->right == nullptr)
				res.push_back(node);
			return true;
		});
		return res;

	}
	snode* find4nested()
	{
		snode* res = nullptr;
		traverse_tree([&](snode* node, int level)
			{
				if (level == 4 && node->left != nullptr && node->right != nullptr)
					res = node;
				return res == nullptr;
			});
		return res;
	}
	snode* find10()
	{
		auto leafs = all_leafs();
		auto it = ranges::find_if(leafs, [](snode* node) { return node->left == nullptr && node->right == nullptr && node->num >= 10; });
		return it != leafs.end() ? *it : nullptr;
	}
	void explode(snode* cur)
	{
		auto leafs = all_leafs();
		auto it_left = ranges::find_if(leafs, [&](snode* node) {return node == cur->left; });
		if (it_left != leafs.begin())
		{
			--it_left;
			(*it_left)->num += cur->left->num;
		}

		auto it_right = ranges::find_if(leafs, [&](snode* node) {return node == cur->right; });
		if (it_right != leafs.end() && next(it_right) != leafs.end())
		{
			++it_right;
			(*it_right)->num += cur->right->num;
		}
		cur->left = cur->right = nullptr;
		cur->num = 0;
	}
	void split(snode* cur)
	{
		auto left = new snode;
		left->parent = cur;
		left->left = left->right = nullptr;
		left->num = cur->num / 2;
		cur->left = left;
		auto right = new snode;
		right->parent = cur;
		right->left = right->right = nullptr;
		right->num = (cur->num + 1) / 2;
		cur->right = right;
		cur->num = 0;
	}
	void reduce()
	{
		snode* next = root;
		while (next != nullptr)
		{
			//cout << *root << endl;
			next = find4nested();
			if (next != nullptr)
				explode(next);
			else
			{
				next = find10();
				if (next != nullptr)
					split(next);
			}
		}
	}
	void add(snum& n)
	{
		auto res = new snode;
		root->parent = n.root->parent = res;
		res->left = root;
		res->right = n.root;
		n.root = new snode;
		root = res;
		//cout << (*root) << endl;
		reduce();
	}
	long mag(snode* cur = nullptr)
	{
		cur = cur == nullptr ? root : cur;
		if (cur->left == nullptr && cur->right == nullptr)
			return cur->num;
		return mag(cur->left) * 3 + mag(cur->right) * 2;
	}
};
istream& operator>>(istream& is, snum& p)
{
	is >> *p.root;
	return is;
}

ostream& operator<<(ostream& os, const snum& p)
{
	os << *p.root;
	return os;
}
long add_all(vector<string>& input)
{
	snum res(input[0]);
	for (size_t i = 1; i < input.size(); ++i)
	{
		snum n(input[i]);
		res.add(n);
	}
	return res.mag();
}
long max_mag(vector<string>& input)
{
	long mag_max = 0;
	for (size_t i = 0; i < input.size(); ++i)
	{
		for (size_t j = 0; j < input.size(); ++j)
		{
			if (i == j) continue;
			snum res(input[i]), n(input[j]);
			res.add(n);
			long cur = res.mag();
			mag_max = max(mag_max, cur);
		}
	}
	return mag_max;
}
void test();
int main()
{
	//test(); return 0;
	ifstream is("Day18.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day18 Answer1: " << add_all(input) << endl;
	cout << "Day18 Answer2: " << max_mag(input) << endl;
}

void exp_test(string str)
{
	snum n(str);
	cout << "Parsed snum: " << n << endl;
	snode* exp = n.find4nested();
	if (exp != nullptr)
		n.explode(exp);
	cout << "Exploded:    " << n << endl;
}
void split_test(string str)
{
	snum n(str);
	cout << "Parsed snode: " << n << endl;
	snode* sp = n.find10();
	if (sp != nullptr)
		n.split(sp);
	cout << "Split:        " << n << endl;
}
void mag_test(string str)
{
	snum n(str);
	cout << n << endl << "mag: " << n.mag() << endl;
}
void test()
{
	exp_test("[[[[[9,8],1],2],3],4]");
	exp_test("[7,[6,[5,[4,[3,2]]]]]");
	exp_test("[[6,[5,[4,[3,2]]]],1]");
	exp_test("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]");
	exp_test("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");

	snum n1("[[[[4,3],4],4],[7,[[8,4],9]]]");
	snum n2("[1,1]");
	n1.add(n2);
	cout << n1 << endl;

	vector<string> strings{
"[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]",
"[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]",
"[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]",
"[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]",
"[7,[5,[[3,8],[1,4]]]]"								   ,
"[[2,[2,2]],[8,[8,1]]]"								   ,
"[2,9]"												   ,
"[1,[[[9,3],9],[[9,0],[0,7]]]]"						   ,
"[[[5,[7,4]],7],1]"									   ,
"[[[[4,2],2],6],[8,7]]" };
	cout << "test sum: " << add_all(strings) << endl;

	mag_test("[[1,2],[[3,4],5]]");
	mag_test("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
	mag_test("[[[[1,1],[2,2]],[3,3]],[4,4]]");
	mag_test("[[[[3,0],[5,3]],[4,4]],[5,5]]");
	mag_test("[[[[5,0],[7,4]],[5,5]],[6,6]]");
	mag_test("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");

	vector<string> hw{
"[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]",
"[[[5,[2,8]],4],[5,[[9,9],0]]]",
"[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
"[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]",
"[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]",
"[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
"[[[[5,4],[7,7]],8],[[8,3],8]]",
"[[9,3],[[9,9],[6,[4,9]]]]",
"[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
"[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]",
	};
	cout << "hw max mag: " << max_mag(hw) << endl;
}