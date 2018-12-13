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

struct Cart
{
	int x;
	int y;
	char d;
	char t;
};
void Print(vector<Cart>& carts, vector<string>& input);

Cart& Dir(Cart& p, char d)
{
	switch (d)
	{
	case '\\':
			switch (p.d)
			{
			case '^': p.d = '<'; break;
			case '>': p.d = 'v'; break;
			case '<': p.d = '^'; break;
			case 'v': p.d = '>'; break;
			};
		break;
	case '/':
		switch (p.d)
		{
		case '^': p.d = '>'; break;
		case '<': p.d = 'v'; break;
		case '>': p.d = '^'; break;
		case 'v': p.d = '<'; break;
		};
		break;
	case '+':
		switch (p.t)
		{
		case 'l': p.t = 's'; 
			switch (p.d)
			{
			case '^': p.d = '<'; break;
			case '<': p.d = 'v'; break;
			case '>': p.d = '^'; break;
			case 'v': p.d = '>'; break;
			};
			break;
		case 'r': p.t = 'l';
			switch (p.d)
			{
			case '^': p.d = '>'; break;
			case '<': p.d = '^'; break;
			case '>': p.d = 'v'; break;
			case 'v': p.d = '<'; break;
			};
			break;
		case 's': p.t = 'r'; break;
		};
		break;
	};
	return p;
}
struct CrashException : exception
{
	Cart crash;
	CrashException(Cart& c) : crash(c) {}
};
struct LastCartException : exception
{
	Cart last;
	LastCartException(Cart& c) : last(c) {}
};
void Check(Cart& c, vector<Cart>& carts)
{
	for (Cart& p : carts)
		if (&p != &c && p.x == c.x && p.y == c.y)
			throw CrashException(c);
}
void Move(Cart& p, vector<Cart>& carts, vector<string>& tracs, bool remove)
{
	switch (p.d)
	{
	case '^': p.y--; break;
	case 'v': p.y++; break;
	case '<': p.x--; break;
	case '>': p.x++; break;
	};
	Dir(p, tracs[p.y][p.x]);
	if (!remove)
		Check(p, carts);
}

void Move(vector<Cart>& carts, vector<string>& tracs, bool remove)
{
	sort(carts.begin(), carts.end(), [](auto p1, auto p2) {return p1.y < p2.y || p1.y == p2.y && p1.x < p2.x; });
	for (auto it = carts.begin(); it != carts.end(); )
	{
		Move(*it, carts, tracs, remove);
		//remove if it crashed
		if (remove)
		{
			if (next(it) != carts.end() && it->x == next(it)->x && it->y == next(it)->y)
				it = carts.erase(it, next(it, 2));
			else if (it != carts.begin() && it->x == prev(it)->x && it->y == prev(it)->y)
				it = carts.erase(prev(it), next(it));
			else
				++it;
		}
		else
		{
			++it;
		}
	}
}

vector<Cart> FindCarts(vector<string>& tracs)
{
	vector<Cart> carts;
	for (int y = 0; y < tracs.size(); y++)
	{
		for (int x = 0; x < tracs[y].size(); x++)
		{
			switch (tracs[y][x])
			{
			case '^': 
			case 'v': 
				carts.push_back(Cart{ x,y,tracs[y][x],'l' }); tracs[y][x] = '|'; break;
			case '<':
			case '>': 
				carts.push_back(Cart{ x,y,tracs[y][x],'l' }); tracs[y][x] = '-'; break;
			};
		}
	}
	return carts;
}
void Print(const vector<Cart>& carts, const vector<string>& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		string s = input[i];
		for (auto c : carts)
		{
			if (c.y == i)
				s[c.x] = c.d;
		}
		cout << s << endl;
	}
	cin.get();
}
Cart Crash(vector<Cart> carts, vector<string> input, bool remove)
{
	//Print(carts, input);
	
	try {
		while (true)
		{
			Move(carts, input, remove);
			if (remove && carts.size() == 1)
				return carts.front();
			//Print(carts, input);
		}
	}
	catch (CrashException& e)
	{
		return e.crash;
	}
	catch (LastCartException& e)
	{
		return e.last;
	}
}
void test()
{
	vector<string> input = {"/->-\\        ",
							"|   |  /----\\",
							"| /-+--+-\\  |",
							"| | |  | v  |",
							"\\-+-/  \\-+--/",
							"  \\------/   "};
	vector<Cart> carts = FindCarts(input);
	Cart crash = Crash(carts, input, false);
	cout << "test: " << crash.x << "," << crash.y << endl;
}
void test2()
{
	vector<string> input = {"/>-<\\  ",
							"|   |  ",
							"| /<+-\\",
							"| | | v",
							"\\>+</ |",
							"  |   ^",
							"  \\<->/"};
	vector<Cart> carts = FindCarts(input);
	Cart crash = Crash(carts, input, true);
	cout << "test2: " << crash.x << "," << crash.y << endl;
}
int main()
{
	test();
	test2(); 

	vector<string> input;
	ifstream is("input.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			break;
		input.push_back(line);
	}
	cout << "Read " << input.size() << " lines" << endl;
	vector<Cart> carts = FindCarts(input);

	Cart crash = Crash(carts, input, false);
	cout << "Day13 Answer1: " << crash.x << "," << crash.y << endl;
	Cart last = Crash(carts, input, true);
	cout << "Day13 Answer2: " << last.x << "," << last.y << endl;

	return 0;
}
