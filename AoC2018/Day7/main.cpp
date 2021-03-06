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
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

struct StepRaw
{
	char s1;
	char s2;
};
struct Step
{
	char s;
	set<char> dep;
};

int main()
{
	//read input file
	vector<StepRaw> input{ {'B', 'G'},{'G', 'J'},{'J', 'F'},{'U', 'Z'},{'C', 'M'},{'Y', 'I'},{'Q', 'A'},{'N', 'L'},{'O', 'A'},{'Z', 'T'},{'I', 'H'},{'L', 'W'},{'F', 'W'},{'T', 'X'},{'A', 'X'},{'K', 'X'},{'S', 'P'},{'M', 'E'},{'E', 'W'},{'D', 'P'},{'P', 'W'},{'X', 'H'},{'V', 'W'},{'R', 'H'},{'H', 'W'},{'N', 'I'},{'X', 'R'},{'D', 'V'},{'V', 'R'},{'F', 'K'},{'P', 'R'},{'P', 'V'},{'S', 'X'},{'I', 'S'},{'J', 'N'},{'T', 'S'},{'T', 'R'},{'K', 'P'},{'N', 'R'},{'G', 'T'},{'I', 'V'},{'G', 'Q'},{'D', 'H'},{'V', 'H'},{'T', 'K'},{'T', 'W'},{'E', 'H'},{'C', 'R'},{'L', 'K'},{'G', 'Y'},{'Y', 'O'},{'O', 'E'},{'U', 'S'},{'X', 'W'},{'C', 'D'},{'E', 'P'},{'B', 'R'},{'F', 'R'},{'A', 'D'},{'G', 'M'},{'B', 'Q'},{'Q', 'V'},{'B', 'W'},{'S', 'H'},{'P', 'X'},{'I', 'M'},{'A', 'S'},{'M', 'X'},{'L', 'S'},{'S', 'W'},{'L', 'V'},{'Z', 'X'},{'M', 'R'},{'T', 'A'},{'N', 'V'},{'M', 'H'},{'E', 'D'},{'F', 'V'},{'B', 'O'},{'G', 'U'},{'J', 'C'},{'G', 'F'},{'Y', 'M'},{'F', 'D'},{'M', 'P'},{'F', 'T'},{'G', 'A'},{'G', 'Z'},{'K', 'V'},{'J', 'Z'},{'O', 'Z'},{'B', 'E'},{'Z', 'V'},{'Q', 'O'},{'J', 'D'},{'Y', 'E'},{'D', 'R'},{'I', 'F'},{'M', 'V'},{'I', 'D'},{'O', 'P'}};
	cout << "Read " << input.size() << " points" << endl;

	set<char> all_steps;
	for (auto i : input) { all_steps.insert(i.s1); all_steps.insert(i.s2); }
	map<char, Step> steps; for (auto i : all_steps) { steps[i] = Step{ i }; }

	for (auto c : all_steps)
	{
		for (auto i : input)
		{
			if (c == i.s2)
				steps[c].dep.insert(i.s1);
		}

	}

	vector<char> order;
	set<char> processed;
	map<char, Step> steps_c(steps);
	while (steps_c.size() > 0)
	{
		set<char> available;
		for (auto i : steps_c)
		{
			if (includes(processed.begin(), processed.end(), i.second.dep.begin(), i.second.dep.end()))
				available.insert(i.first);
		}
		if (available.size() == 0)
			throw "Something wrong";
		char s = *available.begin();
		steps_c.erase(s);
		processed.insert(s);
		order.push_back(s);
	}
	cout << "Day7 Answer1: "; for (auto i : order) cout << i; cout << endl;

	{
		set<char> processed;
		map<char, Step> steps_c(steps);
		struct Work
		{
			int time;
			char step;
		};
		vector<Work> workers;
		int duration = 0;
		while (processed.size() != all_steps.size())
		{
			int curDuration = workers.empty() ? 0 : min_element(workers.begin(), workers.end(), [](auto w1, auto w2) {return w1.time < w2.time; })->time;
			for (auto it = workers.begin(); it != workers.end();)
			{
				if (it->time == curDuration)
				{
					processed.insert(it->step);
					it = workers.erase(it);
				}
				else
				{
					it->time -= curDuration;
					++it;
				}
			}
			duration += curDuration;

			if (!steps_c.empty())
			{
				set<char> available;
				for (auto i : steps_c)
				{
					if (includes(processed.begin(), processed.end(), i.second.dep.begin(), i.second.dep.end()))
						available.insert(i.first);
				}
				//if (available.size() == 0)
				//	throw "Something wrong";

				for (auto i : available)
				{
					if (workers.size() < 6)
					{
						workers.push_back(Work{ i - 'A' + 61, i });
						steps_c.erase(i);
					}
				}
			}
		}
		cout << "Day7 Answer2: " << duration << endl;
	}


	return 0;
}
