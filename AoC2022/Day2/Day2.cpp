#include "../common.h"

map<string, int> scores{
{"A X", 4},
{"A Y", 8},
{"A Z", 3},
{"B X", 1},
{"B Y", 5},
{"B Z", 9},
{"C X", 7},
{"C Y", 2},
{"C Z", 6},
};
map<string, int> scores_w{
{"A X", 3},
{"A Y", 4},
{"A Z", 8},
{"B X", 1},
{"B Y", 5},
{"B Z", 9},
{"C X", 2},
{"C Y", 6},
{"C Z", 7},
};

vector<string> read_input(istream& is)
{
	vector<string> res;
	string line;
	while (getline(is, line) && !line.empty())
		res.push_back(line);
	return res;
}
int main()
{
	ifstream is("Day2.txt");
	vector<string> input = read_input(is);
	//cout << "Read " << input.size() << " commands" << endl;

	cout << "Day2 answer1: " << accumulate(input.begin(), input.end(), 0, [](auto sum, auto& s) {return sum + scores[s]; }) << endl;
	cout << "Day2 answer2: " << accumulate(input.begin(), input.end(), 0, [](auto sum, auto& s) {return sum + scores_w[s]; }) << endl;
}
