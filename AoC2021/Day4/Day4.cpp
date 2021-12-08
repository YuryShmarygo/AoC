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

vector<int> ReadDraws(istream& is)
{
	vector<int> draws;
	string line; getline(is, line);	stringstream s(line); string num;
	while (getline(s, num, ',') && !num.empty())
		draws.push_back(stoi(num));
	return draws;
}

typedef vector<vector<int>> Board;
Board ReadBoard(istream& is)
{
	Board board;
	string line;
	while (getline(is, line) && !line.empty())
	{
		stringstream s(line); istream_iterator<int> start(s), end;
		board.push_back(vector<int>(start, end));
	}
	return board;
}
vector<Board> ReadBoards(istream& is)
{
	vector<Board> boards;
	while (is)
		boards.push_back(ReadBoard(is));
	return boards;
}
bool CheckRows(const Board& board, const set<int>& called)
{ return any_of(board.begin(), board.end(), [&](auto& l) { return all_of(l.begin(), l.end(), [&](auto n) { return called.find(n) != called.end(); }); }); }
bool CheckCols(const Board& board, const set<int>& called)
{
	static vector<int> ind{ 0,1,2,3,4 };
	return any_of(ind.begin(), ind.end(), [&](auto i) { return all_of(board.begin(), board.end(), [&, i](auto& l) { return called.find(l[i]) != called.end(); }); });
}
bool IsWon(const Board& board, const set<int>& called)
{ return CheckRows(board, called) || CheckCols(board, called); }
long Sum(const Board& b, const set<int>& called)
{
	long n = 0;
	for (auto it = b.begin(); it != b.end(); ++it)
		n += accumulate(it->begin(), it->end(), 0, [&](int sum, int num) { return sum + (called.find(num) == called.end() ? num : 0); });
	return n;
}
long GetScore(const vector<Board>& boards, const vector<int>& draws)
{
	set<int> called;
	for (auto it = draws.begin(); it != draws.end(); ++it)
	{
		called.insert(*it);
		auto w = find_if(boards.begin(), boards.end(), [&](auto& b) {return IsWon(b, called); });
		if (w != boards.end())
			return Sum(*w, called) * (*it);
	}
	return 0;
}
long GetLastScore(vector<Board> boards, const vector<int>& draws)
{
	set<int> called;
	for (auto it = draws.begin(); it != draws.end(); ++it)
	{
		called.insert(*it);
		vector<Board>::iterator w;
		while (boards.end() != (w = find_if(boards.begin(), boards.end(), [&](auto& b) {return IsWon(b, called); })))
		{
			if (boards.size() > 1)
				boards.erase(w);
			else
				return Sum(*w, called) * (*it);
		}
	}
	return 0;
}
void Test();
int main()
{
	//Test();
	ifstream is("Day4.txt");
	auto draws = ReadDraws(is);
	string str;  getline(is, str);
	auto boards = ReadBoards(is);

	cout << "Day4 Answer1: " << GetScore(boards, draws) << endl;
	cout << "Day4 Answer2: " << GetLastScore(boards, draws) << endl;
}

void Test()
{
	stringstream is(R"(7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7)");

	auto draws = ReadDraws(is);
	string str;  getline(is, str);
	auto boards = ReadBoards(is);

	cout << "Test1: " << GetScore(boards, draws) << endl;
	cout << "Test2: " << GetLastScore(boards, draws) << endl;
}