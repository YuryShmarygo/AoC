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
#include "IntComp.h"

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

vector<string> to_strings(const vector<long long>& numbers)
{
    vector<string> plan(1, "");
    for (auto& r : numbers)
    {
        if (r == 10)
            plan.push_back("");
        else
            plan.back().push_back((char)r);
    }
    int w=0;
    for_each(plan.begin(), plan.end(), [&](auto& s) {w = max(w, (int)s.size()); });
    for_each(plan.begin(), plan.end(), [&](auto& s) {s.resize(w, '.'); });
    return plan;
}

long task1(const vector<string>& plan)
{
    long sum = 0;
    for (int y = 1; y < plan.size() - 1; ++y)
    {
        for (int x = 1; x < plan[y].size() - 1; ++x)
        {
            if (plan[y][x] != '.' && plan[y - 1][x] != '.' && plan[y + 1][x] != '.' && plan[y][x - 1] != '.' && plan[y][x + 1] != '.')
                sum += x * y;
        }
    }
    return sum;
}

struct Pos
{
    char x;
    char y;
    bool operator==(const Pos& p) const
    { return x == p.x && y == p.y; }
    bool operator<(const Pos& p) const
    { return x < p.x || x == p.x && y < p.y; }
    Pos operator+(char d) const
    {
        static map<char, function<Pos(const Pos& p)>> moves = { 
            {'>',[](auto& p) {return Pos{p.x + 1, p.y}; } },
            {'<',[](auto& p) {return Pos{p.x - 1, p.y}; } }, 
            {'^',[](auto& p) {return Pos{p.x, p.y - 1}; } }, 
            {'v',[](auto& p) {return Pos{p.x, p.y + 1}; } } };
        return moves[d](*this);
    }
};
ostream& operator<<(ostream& o, const Pos& p) { o << "[x: " << (int)p.x << ", y: " << (int)p.y << "]"; return o; }

list<pair<char, char>> find_turns(const Pos& p, char d, const vector<string>& plan)
{
    list<pair<char, char>> res;
    static map<char, pair<char, char>> turns = { {'^', {'<','>'}}, {'>', {'^','v'}}, {'v', {'>','<'}}, {'<', {'v','^'}} };
    auto t = p + d;
    if (t.y >= 0 && t.y < plan.size() && t.x >= 0 && t.x < plan[t.y].size() && plan[t.y][t.x] == '#')
        res.push_back(make_pair(' ', d));
    auto l = turns[d].first;
    t = p + l;
    if (t.y >= 0 && t.y < plan.size() && t.x >= 0 && t.x < plan[t.y].size() && plan[t.y][t.x] == '#')
        res.push_back(make_pair('L', l));
    auto r = turns[d].second;
    t = p + r;
    if (t.y >= 0 && t.y < plan.size() && t.x >= 0 && t.x < plan[t.y].size() && plan[t.y][t.x] == '#')
        res.push_back(make_pair('R', r));
    return res;
}
struct Step
{
    char t;
    char m;
};
typedef list<Step> Path;
int length(const Path& p) { return accumulate(p.begin(), p.end(), (int)0, [](auto sum, auto& step) {return sum + (step.m > 9 ? 5 : 4); }) - 1; }
ostream& operator<<(ostream& o, const Step& s) { o << s.t << ',' << (int)s.m; return o; }
istream& operator>>(istream& i, Step& s) { int num; char c;  i >> s.t >> c >> num; s.m = (char)num; return i; }
ostream& operator<<(ostream& o, const Path& p) { copy(p.begin(), p.end(), ostream_iterator<Step>(o, ",")); return o; }
istream& operator>>(istream& i, Path& p) 
{
    Step step;
    while (i >> step)
    {
        p.push_back(step);
        char c;
        i >> c;
    }
    return i; 
}
ostream& operator<<(ostream& o, const vector<string>& p) { copy(p.begin(), p.end(), ostream_iterator<string>(o, "\n")); return o; }
ostream& operator<<(ostream& o, const list<string>& p) { copy(p.begin(), p.end(), ostream_iterator<string>(o, "\n")); return o; }
string to_hex_string(const Path& p) { stringstream s; for_each(p.begin(), p.end(), [&](auto& p) { s << p.t << hex << (int)p.m; }); return s.str(); }
pair<Pos, char> find_start(const vector<string>& plan)
{
    for (char y = 0; y < plan.size(); ++y)
        for (char x = 0; x < plan[y].size(); ++x)
            if (plan[y][x] != '.' && plan[y][x] != '#')
                return make_pair(Pos{ x, y }, plan[y][x]);
    throw exception(C("start not found." << endl << plan << endl));
}
set<Pos> get_visits(const vector<string>& plan)
{
    set<Pos> res;
    for (char y = 0; y < plan.size(); ++y)
        for (char x = 0; x < plan[y].size(); ++x)
            if (plan[y][x] == '#')
                res.insert({ x, y });
    return res;
}
struct State
{
    Step step;
    Pos pos;
    char dir;
    Path path;
    vector<vector<char>> visited;
    char visit(const Pos& p) { return ++visited[p.y][p.x]; }
    int count_visited()
    { return accumulate(visited.begin(), visited.end(), (int)0, [](auto s, auto& r) {return s + (int)count_if(r.begin(), r.end(), [](auto& c) {return c > 0; }); }); }
};
ostream& operator<<(ostream& o, const State& s) { o << "[visited: " << s.visited.size() << ", step: " << s.step << ", pos: " << s.pos << ", dir: " << s.dir << ", path: " << s.path << "]"; return o;}
bool Move(State& state, const pair<char, char>& t)
{
    if (t.first == ' ')
    {
        ++state.step.m;
    }
    else
    {
        if (state.step.m > 0)
            state.path.push_back(state.step);
        state.step = { t.first, 1 };
        state.dir = t.second;
    }
    state.pos = state.pos + state.dir;
    auto vis = state.visit(state.pos);
    return vis < 3;
}
list<string> split_path(const Path& path)
{
    static vector<string> rep{ "A,", "B,", "C," };
    list<string> res;
    auto str = S(path);
    smatch g;
    if (regex_match(str, g, regex("^(.{1,20})\\1*(.{1,20})(?:\\1|\\2)*(.{1,20})(?:\\1|\\2|\\3)*$")))
    {
        string replaced = str;
        for (int i = 1; i < g.size(); ++i)
        {
            res.push_back(g[i].str());
            replaced = regex_replace(replaced, regex(g[i].str()), rep[i-1]);
        }
        res.insert(res.begin(), replaced);
    }
    return res;
}
list<string> create_paths(const vector<string>& plan)
{
    int max_path = 11 * 20;
    auto st = find_start(plan);
    size_t to_visit = get_visits(plan).size();
    list<State> paths;
    paths.push_back({ {' ', 0}, st.first, st.second, {}, vector<vector<char>>(plan.size(), vector<char>(plan[0].size(),0)) });
    list<string> res;
    auto cur = paths.begin();

    while (!paths.empty())
    {
        //cout << "steps: " << cur.size() << endl;
        if (cur == paths.end())
            cur = paths.begin();
        if (cur->path.size() > 25 && length(cur->path) + (to_visit - cur->count_visited()) / 3 > max_path)
        {
            cur = paths.erase(cur);
            continue;
        }
        if (cur->count_visited() == to_visit)
        {
            res.push_back(S(cur->path));
            cout << "Found path: " << cur->path << endl;
            auto split = split_path(cur->path);
            if (split.size() > 0)
                return split;

            cur = paths.erase(cur);
            continue;
        }
        auto turns = find_turns(cur->pos, cur->dir, plan);
        if (turns.empty())
        {
            cur = paths.erase(cur);
            continue;
        }
        for (auto it = turns.rbegin(); it != turns.rend(); ++it)
        {
            if (next(it) == turns.rend())
            {
                if (!Move(*cur, *it))
                {
                    cur = paths.erase(cur);
                    continue;
                }
            }
            else
            {
                State state(*cur);
                if (Move(state, *it))
                    paths.push_back(state);
            }
        }
    }

    return res;
}

int main()
{
    ifstream is("Day17.txt");
    vector<long long> input;
    is >> input;

    Machine m(input);
    vector<long long> res;
    m.Run(res);

    auto plan = to_strings(res);
    cout << "Day17, task1: " << task1(plan) << endl;

    auto paths = create_paths(plan);
    cout << paths << endl;
    //input[0] = 2;
    //Machine m(input);
    //vector<long long> res;
    //m.Run(res);

    return 0;
}
