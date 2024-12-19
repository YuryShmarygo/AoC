#define _REGEX_MAX_COMPLEXITY_COUNT 0
#define _REGEX_MAX_STACK_COUNT 0

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
#include <array>
#include <cassert>

using namespace std;
using namespace chrono;
namespace r = std::ranges;
namespace v = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
void test();
vector<string> read_strings(istream& is)
{
	istream_iterator<string> start(is), end;
	return vector<string>(start, end);
}
template<typename T>
struct point_t
{
	T x;
	T y;
	bool operator==(const point_t<T>&) const = default;
	auto operator<=>(const point_t& point) const = default;
	bool operator<(const point_t&) const = default;
};
template<typename T>
point_t<T> operator+(const point_t<T>& p1, const point_t<T>& p2)
{ return {p1.x + p2.x, p1.y + p2.y}; }
template<typename T>
point_t<T> operator-(const point_t<T>& p1, const point_t<T>& p2)
{ return {p1.x - p2.x, p1.y - p2.y}; }

typedef point_t<int> point;