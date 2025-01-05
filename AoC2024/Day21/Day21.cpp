#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) { return read_strings(is); }
typedef map<char, point> keypad_t;
vector<string> num_pad_str = {
    "789",
    "456",
    "123",
    " 0A" };
vector<string> mov_pad_str = {
    " ^A",
    "<v>" };
map<char, point> dir_map = { {'^',{0,-1}},{'<',{-1,0}},{'v',{0,1}},{'>',{1,0}} };
map<point, char> build_dir_key()
{
    map<point, char> res;
    for (auto& d : dir_map)
        res[d.second] = d.first;
    return res;
}
map<point, char> dir_key = build_dir_key();
keypad_t to_key_pad(const vector<string> pad_str)
{
    keypad_t res;
    for (int y = 0; y < pad_str.size(); ++y)
        for (int x = 0; x < pad_str[y].size(); ++x)
            res[pad_str[y][x]] = { x,y };
    return res;
}
keypad_t num_pad = to_key_pad(num_pad_str);
keypad_t mov_pad = to_key_pad(mov_pad_str);
vector<point> get_dir(const point& cur, const point& dest, keypad_t& target)
{
    vector<point> res;
    if (cur.x != dest.x)
    {
        point next = cur + (cur.x > dest.x ? point{ -1,0 } : point{ 1,0 });
        if (target[' '] != next)
            res.push_back(next);
    }
    if (cur.y != dest.y)
    {
        point next = cur + (cur.y > dest.y ? point{ 0,-1 } : point(0, 1));
        if (target[' '] != next)
            res.push_back(next);
    }
    return res;
}
string encode_path(const vector<point>& path)
{
    string res;
    for (auto it = path.begin(); it != prev(path.end()); ++it)
    {
        auto& p1 = *it;
        auto& p2 = *next(it);
        res.push_back(dir_key[p2 - p1]);
    }
    res.push_back('A');
    return res;
}
vector<string> get_path(char start, char end, keypad_t& target)
{
    point dest = target[end];
    map<int, vector<string>> res;
    list<vector<point>> paths = { {target[start]} };
    while (!paths.empty())
    {
        auto path = paths.front();
        paths.erase(paths.begin());
        if (target[end] == path.back())
        {
            res[(int)path.size()].push_back(encode_path(path));
            continue;
        }
        const point& cur = path.back();
        auto next = get_dir(cur, dest, target);
        for (auto d : next)
        {
            auto new_p = path;
            new_p.push_back(d);
            paths.push_back(new_p);
        }
    }
    return res.begin()->second;
}
vector<string> add_paths(const vector<string>& first, const vector<string>& second)
{
    vector<string> res; res.reserve(first.size() * second.size());
    if (first.size() == 0)
        res = second;
    for (auto& path : first)
        for (auto& next : second)
        {
            string join = path;
            join.append(next);
            res.push_back(join);
        }
    return res;
}
vector<string> get_path(string str, keypad_t& target)
{
    vector<string> res;
    char start = 'A';
    for (auto end : str)
    {
        auto next = get_path(start, end, target);
        res = add_paths(res, next);
        start = end;
    }
    return res;
}
vector<string> get_path(const vector<string>& codes, keypad_t& target)
{
    map<int, vector<string>> res;
    for (auto code : codes)
    {
        auto paths = get_path(code, target);
        int size = (int)paths.front().size();
        auto it = res.find(size);
        if (it == res.end())
            res[size] = paths;
        else
            it->second.insert(it->second.end(), paths.begin(), paths.end());
    }
    return res.begin()->second;
}
string encode(string code)
{
    auto res = get_path(code, num_pad);
    res = get_path(res, mov_pad);
    res = get_path(res, mov_pad);
    return res.front();
}
long long to_code(string str)
{
    long long num = stoi(str.substr(0, str.size() - 1));
    auto code = encode(str);
    return num * code.size();
}
long long solve1(const vector<string>& codes)
{
    long long num = 0;
    for (auto str : codes)
        num += to_code(str);
    return num;
}
long long solve2(const input_t& input)
{
	long long res = 0;
	return res;
}

int main()
{
	test();
	ifstream is("Day21.txt");
	input_t input = read_input(is);

	cout << "Day21 Answer1: " << solve1(input) << endl;
	cout << "Day21 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(029A
980A
179A
456A
379A)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
