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
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

struct Node
{
	int n;
	int m;
	vector<Node *> childs;
	vector<int> data;
};

Node* ReadNode(istream& is)
{
	Node * node = new Node();
	is >> node->n >> node->m;
	generate_n(back_inserter(node->childs), node->n, [&]() {return ReadNode(is); });
	copy_n(istream_iterator<int>(is), node->m, back_inserter(node->data));
	return node;
}

int NodeSum(Node * node)
{
	int val = 0;
	for (auto i : node->childs) val += NodeSum(i);
	return accumulate(node->data.begin(), node->data.end(), val);
}
int NodeValue(Node * node)
{
	if (node->childs.empty()) return accumulate(node->data.begin(), node->data.end(), 0);
	int val = 0;
	for (auto i : node->data) { if (i >= 1 && i <= node->n) val += NodeValue(node->childs[i - 1]); }
	return val;
}

int main()
{
	//read input file
	Node * root;
	ifstream is("input.txt"); root = ReadNode(is);

	cout << "Day8 Answer1: " << NodeSum(root) << endl;
	cout << "Day8 Answer2: " << NodeValue(root) << endl;


	return 0;
}
