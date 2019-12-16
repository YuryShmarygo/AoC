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
#include <boost/algorithm/string.hpp>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

struct Comp
{
    Comp() : elem(), qty(0) {}
    Comp(string _elem, long long _qty) :elem(_elem), qty(_qty) {}

    string elem;
    long long qty;
};

struct Reaction
{
    vector<Comp> in;
    Comp out;
};

istream& operator>>(istream& is, Reaction& r)
{
    string line;
    getline(is, line);
    if (line.size() == 0)
        return is;

    smatch g;
    if (!regex_match(line, g, regex("((?:(?:\\d+) (?:\\w+),* )+)=> (\\d+) (\\w+)")))
        throw exception(C("Invalid line: " << line));

    r.out = Comp(g[3], stoi(g[2]));
    vector<string> i_str;
    boost::split(i_str, g[1].str(), boost::is_any_of(","));
    for (auto s : i_str)
    {
        smatch gi;
        if (!regex_match(s, gi, regex(" *(\\d+) (\\w+) *")))
            throw exception(C("Invalid string: " << s));
        r.in.push_back(Comp(gi[2], stoi(gi[1])));
    }

    return is;
}

typedef map<string, Reaction> Reactions;
typedef map<string, long long> Elements;

istream& operator>>(istream& is, Reactions& input)
{
    while (is)
    {
        Reaction r;
        is >> r;
        if (r.out.elem.size() > 0)
            input[r.out.elem] = r;
    }
    return is;
}
Elements Brue1(Reactions& r, Comp c, Elements& s)
{
    Elements res;
    auto& b = r[c.elem];
    auto it = s.find(c.elem);
    auto req = c.qty;
    if (it != s.end())
    {
        if (c.qty <= it->second)
        {
            it->second -= c.qty;
            if (it->second == 0)
                s.erase(it);
            return res;
        }
        else
        {
            req -= it->second;
            s.erase(it);
        }
    }
    auto times = (req + b.out.qty -1) / b.out.qty;
    auto spare =  b.out.qty * times - req;
    if (spare > 0)
        s[c.elem] += spare;
    for (auto& i : b.in)
    {
        auto qty = i.qty * times;
        Elements::iterator st = s.find(i.elem);
        if (st != s.end())
        {
            if (st->second > qty)
            {
                st->second -= qty;
                qty = 0;
            }
            else
            {
                qty -= st->second;
                s.erase(st);
            }
        }
        if (qty > 0)
            res[i.elem] += qty;
    }
    return res;
}
long long Brue(Reactions& r, Comp c)
{
    auto& b = r[c.elem];
    Elements spare;
    Elements cur = Brue1(r, c, spare);
    while (!all_of(cur.begin(), cur.end(), [](auto e) {return e.first == "ORE"; }))
    {
        Elements nc;
        for (auto it = cur.begin(); it != cur.end(); ++it)
        {
            if (it->first != "ORE")
            {
                auto tmp = Brue1(r, Comp(it->first, it->second), spare);
                for_each(tmp.begin(), tmp.end(), [&](auto t) {nc[t.first] += t.second; });
            }
            else
            {
                nc[it->first] += it->second;
            }
        }
        cur = nc;
    }
    return cur.begin()->second;
}
long long MaxBrue(Reactions& r, long long ore_st, long long ore_f)
{
    long long low = ore_st / ore_f;
    long long up = low + low + low;
    while (up - low > 1)
    {
        long long cur = low + (up - low) / 2;;
        auto res = Brue(r, Comp("FUEL", cur));
        if (res == ore_st)
            return cur;
        if (res < ore_st)
            low = cur;
        else
            up = cur;
    }
    return low;
}

void tests();
int main()
{
    //tests(); return 0;
    ifstream is("Day14.txt");
    Reactions input;
    is >> input;

    auto ore = Brue(input, Comp("FUEL", 1));
    cout << "Day14, answer1: " << ore << endl;

    long long st = 1000000000000;
    cout << "Day14, answer2: " << MaxBrue(input, st, ore) << endl;
    return 0;
}

void test1()
{
    stringstream is("10 ORE => 10 A\n\
1 ORE => 1 B\n\
7 A, 1 B => 1 C\n\
7 A, 1 C => 1 D\n\
7 A, 1 D => 1 E\n\
7 A, 1 E => 1 FUEL");
    Reactions input;
    is >> input;

    auto res = Brue(input, Comp("FUEL", 1));
    cout << "Test1: " << res << endl;
}
void test2()
{
    stringstream is("9 ORE => 2 A\n\
8 ORE => 3 B\n\
7 ORE => 5 C\n\
3 A, 4 B => 1 AB\n\
5 B, 7 C => 1 BC\n\
4 C, 1 A => 1 CA\n\
2 AB, 3 BC, 4 CA => 1 FUEL");
    Reactions input;
    is >> input;

    auto res = Brue(input, Comp("FUEL", 1));
    cout << "Test2: " << res << endl;
}
void tests()
{
    test1();
    test2();
}