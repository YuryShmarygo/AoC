#include "../common.h"

typedef point_t<long long> pointl;
struct machine_t
{
	pointl a;
	pointl b;
	pointl prize;
};
pointl read_point(string line, string format)
{
	pointl p;
	smatch m;
	if (regex_match(line, m, regex(format)))
	{
		p.x = stoi(m[1].str());
		p.y = stoi(m[2].str());
	}
	return p;
}
pointl read_button(string line)
{ return read_point(line, R"(Button [A|B]: X\+(\d+), Y\+(\d+))"); }
pointl read_prize(string line)
{ return read_point(line, R"(Prize: X=(\d+), Y=(\d+))"); }
istream& operator>>(istream& is, machine_t& m)
{
	string line;
	while (getline(is, line) && line.empty());
	if (line.empty())
		return is;
	m.a = read_button(line);
	getline(is, line);
	m.b = read_button(line);
	getline(is, line);
	m.prize = read_prize(line);
	return is;
}
typedef vector<machine_t> input_t;
input_t read_input(istream& is) 
{
	istream_iterator<machine_t> start(is), end;
	return input_t(start, end);
}
long long get_min_cost(const machine_t& m)
{
	long long res = 0;
	//*
	//ofstream log("day13.out", ios_base::app);

	for (long long pa = 0; pa <= 10000; ++pa)
	{
		if (((m.prize.x - pa * m.a.x) % m.b.x) == 0 && ((m.prize.y - pa * m.a.y) % m.b.y) == 0)
		{
			long long pb = (m.prize.x - pa * m.a.x) / m.b.x;
			if (m.prize.y != pa * m.a.y + pb * m.b.y)
				continue;
			long long cost = pa * 3 + pb;
			if (res == 0 || cost < res)
				res = cost;
			//log << "res = " << res << " : " << m.prize.x << " = " << pa << " * " << m.a.x << " + " << pb << " * " << m.b.x << ", " << m.prize.y << " = " << pa << " * " << m.a.y << " + " << pb << " * " << m.b.y << endl;
		}
	}
	for (long long pa = m.prize.x / m.a.x - 10000; pa <= m.prize.x / m.a.x; ++pa)
	{
		if (((m.prize.x - pa * m.a.x) % m.b.x) == 0 && ((m.prize.y - pa * m.a.y) % m.b.y) == 0)
		{
			long long pb = (m.prize.x - pa * m.a.x) / m.b.x;
			if (m.prize.y != pa * m.a.y + pb * m.b.y)
				continue;
			long long cost = pa * 3 + pb;
			if (res == 0 || cost < res)
				res = cost;
			//log << "res = " << res << " : " << m.prize.x << " = " << pa << " * " << m.a.x << " + " << pb << " * " << m.b.x << ", " << m.prize.y << " = " << pa << " * " << m.a.y << " + " << pb << " * " << m.b.y << endl;
		}
	}
	for (long long pb = m.prize.x / m.b.x - 10000; pb <= m.prize.x / m.b.x; ++pb)
	{
		if (((m.prize.x - pb * m.b.x) % m.a.x) == 0 && ((m.prize.y - pb * m.b.y) % m.a.y) == 0)
		{
			long long pa = (m.prize.x - pb * m.b.x) / m.a.x;
			if (m.prize.y != pa * m.a.y + pb * m.b.y)
				continue;
			long long cost = pa * 3 + pb;
			if (res == 0 || cost < res)
				res = cost;
			//log << "res = " << res << " : " << m.prize.x << " = " << pa << " * " << m.a.x << " + " << pb << " * " << m.b.x << ", " << m.prize.y << " = " << pa << " * " << m.a.y << " + " << pb << " * " << m.b.y << endl;
		}
	}

	/*/
	ofstream log("day13.out", ios_base::app);
	for (long long pa = 0; pa < 100; ++pa)
	{
		for (long long pb = 0; pb < 100; ++pb)
		{
			if (m.prize.x == pa * m.a.x + pb * m.b.x && m.prize.y == pa * m.a.y + pb * m.b.y)
			{
				long long cost = pa * 3 + pb;
				if (res == 0 || cost < res)
					res = cost;
				log << "res = " << res << " : " << m.prize.x << " = " << pa << " * " << m.a.x << " + " << pb << " * " << m.b.x << ", " << m.prize.y << " = " << pa << " * " << m.a.y << " + " << pb << " * " << m.b.y << endl;
			}
		}
	}
	log.close();
	//*/
	
	return res;
}
long long solve1(const input_t& input)
{
	long long num = 0;
	for (auto m : input)
		num += get_min_cost(m);
	return num;
}

long long solve2(const input_t& input)
{
	long long num = 0;
	for (auto m : input)
	{
		m.prize.x += 10000000000000;
		m.prize.y += 10000000000000;
		num += get_min_cost(m);
	}
	return num;
}

int main()
{
	test();
	ifstream is("Day13.txt");
	input_t input = read_input(is);

	cout << "Day13 Answer1: " << solve1(input) << endl;
	cout << "Day13 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
