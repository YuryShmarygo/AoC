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

struct Pos
{
	int x;
	int y;
	bool operator<(const Pos& p) const { return y < p.y || y == p.y && x < p.x; }
	int get_id() const { return y * 8 + x; }
	Pos operator+(int n) const { return Pos{ (x + n) % 8, y + (x + n) / 8 }; }
};
ostream& operator<<(ostream& o, const Pos& p) { o << "[r: " << (int)p.y << ", c: " << (int)p.x << "]"; return o; }

Pos get_seat(string seat)
{
	return Pos{
		accumulate(seat.begin() + 7, seat.end(), (int)0, [](int sum, char c) { return (sum << 1) + (c == 'R' ? 1 : 0); }),
		accumulate(seat.begin(), seat.begin() + 7, (int)0, [](int sum, char c) { return (sum << 1) + (c == 'B' ? 1 : 0); }) };
}
Pos find_seat(const vector<Pos>& seats)
{
	return *adjacent_find(seats.begin(), seats.end(), [](auto& s1, auto& s2) { return s2.get_id() - s1.get_id() > 1; }) + 1;
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day5.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	vector<Pos> seats;
	transform(input.begin(), input.end(), back_inserter(seats), [](auto& s) { return get_seat(s); });
	sort(seats.begin(), seats.end());
	cout << "Day5 Answer1: " << seats.back().get_id() << endl;

	cout << "Day5 Answer2: " << find_seat(seats).get_id() << endl;
}

void Test()
{
	vector<string> tests = { "FBFBBFFRLR", "FBFBBFBRLR", "BFFFBBFRRR", "FFFBBBFRRR","BBFFBBFRLL" };
	for (auto& test : tests)
		cout << test << " : " << get_seat(test) << endl;
}