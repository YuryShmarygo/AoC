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
    char x;
    char y;
    bool operator==(const Pos& p) const
    { return x == p.x && y == p.y;}
    bool operator<(const Pos& p) const
    { return x < p.x || x == p.x && y < p.y; }
    Pos operator+(const Pos& p) const
    { return Pos{x + p.x, y + p.y}; }
};
typedef list<Pos> Path;

map<char, Pos> find_key_pos(const vector<string>& maze)
{
    map<char, Pos> key_pos;
    for (char y = 0; y < maze.size(); ++y)
        for (char x = 0; x < maze[y].size(); ++x)
            if (maze[y][x] >= 'a' && maze[y][x] <= 'z')
                key_pos.insert(make_pair(maze[y][x], Pos{ x, y }));
    return key_pos;
}
Pos find_start(const vector<string>& maze)
{
    for (char y = 0; y < maze.size(); ++y)
        for (char x = 0; x < maze[y].size(); ++x)
            if (maze[y][x] == '@')
                return { x, y };
    throw exception("Start not found");
}
/*
char can_move(const Pos& pos, const vector<string>& maze, const set<char>& keys)
{
    if (pos.y < 0 || pos.y >= maze.size() || pos.x < 0 || pos.x >= maze[pos.y].size() || maze[pos.y][pos.x] == '#')
        return -1;

    char block = maze[pos.y][pos.x];
    char key = tolower(block);
    bool isBlocked = block >= 'A' && block <= 'Z' && keys.find(key) == keys.end();
    if (isBlocked)
        return key;
    return 0;
}

Path find_path(const vector<string>& maze, const Pos& start, const Pos& dst, const set<char>& keys, set<char>& blockers)
{
    static vector<Pos> moves{ {1,0},{-1,0},{0,1},{0,-1} };
    struct Step
    {
        Path path;
        set<Pos> visited;
        char blocker;
    };
    list<Step> steps;
    steps.push_back({ {start}, {start}, 0 });
    auto cur = steps.begin();
    Path res;
    while (!steps.empty())
    {
        if (cur->path.back() == dst)
        {
            if (cur->blocker > 0)
                blockers.insert(cur->blocker);
            else if (res.empty())
                res = cur->path;
        }
        else
        {
            for (auto& move : moves)
            {
                auto step = *cur;
                auto new_pos = step.path.back() + move;
                if (step.visited.find(new_pos) != step.visited.end())
                    continue;
                char blocker = can_move(new_pos, maze, keys);
                if (blocker == -1)
                    continue;
                if (step.blocker == 0)
                    step.blocker = blocker;
                step.path.push_back(new_pos);
                step.visited.insert(new_pos);
                steps.push_back(step);
            }
        }
        cur = steps.erase(cur);
    }
    return res;
}
Path find_keys(const vector<string>& maze)
{
    auto start = find_start(maze);
    auto key_pos = find_key_pos(maze);
    struct Step
    {
        set<char> keys;
        Path path;
        set<char> blocked;
        map<char, set<char>> blockers;
        bool operator<(const Step& s) const
        //{ return keys.size() < s.keys.size() || keys.size() == s.keys.size() && path.size() > s.path.size(); }
        { return path.size() > s.path.size() || path.size() == s.path.size() && keys.size() < s.keys.size(); }
    };
    priority_queue<Step> steps;
    steps.push(Step{ set<char>(), {start} });
    while (!steps.empty())
    {
        auto cur = steps.top();
        steps.pop();

        if (cur.keys.size() == key_pos.size())
            return cur.path;

        map<char, Path> moves;
        for (auto& k : key_pos)
        {
            if (cur.keys.find(k.first) == cur.keys.end() && cur.blocked.find(k.first) == cur.blocked.end())
            {
                set<char> blocks;
                auto path = find_path(maze, cur.path.back(), k.second, cur.keys, blocks);
                if (!path.empty())
                    moves[k.first] = path;
                else
                {
                    cur.blocked.insert(k.first);
                    for (auto b : blocks)
                        cur.blockers[b].insert(k.first);
                }
            }
        }
        for (auto& m : moves)
        {
            Step step = cur;
            step.path.insert(step.path.end(), next(m.second.begin()), m.second.end());
            step.keys.insert(m.first);
            auto blocking = step.blockers.find(m.first);
            if (blocking != step.blockers.end())
            {
                for (auto b : blocking->second)
                    step.blocked.erase(b);
                step.blockers.erase(blocking);
            }

            steps.push(step);
        }
    }
    return Path();
}
*/

bool can_move(const Pos& pos, const vector<string>& maze, const set<char>& keys)
{
    return pos.y >= 0 && pos.y < maze.size() && pos.x >= 0 && pos.x < maze[pos.y].size() && maze[pos.y][pos.x] != '#'
        && (maze[pos.y][pos.x] < 'A' || maze[pos.y][pos.x] > 'Z' || keys.find(tolower(maze[pos.y][pos.x])) != keys.end());
}

map<char, Path> find_paths(const vector<string>& maze, const Pos& start, const  map<char, Pos>& dst, const set<char>& keys)
{
    static vector<Pos> moves{ {1,0},{-1,0},{0,1},{0,-1} };

    map<char, Pos> to_visit;
    for (auto& d : dst)
        if (keys.find(d.first) == keys.end())
            to_visit.insert(d);

    struct Step
    {
        Path path;
        set<Pos> visited;
    };
    list<Step> steps({ {{start}, {start}} });
    auto cur = steps.begin();
    map<char, Path> res;
    while (!dst.empty() && !steps.empty())
    {
        for (auto& move : moves)
        {
            auto step = *cur;
            auto new_pos = step.path.back() + move;
            if (step.visited.find(new_pos) != step.visited.end())
                continue;
            if (can_move(new_pos, maze, keys))
            {
                step.path.push_back(new_pos);
                step.visited.insert(new_pos);
                steps.push_back(step);
                char block = maze[new_pos.y][new_pos.x];
                if (block >= 'a' && block <= 'z')
                {
                    auto it = to_visit.find(block);
                    if (it != to_visit.end())
                    {
                        res[block] = step.path;
                        to_visit.erase(it);
                    }
                }
            }
        }
        cur = steps.erase(cur);
    }
    return res;
}
Path find_keys(const vector<string>& maze)
{
    auto start = find_start(maze);
    auto key_pos = find_key_pos(maze);
    struct Step
    {
        set<char> keys;
        Path path;
        bool operator==(const Step& s) const { return path.back() == s.path.back() && keys == s.keys; }
        bool operator<(const Step& s) const { return path.back() < s.path.back() || path.back() == s.path.back() && keys < s.keys; }
    };
    set<Step> steps;
    steps.insert(Step{ set<char>(), {start} });
    auto cur = steps.begin();
    Path res;
    while (!steps.empty())
    {
        if (cur == steps.end())
            cur = steps.begin();
        if (cur->keys.size() == key_pos.size())
        {
            if (res.empty() || res.size() > cur->path.size())
                res = cur->path;
        }
        if (res.empty() || res.size() > cur->path.size())
        {
            auto moves = find_paths(maze, cur->path.back(), key_pos, cur->keys);
            for (auto& m : moves)
            {
                Step step = *cur;
                step.path.insert(step.path.end(), next(m.second.begin()), m.second.end());
                step.keys.insert(m.first);
                auto it = steps.find(step);
                if (it != steps.end())
                {
                    if (it->path.size() > step.path.size())
                    {
                        steps.erase(it);
                        steps.insert(step);
                    }
                }
                else
                    steps.insert(step);
            }
        }
        cur = steps.erase(cur);
    }
    return res;
}

void Test();
int main()
{
    //Test(); return 0;
    ifstream is("Day18.txt");
    istream_iterator<string> start(is), end;
    vector<string> input(start, end);

    auto path = find_keys(input);
    cout << "Day18, task1: " << path.size() << endl;

    //cout << "Day18, task2: " << res[res.size()-1] << endl;

    return 0;
}

void Test()
{
    vector<string> test = {
"#########",
"#b.A.@.a#",
"#########" };
    cout << "Test 1: " << find_keys(test).size() - 1 << endl;

    test = {
"########################",
"#f.D.E.e.C.b.A.@.a.B.c.#",
"######################.#",
"#d.....................#",
"########################" };
    cout << "Test 2: " << find_keys(test).size() - 1 << endl;

    test = {
"########################",
"#...............b.C.D.f#",
"#.######################",
"#.....@.a.B.c.d.A.e.F.g#",
"########################" };
    cout << "Test 3: " << find_keys(test).size() - 1 << endl;

    test = {
"#################",
"#i.G..c...e..H.p#",
"########.########",
"#j.A..b...f..D.o#",
"########@########",
"#k.E..a...g..B.n#",
"########.########",
"#l.F..d...h..C.m#",
"#################" };
    cout << "Test 4: " << find_keys(test).size() - 1 << endl;

    test = {
"########################",
"#@..............ac.GI.b#",
"###d#e#f################",
"###A#B#C################",
"###g#h#i################",
"########################" };
    cout << "Test 5: " << find_keys(test).size() - 1 << endl;
}