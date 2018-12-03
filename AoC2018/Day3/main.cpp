#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <regex>

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
	
	//read input file
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
	vector<vector<list<int>>> fabric;
	int maxWidth = 0, maxHeight = 0;
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		if (it->Left + it->Width > maxWidth)
			maxWidth = it->Left + it->Width;
		if (it->Top + it->Height > maxHeight)
			maxHeight = it->Top + it->Height;
	}
	for (int i = 0; i < maxHeight; i++)
	{
		fabric.push_back(vector<list<int>>(maxWidth));
		for (int j = 0; j < maxWidth; j++)
			fabric[i].push_back(list<int>());
	}
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		for (int i = it->Top; i < it->Top + it->Height; i++)
		{
			for (int j = it->Left; j < it->Left + it->Width; j++)
			{
				fabric[i][j].push_back(it->Id);
			}
		}
	}
	int overlap = 0;
	for (int i = 0; i < maxHeight; i++)
	{
		for (int j = 0; j < maxWidth; j++)
		{
			if (fabric[i][j].size() > 1)
				overlap++;
		}
	}

	cout << "Day3 Answer1: " << overlap << endl;

	for (auto it = input.begin(); it != input.end(); ++it)
	{
		bool overlap = false;
		for (int i = 0; i < maxHeight && !overlap; ++i)
		{
			for (int j = 0; j < maxWidth && !overlap; ++j)
			{
				auto aria = fabric[i][j];
				if (aria.size() > 0 && find(aria.begin(), aria.end(), it->Id) != aria.end() && aria.size() > 1)
					overlap = true;
			}
		}
		if (!overlap)
		{
			cout << "Day3 Answer2: " << it->Id << endl;
			break;
		}
	}
	return 0;
}
