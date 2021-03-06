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
#include <unordered_map>

using namespace std;
using namespace placeholders;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

enum UnitType { Immune, Infection };
enum AttackType {};
struct Unit
{
	int id;
	UnitType type;
	int n;
	int hp;
	int ad;
	string atype;
	int initiative;
	string weak;
	string immune;
	static int NextId()
	{
		static int _next = 0;
		return _next++;
	}
};
ostream& operator<<(ostream& os, const Unit& u)
{
	cout << "id: " << u.id << ", type: " << u.type << ", n: " << u.n << ", hp: " << u.hp << ", ad: " << u.ad << ", atype: " << u.atype
		<< ", initiative: " << u.initiative << ", weak: " << u.weak << ", immune: " << u.immune;
	return os;
}
typedef list<Unit> Army;
int EP(const Unit& u)
{
	return u.n * u.ad;
}
int HP(const Unit& u)
{
	return u.n * u.hp;
}
int Damadge(const Unit& a, const Unit& d)
{
	int damadge = a.n * a.ad;
	if (d.weak.find(a.atype) != string::npos)
		damadge += damadge;
	if (d.immune.find(a.atype) != string::npos)
		damadge = 0;
	return damadge;
}
bool CompDamadge(const Unit& a, const Unit& d1, const Unit& d2)
{
	int dam1 = Damadge(a, d1);
	int dam2 = Damadge(a, d2);
	return dam1 > dam2
		|| dam1 == dam2 && EP(d1) > EP(d2)
		|| dam1 == dam2 && EP(d1) == EP(d2) && d1.initiative > d2.initiative;
}
Unit FromString(string str, UnitType type)
{
	Unit u;
	u.id = Unit::NextId();
	u.type = type;
	smatch g;
	if (regex_match(str, g, regex(R"((\d+) units each with (\d+) hit points (\([^)]+\) )*with an attack that does (\d+) (\w+) damage at initiative (\d+))")))
	{
		string special = g.size() == 7 ? g[3] : string("");
		u.n = stoi(g[1]);
		u.hp = stoi(g[2]);
		int shift = g.size() == 7 ? 1 : 0;
		u.ad = stoi(g[3 + shift]);
		u.atype = g[4 + shift];
		u.initiative = stoi(g[5 + shift]);
		if (special.size() > 0)
		{
			smatch w;
			if (regex_search(special, w, regex(R"(weak to ((?:\w+,?\s?)+))")))
				u.weak = w[1];
			smatch im;
			if (regex_search(special, im, regex(R"(immune to ((?:\w+,?\s?)+))")))
				u.immune = im[1];
		}
	}
	return u;
	//"4400 units each with 10384 hit points (weak to slashing) with an attack that does 21 radiation damage at initiative 16"
}

vector<Army> ReadFromFile(string fileName)
{
	ifstream is(fileName);
	Army immunes;
	Army infections;
	string line;
	bool isImmunes = false;
	while (is && getline(is, line))
	{
		if (line.find("Immune System:") != string::npos)
		{
			isImmunes = true;
			continue;
		}
		else if (line.find("Infection:") != string::npos)
		{
			isImmunes = false;
			continue;
		}
		else if (line.size() == 0)
		{
			continue;
		}

		Unit u = FromString(line, isImmunes ? Immune : Infection);
		if (isImmunes)
			immunes.push_back(u);
		else
			infections.push_back(u);
	}
	return vector<Army>{immunes, infections };
}

struct Schedule
{
	int initiative;
	int uid;
	int tid;
};

vector<Schedule> GetSchedule(const Army& a, const Army& b)
{
	vector<Schedule> schedule;
	list<Unit> toS = a;
	list<Unit> toA = b;
	toS.sort([](auto u1, auto u2) {return EP(u1) > EP(u2) || EP(u1) == EP(u2) && u1.initiative > u2.initiative; });

	for (auto u : toS)
	{
		//cout << "  schedule for " << EP(u) << " " << u.initiative << " " << u << endl;
		if (toA.size() == 0)
			break;

		toA.sort(bind(CompDamadge, u, _1, _2));
		//for (auto a : toA) cout << "   attack: damadge: " << Damadge(u, a) << ", EP: " << EP(a) << " " << a << endl;
		auto& best = toA.front();
		if (Damadge(u, best) > 0)
		{
			schedule.push_back({ u.initiative, u.id, best.id });
			toA.erase(toA.begin());
		}
	}

	return schedule;
}

vector<Army> Battle(const vector<Army>& input)
{
	vector<Army> f = input;
	while (!f[0].empty() && !f[1].empty())
	{
		vector<Schedule> schedule = GetSchedule(f[0], f[1]);
		vector<Schedule> s2 = GetSchedule(f[1], f[0]);
		schedule.insert(schedule.end(), s2.begin(), s2.end());
		sort(schedule.begin(), schedule.end(), [](auto s1, auto s2) { return s1.initiative > s2.initiative; });
		Army units = f[0];
		units.insert(units.end(), f[1].begin(), f[1].end());
		bool noKilled = true;
		for (auto s : schedule)
		{
			auto a = find_if(units.begin(), units.end(), [&](auto u) { return u.id == s.uid; });
			auto d = find_if(units.begin(), units.end(), [&](auto u) { return u.id == s.tid; });
			if (a != units.end() && d != units.end())
			{
				//cout << "unit type: " << a->type << ", id: " << a->id << " attacks unit type: " << d->type << ", id: " << d->id << " making " << Damadge(*a, *d) << " damage, killing ";
				//int prevN = d->n;
				int damadge = Damadge(*a, *d);
				if (d->hp < damadge)
					noKilled = false;
				int left = HP(*d) - damadge;
				if (left <= 0)
					units.erase(d);
				else
					d->n = (left + d->hp - 1) / d->hp;
				//cout << (left > 0 ? S("" << prevN - d->n) : S("all")) << " units" << endl;
			}
		}
		if (noKilled)
			units.clear();
		f[0].clear();
		copy_if(units.begin(), units.end(), back_inserter(f[0]), [](auto u) {return u.type == Immune; });
		f[1].clear();
		copy_if(units.begin(), units.end(), back_inserter(f[1]), [](auto u) {return u.type == Infection; });
	}
	return f;
}
int UpperBound(const vector<Army>& input, const Army& left)
{
	int leftHP = 0;
	for (auto a : left)
		leftHP += HP(a);
	int iUnits = 0;
	for (auto a : input[0])
		iUnits += a.n;
	int boost = leftHP / iUnits + 1;
	if (boost < 2) boost = 2;

	do
	{
		vector<Army> boosted = input;
		for (auto& a : boosted[0])
			a.ad += boost;
		if (!Battle(boosted)[0].empty())
			break;
		boost += boost / 2;
	} while (true);
	return boost;
}
vector<Army> FindBoost(const vector<Army>& input, const Army& left)
{
	int minB = 0, maxB = UpperBound(input, left);
	vector<Army> res;
	while (minB != maxB - 1)
	{
		int boost = (maxB + minB + 1) / 2;
		vector<Army> boosted = input;
		for (auto& a : boosted[0])
			a.ad += boost;
		if (!(res = Battle(boosted))[0].empty())
			maxB = boost;
		else
			minB = boost;
	}
	vector<Army> boosted = input;
	for (auto& a : boosted[0])
		a.ad += maxB;
	return Battle(boosted);
}
int CountUnits(const vector<Army>& input)
{
	int units = 0;
	for (auto a : input)
		for (auto u : a)
			units += u.n;
	return units;
}
void Test();
int main()
{
	//Test(); return 0;
	vector<Army> input = ReadFromFile("d24.txt");

	auto res = Battle(input);
	cout << "Day24 Answer1: " << CountUnits(res) << endl;
	res = FindBoost(input, res[1]);
	cout << "Day24 Answer2: " << CountUnits(res) <<  endl;
	return 0;
}

void Test()
{
	vector<string> immuneInput = {
		{"17 units each with 5390 hit points (weak to radiation, bludgeoning) with an attack that does 4507 fire damage at initiative 2"},
		{"989 units each with 1274 hit points (immune to fire; weak to bludgeoning, slashing) with an attack that does 25 slashing damage at initiative 3"}
	};
	vector<string> infectionInput = {
		{"801 units each with 4706 hit points (weak to radiation) with an attack that does 116 bludgeoning damage at initiative 1"},
		{"4485 units each with 2961 hit points (immune to radiation; weak to fire, cold) with an attack that does 12 slashing damage at initiative 4"}
	};
	Army immunes;
	Army infections;
	for (auto s : immuneInput)
		immunes.push_back(FromString(s, Immune));
	for (auto s : infectionInput)
		infections.push_back(FromString(s, Infection));
	vector<Army> input{immunes, infections };

	auto res = Battle(input);
	cout << "Test1: " << CountUnits(res) << endl;
}