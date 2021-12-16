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
#include <chrono>
#include <ranges>

using namespace std;
using namespace chrono;
namespace r = std::ranges;
namespace v = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

vector<char> h2b(const string& s)
{
	map<char, vector<char>> h{
		{'0', {0,0,0,0}},
		{'1', {0,0,0,1}},
		{'2', {0,0,1,0}},
		{'3', {0,0,1,1}},
		{'4', {0,1,0,0}},
		{'5', {0,1,0,1}},
		{'6', {0,1,1,0}},
		{'7', {0,1,1,1}},
		{'8', {1,0,0,0}},
		{'9', {1,0,0,1}},
		{'A', {1,0,1,0}},
		{'B', {1,0,1,1}},
		{'C', {1,1,0,0}},
		{'D', {1,1,0,1}},
		{'E', {1,1,1,0}},
		{'F', {1,1,1,1}},
	};
	vector<char> b;
	for (auto c : s)
		b.insert(b.end(), h[c].begin(), h[c].end());
	return b;
}
int64_t b2i(vector<char>::const_iterator start, vector<char>::const_iterator end)
{
	int64_t i = 0;
	for (; start != end; ++start)
	{
		i <<= 1;
		i += (*start);
	}
	return i;
}
struct Header
{
	int version;
	int type;
	int64_t parse(const vector<char>& b)
	{ return parse(b.begin()) - b.begin(); }
	vector<char>::const_iterator parse(vector<char>::const_iterator it)
	{
		version = (int)b2i(it, it + 3);
		type = (int)b2i(it + 3, it + 6);
		return it + 6;
	}
};
struct Packet
{
	Header header;
	int64_t value;
	list<Packet> sub;
	vector<char>::const_iterator parse(vector<char>::const_iterator it)
	{
		it = header.parse(it);
		if (header.type == 4)
		{
			value = 0;
			int group_type = 1;
			while (group_type != 0)
			{
				group_type = (*it++);
				for (int i = 0; i < 4; ++i)
				{
					value <<= 1;
					value += (*it++);
				}
			}
		}
		else
		{
			int length_type_id = (*it++);
			if (length_type_id == 0)
			{
				int64_t length = b2i(it, it + 15);
				it += 15;
				while (length > 0)
				{
					Packet subp;
					auto ite = subp.parse(it);
					sub.push_back(subp);
					length -= (ite - it);
					it = ite;
				}
			}
			else if (length_type_id == 1)
			{
				int64_t sub_packets = b2i(it, it + 11);
				it += 11;
				for (; sub_packets > 0; --sub_packets)
				{
					Packet subp;
					it = subp.parse(it);
					sub.push_back(subp);
				}
			}
		}
		return it;
	}
};
ostream& operator<<(ostream& os, const Packet& p)
{ 
	os << "[version: " << p.header.version << ", type: " << p.header.type << "]: value: " << p.value; 
	os << " subpackets: " << p.sub.size(); if (p.sub.size() > 0) os << "\n\t";
	r::copy(p.sub, ostream_iterator<Packet>(os, "\n\t"));

	return os; 
}
uint64_t sum_versions(const Packet& p)
{
	return accumulate(p.sub.begin(), p.sub.end(), (uint64_t)p.header.version, [](auto sum, auto& p) { return sum + sum_versions(p); });
}
uint64_t calculate(Packet& p)
{
	for (auto& sub : p.sub)
		calculate(sub);
	switch (p.header.type)
	{
	case 0: p.value = accumulate(p.sub.begin(), p.sub.end(), (uint64_t)0, [](auto sum, auto& p) { return sum + p.value; }); break;
	case 1: p.value = accumulate(p.sub.begin(), p.sub.end(), (uint64_t)1, [](auto sum, auto& p) { return sum * p.value; }); break;
	case 2: p.value = r::min_element(p.sub, [](auto& p1, auto& p2) {return p1.value < p2.value; })->value; break;
	case 3: p.value = r::max_element(p.sub, [](auto& p1, auto& p2) {return p1.value < p2.value; })->value; break;
	case 5: p.value = p.sub.begin()->value > next(p.sub.begin())->value ? 1 : 0; break;
	case 6: p.value = p.sub.begin()->value < next(p.sub.begin())->value ? 1 : 0; break;
	case 7: p.value = p.sub.begin()->value == next(p.sub.begin())->value ? 1 : 0; break;
	}
	return p.value;
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day16.txt");
	string input; is >> input;

	auto t_start = high_resolution_clock::now();
	Packet p; p.parse(h2b(input).begin());
	cout << "Day16 Answer1: " << sum_versions(p); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day16 Answer2: " << calculate(p); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	{
		Packet p;
		p.parse(h2b("D2FE28").begin());
		cout << "Test1: " << p << endl;
	}
	{
		Packet p;
		p.parse(h2b("38006F45291200").begin());
		cout << "Test2: " << p << endl;
	}
	{
		Packet p;
		p.parse(h2b("EE00D40C823060").begin());
		cout << "Test3: " << p << endl;
	}
	{
		Packet p;
		p.parse(h2b("8A004A801A8002F478").begin());
		cout << "Test4: " << sum_versions(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("620080001611562C8802118E34").begin());
		cout << "Test5: " << sum_versions(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("C0015000016115A2E0802F182340").begin());
		cout << "Test6: " << sum_versions(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("A0016C880162017C3686B18A3D4780").begin());
		cout << "Test7: " << sum_versions(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("C200B40A82").begin());
		cout << "Test8: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("04005AC33890").begin());
		cout << "Test9: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("880086C3E88112").begin());
		cout << "Test10: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("CE00C43D881120").begin());
		cout << "Test11: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("D8005AC2A8F0").begin());
		cout << "Test12: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("F600BC2D8F").begin());
		cout << "Test13: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("9C005AC2F8F0").begin());
		cout << "Test14: " << calculate(p) << endl;
	}
	{
		Packet p;
		p.parse(h2b("9C0141080250320F1802104A08").begin());
		cout << "Test15: " << calculate(p) << endl;
	}
}