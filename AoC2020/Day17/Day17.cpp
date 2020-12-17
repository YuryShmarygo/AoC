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

typedef vector<char> Vec;
typedef vector<Vec> Mat;
typedef vector<Mat> Cube;
typedef vector<Cube> HCube;

template<typename T> typename vector<T>::const_iterator p_it(const vector<T>& t, int d)
{ return d == 0 ? t.begin() : t.begin() + d - 1; }
template<typename T> typename vector<T>::const_iterator n_it(const vector<T>& t, int d)
{ return d == t.size() - 1 ? t.end() : t.begin() + d + 2; }

typedef vector<int> Pos;
template<typename T, int N> int count_neibours(const vector<T>& cn, const Pos& pos)
{ return accumulate(p_it(cn, pos[N-1]), n_it(cn, pos[N-1]), 0, [&](int sum, auto& cx) {return sum + count_neibours<T::value_type, N-1>(cx, pos);}); }
template<> int count_neibours<char,1>(const vector<char>& cx, const Pos& pos)
{ return (int)count(p_it(cx, pos[0]), n_it(cx, pos[0]), '#'); }
template<typename T, int N> int count(const vector<T>& cn)
{ return accumulate(cn.begin(), cn.end(), 0, [&](int sum, auto& cx) {return sum + count<T::value_type, N-1>(cx);}); }
template<> int count<char,1>(const vector<char>& cx)
{ return (int)count(cx.begin(), cx.end(), '#'); }


HCube cycle(const HCube& hcube)
{
	HCube res(hcube);
	for (int w = 0; w < hcube.size(); ++w)
		for (int z = 0; z < hcube[w].size(); ++z)
			for (int y = 0; y < hcube[w][z].size(); ++y)
				for (int x = 0; x < hcube[w][z][y].size(); ++x)
				{
					int neibours = count_neibours<Cube, 4>(hcube, { x, y, z, w });
					if (hcube[w][z][y][x] == '#')
						res[w][z][y][x] = (neibours == 3 || neibours == 4) ? '#' : '.';
					else
						res[w][z][y][x] = neibours == 3 ? '#' : '.';
				}
	return res;
}

int hcount6(const vector<string>& input, bool hyper = true)
{
	int dim = 12 + (int)input.size();
	HCube hcube(hyper ? 13 : 1, Cube(13, Mat(dim, Vec(dim, '.'))));
	for (int y = 0; y < input.size(); ++y)
		copy(input[y].begin(), input[y].end(), hcube[hyper ? 6 : 0][6][y + 6].begin() + 6);
	for (int i = 0; i < 6; ++i)
		hcube = cycle(hcube);
	return count<Cube, 4>(hcube);
}


void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day17.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day17 Answer1: " << hcount6(input, false) << endl;
	cout << "Day17 Answer2: " << hcount6(input) << endl;
}

void Test()
{
	stringstream test(R"(.#.
..#
###)");
	istream_iterator<string> start(test), end;
	vector<string> input(start, end);
	cout << "Test 1: " << hcount6(input, false) << endl;
	cout << "Test 2: " << hcount6(input) << endl;
}