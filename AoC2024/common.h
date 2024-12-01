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