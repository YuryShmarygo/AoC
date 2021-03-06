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
typedef vector<string> Tracs;
typedef list<Cart> Carts;
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
struct CartCrashException : exception
{
	Cart crash;
	CartCrashException(Cart& c) : crash(c) {}
};
void Move(Cart& p, Carts& carts, Tracs& tracs)
{
	switch (p.d)
	{
	case '^': p.y--; break;
	case 'v': p.y++; break;
	case '<': p.x--; break;
	case '>': p.x++; break;
	};
	Dir(p, tracs[p.y][p.x]);
}

void Move(Carts& carts, Tracs& tracs, bool remove)
{
	auto find_dup = [](Carts::iterator it, Cart& c) {return &c != &(*it) && c.x == it->x && c.y == it->y; };
	carts.sort([](auto p1, auto p2) {return p1.y < p2.y || p1.y == p2.y && p1.x < p2.x; });
	for (auto it = carts.begin(); it != carts.end(); )
	{
		Move(*it, carts, tracs);
		if (remove)
		{
			auto it2 = find_if(carts.begin(), carts.end(), bind(find_dup, it, _1));
			if (it2 != carts.end())
			{
				carts.erase(it2);
				it = carts.erase(it);
			}
			else
				++it;
		}
		else
		{
			if (find_if(carts.begin(), carts.end(), bind(find_dup, it, _1)) != carts.end())
				throw CartCrashException(*it);
			++it;
		}
	}
}

Carts FindCarts(Tracs& tracs)
{
	Carts carts;
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
void Print(const Carts& carts, const Tracs& input)
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
Cart Crash(Carts carts, Tracs input, bool remove)
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
	catch (CartCrashException& e)
	{
		return e.crash;
	}
}
void test()
{
	vector<string> input = {R"(/->-\        )",
							R"(|   |  /----\)",
							R"(| /-+--+-\  |)",
							R"(| | |  | v  |)",
							R"(\-+-/  \-+--/)",
							R"(  \------/   )" };
	auto carts = FindCarts(input);
	auto crash = Crash(carts, input, false);
	cout << "test: " << crash.x << "," << crash.y << endl;
}
void test2()
{
	vector<string> input = {R"(/>-<\  )",
							R"(|   |  )",
							R"(| /<+-\)",
							R"(| | | v)",
							R"(\>+</ |)",
							R"(  |   ^)",
							R"(  \<->/)"};
	auto carts = FindCarts(input);
	auto crash = Crash(carts, input, true);
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
	Carts carts = FindCarts(input);

	Cart crash = Crash(carts, input, false);
	cout << "Day13 Answer1: " << crash.x << "," << crash.y << endl;
	Cart last = Crash(carts, input, true);
	cout << "Day13 Answer2: " << last.x << "," << last.y << endl;

	return 0;
}
