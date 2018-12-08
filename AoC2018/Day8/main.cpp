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

vector<Node*> input;
istream& ReadNode(istream& is, Node* node)
{
	input.push_back(node);
	is >> node->n >> node->m;
	generate_n(back_inserter(node->childs), node->n, []() {return new Node(); });
	for (auto c : node->childs) { ReadNode(is, c); }
	copy_n(istream_iterator<int>(is), node->m, back_inserter(node->data));
	return is;
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
	Node root;
	ifstream is("input.txt");  ReadNode(is, &root);
	cout << "Read " << input.size() << " nodes" << endl;

	int sum = 0; for (auto i : input) for (auto j : i->data) sum += j;
	cout << "Day8 Answer1: " << sum << endl;
	cout << "Day8 Answer2: " << NodeValue(&root) << endl;


	return 0;
}
