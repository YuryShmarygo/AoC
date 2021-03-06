#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct Claim
{
	int Id;
	int Left;
	int Top;
	int Width;
	int Height;
};

int main()
{
	//read input file : is there a nice way?
	vector<Claim> input;
	ifstream is("input.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			break;
		Claim claim;
		if (5 == sscanf_s(line.c_str(),"#%d @ %d,%d: %dx%d", &claim.Id, &claim.Left, &claim.Top, &claim.Width, &claim.Height))
			input.push_back(claim);
	}
	cout << "Read " << input.size() << " claims" << endl;
	
	// First task
	int maxWidth = 0, maxHeight = 0;
	for (auto& c : input) { maxWidth = max(maxWidth, c.Left + c.Width); maxHeight = max(maxHeight, c.Top + c.Height); }
	vector<vector<list<int>>> fabric(maxHeight, vector<list<int>>(maxWidth, list<int>()));
	for (auto& c : input) for (int i = c.Top; i < c.Top + c.Height; i++) for (int j = c.Left; j < c.Left + c.Width; j++) fabric[i][j].push_back(c.Id);
	int overlap = 0;
	for (int i = 0; i < maxHeight; i++) overlap += (int)count_if(fabric[i].begin(), fabric[i].end(), [](auto it) {return it.size() > 1; });
	cout << "Day3 Answer1: " << overlap << endl;

	// Second task
	auto c = find_if(input.begin(), input.end(), [&](auto c) { for (int i = c.Top; i < c.Top + c.Height; i++) for (int j = c.Left; j < c.Left + c.Width; j++) if (fabric[i][j].size() > 1) return false; return true; });
	cout << "Day3 Answer2: " << c->Id << endl;
	return 0;
}