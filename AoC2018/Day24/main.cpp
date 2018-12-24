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
typedef list<Unit> Army;
int EffectivePower(const Unit& u)
{
	return u.n * u.ad;
}
int EffectiveHP(const Unit& u)
{
	return u.n * u.hp;
}
int Damadge(const Unit& a, const Unit& d)
{
	int damadge = EffectivePower(a);
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
	return dam1 < dam2
		|| dam1 == dam2 && EffectivePower(d1) < EffectivePower(d2)
		|| dam1 == dam2 && EffectivePower(d1) == EffectivePower(d2) && d1.initiative < d2.initiative;
}
Unit FromString(string str, UnitType type)
{
	Unit u;
	u.id = Unit::NextId();
	u.type = type;
	smatch g;
	if (regex_match(str, g, regex(R"((\d+) units each with (\d+) hit points (\([^)]+\))* with an attack that does (\d+) (\w+) damage at initiative (\d+))")))
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
	int damage;
};

vector<Schedule> GetSchedule(const Army& a, const Army& b)
{
	vector<Schedule> schedule;
	list<Unit> toS = a;
	list<Unit> toA = b;
	toS.sort([](auto u1, auto u2) {return u1.n * u1.ad < u2.n * u2.ad || u1.n * u1.ad == u2.n * u2.ad && u1.initiative < u2.initiative; });

	for (auto u : toS)
	{
		if (toA.size() == 0)
			break;

		toA.sort(bind(CompDamadge, u, _1, _2));
		auto& best = toA.back();
		if (Damadge(u, best) > 0)
			schedule.push_back({ u.initiative, u.id, best.id, Damadge(u, best) });
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
		sort(schedule.begin(), schedule.end(), [](auto s1, auto s2) { return s1.initiative < s2.initiative; });
		Army units = f[0];
		units.insert(units.end(), f[1].begin(), f[1].end());
		for (auto it = schedule.rbegin(); it != schedule.rend(); ++it)
		{
			auto a = find_if(units.begin(), units.end(), [&](auto u) { return u.id = it->uid; });
			auto d = find_if(units.begin(), units.end(), [&](auto u) { return u.id = it->tid; });
			if (a != units.end() && d != units.end())
			{
				int left = EffectiveHP(*d) - it->damage;
				if (left <= 0)
					units.erase(d);
				else
					d->n = (left + d->hp - 1) / d->hp;
			}
		}
		f[0].clear();
		copy_if(units.begin(), units.end(), back_inserter(f[0]), [](auto u) {return u.type == Immune; });
		f[1].clear();
		copy_if(units.begin(), units.end(), back_inserter(f[0]), [](auto u) {return u.type == Infection; });
	}
	return f;
}

int main()
{
	vector<Army> input = ReadFromFile("d24.txt");

	auto res = Battle(input);
	int units = 0;
	for (auto a : res)
		for (auto u : a)
			units += u.n;
	cout << "Day20 Answer1: " << units << endl;
	cout << "Day20 Answer2: " << "" <<  endl;
	return 0;
}
