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

struct Bus
{
	int offset;
	int id;
	bool operator<(const Bus& b) const { return id > b.id; }
};
struct Schedule
{
	int timestamp;
	vector<Bus> buses;
};
Schedule ReadInput(istream& is)
{
	Schedule s;
	is >> s.timestamp;
	string bus;
	int i = 0;
	while (getline(is, bus, ','))
	{
		if (bus != "x")
			s.buses.push_back(Bus{ i, stoi(bus) });
		++i;
	}
	return s;
}

long long get_schedule(const Schedule& s)
{
	int next = s.timestamp + s.timestamp;
	long long res = 0;
	for (auto& b : s.buses)
	{
		int n = s.timestamp / b.id;
		int rest = s.timestamp % b.id;
		int cur = (n + 1) * b.id;
		if (rest == 0)
			cur -= b.id;
		if (cur < next)
		{
			next = cur;
			res = (long long)(next - s.timestamp) * b.id;
		}
	}
	return res;
}

long long get_schedule2(const Schedule& s)
{
	vector<Bus> buses(s.buses);
	sort(buses.begin(), buses.end());

	auto it = buses.begin();
	long long cur_d = it->id;
	long long cur = (it->offset / it->id + 1) * it->id - it->offset;
	++it;
	while (it != buses.end())
	{
		if ((cur + it->offset) % it->id == 0)
		{
			cur_d *= (it->id);
			++it;
			continue;
		}
		cur += cur_d;
	}
	return cur;
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day13.txt");
	Schedule s = ReadInput(is);
	//cout << "Read " << s.buses.size() << " buses" << endl;

	cout << "Day13 Answer1: " << get_schedule(s) << endl;
	cout << "Day13 Answer2: " << get_schedule2(s) << endl;
}

void Test()
{
	stringstream test(R"(939
7,13,x,x,59,x,31,19)");
	Schedule s = ReadInput(test);
	cout << "Test 1: " << get_schedule(s) << endl;
	cout << "Test 2: " << get_schedule2(s) << endl;
	test = stringstream(R"(939
17,x,13,19)");
	s = ReadInput(test);
	cout << "Test 3: " << get_schedule2(s) << endl;
	test = stringstream(R"(939
67,7,59,61)");
	s = ReadInput(test);
	cout << "Test 4: " << get_schedule2(s) << endl;
	test = stringstream(R"(939
67,x,7,59,61)");
	s = ReadInput(test);
	cout << "Test 5: " << get_schedule2(s) << endl;
	test = stringstream(R"(939
67,7,x,59,61)");
	s = ReadInput(test);
	cout << "Test 6: " << get_schedule2(s) << endl;
	test = stringstream(R"(939
1789,37,47,1889)");
	s = ReadInput(test);
	cout << "Test 7: " << get_schedule2(s) << endl;
}