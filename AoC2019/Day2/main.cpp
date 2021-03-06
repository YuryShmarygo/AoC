#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<long> ReadInput(istream& is);
void Solve(vector<long>& input);
void Test();

int main()
{
	//read input file
	ifstream is("Day2.txt");

	auto source = ReadInput(is);
	auto input = source;


	cout << "Read " << input.size() << " values" << endl;
	//for_each(input.begin(), input.end(), [&](auto i) {cout << i << endl; });
	input[1] = 12;
	input[2] = 2;
	Solve(input);
	cout << "Day2, answer1: " << input[0] << endl;

	for (long noun = 0; noun <= 99; ++noun)
	{
		for (long verb = 0; verb <= 99; ++verb)
		{
			auto input = source;
			input[1] = noun;
			input[2] = verb;
			Solve(input);
			if (input[0] == 19690720)
			{
				cout << "Day2, answer2: " << 100 * noun + verb << endl;
				return 0;
			}
		}
	}

	//Test();
	return 0;
}

vector<long> ReadInput(istream& is)
{
	vector<long> input;
	long val;
	while (is >> val)
	{
		input.push_back(val);
		char separator;
		is >> separator;
	}
	return input;
}

void Test()
{
	auto test_input = string("1,9,10,3,2,3,11,0,99,30,40,50");
	stringstream is(test_input);
	auto input = ReadInput(is);
	Solve(input);
	cout << "Test: " << input[0];
}

void Solve(vector<long>& input)
{
	int cur = 0;
	while (true)
	{
		if (input[cur] == 99)
			break;
		if (input[cur] == 1)
		{
			input[input[cur + 3]] = input[input[cur + 1]] + input[input[cur + 2]];
		}
		else if (input[cur] == 2)
		{
			input[input[cur + 3]] = input[input[cur + 1]] * input[input[cur + 2]];
		}
		else
		{
			cout << "Invalid code: " << input[cur] << endl;
			break;
		}
		cur += 4;
	}
}
