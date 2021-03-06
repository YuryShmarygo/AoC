#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;

struct Planet
{
	Planet() : name(""), orb(0), parent("") {}
	Planet(string _name, int _orb, string _parent) : name(_name), orb(_orb), parent(_parent) {}
	string name;
	int orb;
	string parent;
};

int main()
{
	map<string, Planet> planets;
	ifstream is("Day6.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			break;
		string pname = line.substr(0, 3);
		string cname = line.substr(4, 3);

		auto it = planets.find(pname);
		if (it == planets.end())
		{
			planets[pname] = Planet(pname, 0, "");
			it = planets.find(pname);
		}
		planets[cname] = Planet(cname, 0, pname);
	}
	cout << "Read " << planets.size() << " planets" << endl;

	// correct orbs
	for (auto& p : planets)
	{
		if (p.second.orb != 0)
			continue;

		int orbs = 0;
		auto it = planets.find(p.second.name);
		while ((it = planets.find(it->second.parent)) != planets.end())
			++orbs;

		p.second.orb = orbs;

		it = planets.find(p.second.name);
		while ((it = planets.find(it->second.parent)) != planets.end())
			it->second.orb = --orbs;
	}
	int orbs = 0;
	for (auto p : planets)
	{
		orbs += p.second.orb;
	}
	cout << "Day6 Answer1: " << orbs << endl;

	int oyou = 0;
	int osan = 0;
	auto you = planets[planets["YOU"].parent];
	auto san = planets[planets["SAN"].parent];
	while (you.name != san.name)
	{
		if (you.orb >= san.orb)
		{
			++oyou;
			you = planets[you.parent];
		}
		else
		{
			++osan;
			san = planets[san.parent];
		}
	}
	cout << "Day6 Answer2: " << oyou + osan << endl;


	return 0;
}
