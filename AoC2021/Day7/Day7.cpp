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
namespace rv = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

istream& operator>>(istream& is, vector<int>& pos)
{
	string num;
	while (getline(is, num, ',') && !num.empty())
		pos.push_back(stoi(num));
	return is;
}
uint64_t Align(const vector<int>& pos, function<uint64_t(int p1, int p2)> dist)
{
	auto sum_dist = [&](int i) { return accumulate(pos.begin(), pos.end(), 0ULL, [i, &dist](auto sum, auto p) { return sum + dist(i, p); }); };
	auto m = r::iota_view(*r::min_element(pos), *r::max_element(pos)) | rv::transform(sum_dist);
	return *r::min_element(m);
}
uint64_t plain_dist(int p1, int p2) { return abs(p1 - p2); };
uint64_t progr_dist(int p1, int p2) { uint64_t d = abs(p1 - p2); return d * (d + 1) / 2; };

void Test();
int main()
{
	Test();
	ifstream is("Day7.txt");
	vector<int> input; is >> input;

	auto start = high_resolution_clock::now();
	cout << "Day7 Answer1: " << Align(input, plain_dist); cout << " took " << ElapsedMs(start) << "ms" << endl;
	start = high_resolution_clock::now();
	cout << "Day7 Answer2: " << Align(input, progr_dist); cout << " took " << ElapsedMs(start) << "ms" << endl;
}

void Test()
{
	auto test = stringstream(R"(16,1,2,0,4,2,7,1,2,14)");
	vector<int> input; test >> input;

	cout << "test 1: " << Align(input, plain_dist) << endl;
	cout << "test 2: " << Align(input, progr_dist) << endl;
}