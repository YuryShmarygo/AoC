#include "../common.h"
int find_unique(string input, int n = 4)
{
	for (size_t i = 0; i < input.size(); ++i)
	{
		bool unique = true;
		for (size_t j = i; j < input.size() && j < i + n && unique; ++j)
			for (size_t k = j + 1; k < input.size() && k < i + n && unique; ++k)
				if (input[j] == input[k])
					unique = false;
		if (unique)
			return (int)i + n;
	}

	return -1;
}


int main()
{
	test();
	ifstream is("Day6.txt");
	string input;
	is >> input;

	cout << "Day6 answer1: " << find_unique(input) << endl;
	cout << "Day6 answer2: " << find_unique(input, 14) << endl;
}
void test()
{
	cout << "test1: " << find_unique("mjqjpqmgbljsphdztnvjfqwrcgsmlb") << endl;
	cout << "test2: " << find_unique("bvwbjplbgvbhsrlpgdmjqwftvncz") << endl;
	cout << "test3: " << find_unique("nppdvjthqldpwncqszvftbrmjlhg") << endl;
	cout << "test4: " << find_unique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") << endl;
	cout << "test5: " << find_unique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") << endl;

	cout << "test6: " << find_unique("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14) << endl;
	cout << "test7: " << find_unique("bvwbjplbgvbhsrlpgdmjqwftvncz", 14) << endl;
	cout << "test8: " << find_unique("nppdvjthqldpwncqszvftbrmjlhg", 14) << endl;
	cout << "test9: " << find_unique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 14) << endl;
	cout << "test10: " << find_unique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 14) << endl;
}
