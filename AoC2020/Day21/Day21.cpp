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

struct Food
{
	vector<string> ing;
	vector<string> aller;
};
istream& operator>>(istream& is, Food& food)
{
	string line;
	getline(is, line);
	smatch m;
	if (regex_match(line, m, regex("^([^\\(]+)(?: \\(contains (.+)\\))?$")))
	{
		stringstream s(m[1]);
		istream_iterator<string> start(s), end;
		food.ing.assign(start, end);
		if (m[2].matched)
		{
			s = stringstream(m[2]);
			istream_iterator<string> start(s), end;
			food.aller.assign(start, end);
			for (auto& str : food.aller)
				if (str.back() == ',')
					str.pop_back();
		}
	}
	return is;
}
typedef map<string, set<string>> AllMap;
set<string> remove_known(set<string> ing, const AllMap& all)
{
	set<string> res(ing);
	for (auto& a : all)
		if (a.second.size() == 1)
			res.erase(*(a.second.begin()));
	return res;
}
void remove_known(AllMap& all, string aller)
{
	string ing = *(all[aller].begin());
	for (auto& a : all)
	{
		if (a.first != aller && a.second.size() > 1)
		{
			a.second.erase(ing);
			if (a.second.size() == 1)
				remove_known(all, a.first);
		}
	}
}
AllMap find_all(const vector<Food>& input)
{
	AllMap all;
	for (auto& food : input)
	{
		set<string> ing(food.ing.begin(), food.ing.end());
		//set<string> aller(food.aller.begin(), food.aller.end());
		for (auto& a : food.aller)
		{
			auto it = all.find(a);
			if (it != all.end())
			{
				set<string> inter;
				set_intersection(it->second.begin(), it->second.end(), ing.begin(), ing.end(), inserter(inter, inter.end()));
				it->second = inter;
				if (it->second.size() == 1)
				{
					ing.erase(*(it->second.begin()));
					remove_known(all, it->first);
				}
			}
			else
			{
				all[a] = remove_known(ing, all);
			}
		}
	}
	return all;
}
int count_free(const vector<Food>& input)
{
	auto all = find_all(input);
	set<string> all_known;
	map<int, vector<AllMap::value_type>> range;
	for (auto& a : all)
	{
		if (a.second.size() == 1)
			all_known.insert(*(a.second.begin()));
		else
			range[(int)a.second.size()].push_back(a);
	}
	return accumulate(input.begin(), input.end(), 0L, [&](auto sum, auto& food)
		{
			set<string> ing(food.ing.begin(), food.ing.end());
			set<string> diff;
			set_difference(ing.begin(), ing.end(), all_known.begin(), all_known.end(), inserter(diff, diff.end()));
			return sum + (int)diff.size();
		});
}
string find_ing(const vector<Food>& input)
{
	string res;
	auto all = find_all(input);
	for (auto& a : all)
		res = S(res << (res.empty() ? "" : ",") << *(a.second.begin()));
	return res;
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day21.txt");
	istream_iterator<Food> start(is), end;
	vector<Food> input(start, end);
	//cout << "Read " << input.size() << " foods" << endl;

	cout << "Day21 Answer1: " << count_free(input) << endl;
	cout << "Day21 Answer2: " << find_ing(input) << endl;
}

void Test()
{
	stringstream test(R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))");
	istream_iterator<Food> start(test), end;
	vector<Food> input(start, end);
	cout << "Test 1: " << count_free(input) << endl;
	cout << "Test 2: " << find_ing(input) << endl;
}