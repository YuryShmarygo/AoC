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

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

vector<char> FFT(const vector<char>& input)
{
    vector<char> output(input.size(),0);
    for (size_t i = 0; i < input.size(); ++i)
    {
        long sum = 0;
        size_t shift = i + i + 2;
        for (size_t g = i; g <=input.size(); g += shift + shift)
        {
            size_t g_end = min(g + i + 1, input.size());
            for (size_t j = g; j < g_end; ++j)
            {
                sum += input[j];
                if (j + shift < input.size())
                    sum -= input[j + shift];
            }
        }
        output[i]=(abs(sum) % 10);
    }
    return output;
}

vector<char> FFT_Fast(const vector<char>& input, long offset = 0)
{
    // Only sums in the second half
    if (offset < input.size() / 2)
        return FFT(input);

    vector<char> output(input.size(), 0);
    long sum = 0;
    for (size_t i = input.size() - 1; i >= offset; --i)
    {
        sum += input[i];
        output[i] = sum % 10;
    }
    return output;
}

istream& operator>>(istream& is, vector<char>& input)
{
    string str;
    is >> str;
    input.reserve(str.size());
    transform(str.begin(), str.end(), back_inserter(input), [](auto c) {return c - '0'; });
    return is;
}

string task1(const vector<char>& input)
{
    vector<char> cur = input;
    for (int i = 0; i < 100; ++i)
        cur = FFT(cur);

    stringstream os;
    copy_n(cur.begin(), 8, ostream_iterator<int>(os, ""));
    return os.str();
}

string task2(const vector<char>& input)
{
    long offset = 0;
    for (int i = 0; i < 7; ++i)
    {
        offset += input[i];
        if (i < 6)
            offset *= 10;
    }

    vector<char> cur;
    cur.reserve(input.size() * 10000);
    for (int i = 0; i < 10000; ++i)
        copy(input.begin(), input.end(), back_inserter(cur));
    for (int i = 0; i < 100; ++i)
        cur = FFT_Fast(cur, offset);

    stringstream os;
    copy_n(cur.begin() + offset, 8, ostream_iterator<int>(os, ""));
    return os.str();
}

void run_tests();
int main()
{
    //run_tests();
    ifstream is("Day16.txt");
    vector<char> input;
    is >> input;

    cout << "Day16, answer1: " << task1(input) << endl;
    cout << "Day16, answer2: " << task2(input) << endl;

    return 0;
}

void test0()
{
    stringstream is("12345678");
    vector<char> input;
    is >> input;

    vector<char> cur = input;
    for (int i = 0; i < 3; ++i)
    {
        cur = FFT(cur);
        cout << i << ": "; copy_n(cur.begin(), 8, ostream_iterator<int>(cout, "")); cout << endl;
    }
}
void test1(string str)
{
    stringstream is(str);
    vector<char> input;
    is >> input;

    cout << "test1: " << str << ": " << task1(input) << endl;
}
void test2(string str)
{
    stringstream is(str);
    vector<char> input;
    is >> input;

    cout << "test2: " << str << ": " << task2(input) << endl;
}

void run_tests()
{
    test0();
    test1("80871224585914546619083218645595");
    test1("19617804207202209144916044189917");
    test1("69317163492948606335995924319873");
    test2("03036732577212944063491565474664");
    test2("02935109699940807407585447034323");
    test2("03081770884921959731165446850517");
}

/*
Too slow

vector<char> Process(const vector<char>& input, const vector<vector<char>>& patterns)
{
    vector<char> output; output.reserve(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        auto itp = patterns[i].begin() + 1;
        long long sum = 0;
        for (auto c : input)
        {
            if (itp == patterns[i].end())
                itp = patterns[i].begin();
            sum += (long long)c * (*itp);
            ++itp;
        }
        output.push_back(abs(sum) % 10);
    }
    return output;
}

typedef vector<vector<char>> Patterns;
Patterns CreatePatterns(int n)
{
    static char _base[] = { 0,1,0,-1 };
    Patterns patterns(n, vector<char>());
    for (int i = 0; i < n; ++i)
    {
        auto& p = patterns[i];
        int r = i + 1;
        p.reserve(r * 4);
        for (auto b : _base)
            for (int j = 0; j < r; ++j)
                p.push_back(b);
    }
    return patterns;
}
*/