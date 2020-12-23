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

template<int N> int mod_n(int n)
{ return n < 0 ? N + n : n >= N ? n - N : n; }

int mod_n(int min, int max, int n)
{
	return n < min ? max - (min - n - 1) : n > max ? min + (n - max - 1) : n;
}
typedef list<int> Cups;
typedef vector<Cups::iterator> Cache;
ostream& operator<<(ostream& os, const Cups& cups) { copy(cups.begin(), cups.end(), ostream_iterator<int>(cout)); return os; }
struct riterator : iterator<bidirectional_iterator_tag, int>
{
	Cups& cups;
	Cups::iterator cur;
	riterator(Cups& _cups, Cups::iterator _cur) : cups(_cups), cur(_cur) {}
	riterator& operator=(const riterator& r)
	{
		cur = r.cur;
		return *this;
	}
	riterator& operator=(const Cups::iterator& it)
	{
		cur = it == cups.end() ? cups.begin() : it;
		return *this;
	}
	riterator& operator++()
	{
		cur = next(cur);
		if (cur == cups.end())
			cur = cups.begin();
		return *this;
	}
	riterator operator++(int) { riterator tmp(*this); operator++(); return tmp; }
	riterator operator+(int n) const { riterator tmp(*this); for (; n > 0; --n) ++tmp; return tmp; }
	bool operator==(const riterator& r) const { return cur == r.cur; }
	bool operator!=(const riterator& r) const { return cur != r.cur; }
	int& operator*()
	{
		return *cur;
	}
	operator Cups::iterator() { return cur; }
};
pair<riterator, bool> find_r(riterator start, riterator end, int n, const Cache& cache)
{
	if (find(start, end, n) != end)
		return make_pair(end, false);
	return make_pair(riterator(start.cups, cache[n-1]), true);
}
void splice(Cups::iterator dst, riterator start, riterator end)
{
	for (; start != end; )
	{
		Cups::iterator tmp = start + 1;
		start.cups.splice(dst, start.cups, start);
		start = tmp;
	}
}
void move(riterator cur, int max, const Cache& cache)
{
	auto start = cur + 1;
	auto end = start + 3;
	int dst_num = mod_n(1, max, *cur - 1);
	auto dst = find_r(start, end, dst_num, cache);
	while (!dst.second)
	{
		dst_num = mod_n(1, max, dst_num - 1);
		dst = find_r(start, end, dst_num, cache);
	}
	++dst.first;
	splice(dst.first, start, end);
}

int task1(const Cups& input)
{
	Cache cache(9);
	Cups cups;
	for (auto& c : input)
		cache[c - 1] = cups.insert(cups.end(), c);
	riterator cur(cups, cups.begin());
	for (int i = 0; i < 100; ++i)
	{
		move(cur, 9, cache);
		//cout << i << ": cur=" << *cur << " " << cups << endl;
		++cur;
	}
	riterator end(cups, cache[0]);
	riterator it = end + 1;
	stringstream s;
	for (; it != end; ++it)
		s << *it;
	return stoi(s.str());
}
long long task2(const Cups& input)
{
	Cache cache(1000000);
	Cups cups;
	for (auto& c : input)
		cache[c-1] = cups.insert(cups.end(), c);
	for (int i = 10; i <= 1000000; ++i)
		cache[i-1] = cups.insert(cups.end(), i);

	riterator cur(cups, cups.begin());
	for (int i = 0; i < 10000000; ++i)
	{
		move(cur, 1000000, cache);
		++cur;
	}
	riterator it(cups, cache[0]);
	return (long long)(*(it + 1)) * (long long)(*(it + 2));
}

void Test();
int main()
{
	Test();// return 0;
	Cups input = { 5,8,3,9,7,6,2,4,1 };

	cout << "Day23 Answer1: " << task1(input) << endl;
	cout << "Day23 Answer2: " << task2(input) << endl;
}

void Test()
{
	Cups test = { 3,8,9,1,2,5,4,6,7 };
	cout << "Test1: " << task1(test) << endl;
	cout << "Test2: " << task2(test) << endl;
}