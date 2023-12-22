#include "../common.h"

struct module_base
{
	char type;
	string name;
	vector<string> output;
};
ostream& operator<<(ostream& os, const module_base& m) { os << m.type << m.name << " -> "; r::copy(m.output, ostream_iterator<string>(os, ",")); return os; }
module_base read_module(string line)
{
	module_base m;
	smatch sm;
	if (regex_match(line, sm, regex(R"(([%|&]?)(\w+) -> (.+))")))
	{
		m.type = sm[1].length() > 0 ? sm[1].str()[0] : ' ';
		m.name = sm[2].str();
		string str_out = sm[3].str();
		smatch so;
		while (regex_search(str_out, so, regex(R"((\w+),? ?)")))
		{
			m.output.push_back(so[1].str());
			str_out = so.suffix();
		}
	}
	return m;
}
typedef vector<module_base> module_v;
istream& operator>>(istream& is, module_v& m)
{
	m.clear();
	string line;
	while (getline(is, line) && !line.empty())
		m.push_back(read_module(line));
	return is;
}
struct imodule;
enum e_pulse { p_high, p_low };
ostream& operator<<(ostream& os, const e_pulse& p) { return os << (p == p_high ? "p_high" : "p_low"); }
struct signal_t
{
	string source;
	string dest;
	e_pulse pulse;
};
struct istate
{
	virtual bool is_equal(const istate* o) const = 0;
	virtual string to_string() const = 0;
};
struct imodule
{
	virtual void init(const module_v& mo) = 0;
	virtual list<signal_t> process(const signal_t& s) = 0;
	virtual istate* get_state() = 0;
};
struct flip_module : module_base, imodule
{
	struct flip_state : istate
	{
		bool is_on;
		bool is_equal(const istate* o) const override { auto of = dynamic_cast<const flip_state*>(o);  return of != nullptr && is_on == of->is_on; }
		string to_string() const override { return S((is_on ? "on" : "off")); }
	};
	flip_module(const module_base& base) : module_base(base) {}
	//bool is_on;
	flip_state state;
	list<signal_t> off_signal;
	list<signal_t> on_signal;
	void init(const module_v& mo) override
	{
		state.is_on = false;
		for (auto& o : output)
		{
			off_signal.push_back({ name, o, p_high });
			on_signal.push_back({ name, o, p_low });
		}
	}
	list<signal_t> process(const signal_t& s) override
	{
		list<signal_t> res;
		if (s.pulse == p_high)
			return res;
		state.is_on = !state.is_on;
		return state.is_on ? off_signal : on_signal;
	}
	istate* get_state() override { return &state; }
};
ostream& operator<<(ostream& os, const pair<string, e_pulse>& p) { return os << "[" << p.first << ", " << p.second << "]"; }
struct conj_module : module_base, imodule
{
	struct conj_state : istate
	{
		map<string, e_pulse> received;
		bool is_equal(const istate* o) const override 
		{ 
			auto oc = dynamic_cast<const conj_state*>(o);
			return received == oc->received;
		}
		string to_string() const override{ stringstream is; r::copy(received, ostream_iterator<pair<string, e_pulse>> (is, ", ")); return is.str(); }
	};
	conj_module(const module_base& base) : module_base(base) {}
	//map<string, e_pulse> received;
	conj_state state;
	list<signal_t> high_signal;
	list<signal_t> low_signal;
	void init(const module_v& mo) override
	{
		for (auto& m : mo)
		{
			auto it = r::find_if(m.output, [this](auto& str) { return str == this->name; });
			if (it != m.output.end())
				state.received[m.name] = p_low;
		}
		for (auto& o : output)
		{
			high_signal.push_back({ name, o, p_high });
			low_signal.push_back({ name, o, p_low });
		}
	}
	list<signal_t> process(const signal_t& s) override
	{
		state.received[s.source] = s.pulse;
		if (r::all_of(state.received, [](auto& p) { return p.second == p_high; }))
			return low_signal;
		return high_signal;
	}
	virtual istate* get_state() override { return &state; }
};
struct bc_module : module_base, imodule
{
	struct bc_state : istate { bool is_equal(const istate* o) const override { return true; }  string to_string() const override { return "<empty>"; } };
	bc_state state;
	bc_module(const module_base& base) : module_base(base) {}
	list<signal_t> high_signal;
	list<signal_t> low_signal;
	void init(const module_v& mo) override
	{
		for (auto& o : output)
		{
			high_signal.push_back({ name, o, p_high });
			low_signal.push_back({ name, o, p_low });
		}
	}
	list<signal_t> process(const signal_t& s) override
	{
		return s.pulse == p_high ? high_signal : low_signal;
	}
	istate* get_state() override { return &state; }
};
imodule* create_module(const module_base& m, const module_v& mo)
{
	imodule* mod;
	switch (m.type)
	{
	case ' ': mod = new bc_module(m); break;
	case '%': mod = new flip_module(m); break;
	case '&': mod = new conj_module(m); break;
	default: cout << "warning: cannot create module of type [" << m.type << "]" << endl; mod = new bc_module(m);
	}
	mod->init(mo);
	return mod;
}
ostream& operator<<(ostream& os, const signal_t& s) { return os << "[" << s.source << " -> " << s.dest << " : " << s.pulse << "]"; }
ostream& operator<<(ostream& os, const list<signal_t>& s) { r::copy(s, ostream_iterator<signal_t>(os, ", ")); return os; }
ostream& operator<<(ostream& os, const vector<istate*>& states) { os << "["; for (auto& s : states) { os << "[" << s->to_string() << "]"; } return os; }
bool are_equal_states(const vector<istate*>& s1, const vector<istate*>& s2)
{
	auto its1 = s1.begin();
	auto its2 = s2.begin();
	for (; its1 != s1.end() && its2 != s2.end(); ++its1, ++its2)
		if (!(*its1)->is_equal(*its2))
			return false;
	return true;
}
struct machine_t
{
	map<string, imodule*> modules;
	static machine_t build(const module_v& mo)
	{
		machine_t machine;
		for (auto& m : mo)
			machine.modules[m.name] = create_module(m, mo);
		return machine;
	}
	vector<istate*> get_state()
	{
		vector<istate*> ns;
		for (auto& mp : modules)
			ns.push_back(mp.second->get_state());
		return ns;
	}
	tuple<int, int> process(list<signal_t> signals)
	{
		int hn = 0, ln = 0;
		while (!signals.empty())
		{
			list<signal_t> next_s;
//			cout << signals << endl;
			for (auto& s : signals)
			{
				auto itm = modules.find(s.dest);
				if (itm != modules.end())
				{
					auto cur_s = itm->second->process(s);
					next_s.insert(next_s.end(), cur_s.begin(), cur_s.end());
				}
				if (s.pulse == p_high)
					++hn;
				else
					++ln;
			}
			swap(signals, next_s);
		}
		return { ln, hn};
	}
	long long process(int num)
	{
		list<tuple<int, int, vector<istate*>>> states;
		long long ln_t = 0, hn_t = 0;
		for (int i = 0; i < num; ++i)
		{
			auto [ln, hn] = process({ {"button","broadcaster", p_low} });
			ln_t += ln;
			hn_t += hn;
		}
		return ln_t * hn_t;
	}
};
long long solve1(const module_v& mo)
{
	auto machine = machine_t::build(mo);
	return machine.process(1000);
}
long long solve2(const module_v& mo)
{
	auto machine = machine_t::build(mo);
	map<string, int> monitor;
	auto rx = dynamic_cast<conj_module*>(machine.modules["dh"]);
	auto rx_state = dynamic_cast<conj_module::conj_state*>(rx->get_state());
	for (auto& m : rx_state->received)
		monitor[m.first] = 0;
	int n = 1;
	while (!r::all_of(monitor, [](auto& m) { return m.second > 0; }))
	{
		list<signal_t> signals = { {"button","broadcaster", p_low} };
		while (!signals.empty())
		{
			list<signal_t> next_s;
			for (auto& s : signals)
			{
				auto itmon = monitor.find(s.source);
				if (itmon != monitor.end() && itmon->second == 0 && s.pulse == p_high)
					itmon->second = n;

				auto itm = machine.modules.find(s.dest);
				if (itm != machine.modules.end())
				{
					auto cur_s = itm->second->process(s);
					next_s.insert(next_s.end(), cur_s.begin(), cur_s.end());
				}
			}
			swap(signals, next_s);
		}
		++n;
	}
	long long res = 1;
	for (auto& m : monitor)
		res = lcm(res, m.second);
	return res;
}
int main()
{
	test();
	ifstream is("Day20.txt");
	module_v input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day20 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day20 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a)";
	auto test2 = R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output)";

	auto is = stringstream(test1);
	module_v input; is >> input;
	cout << "Test1: " << solve1(input) << endl;

	is = stringstream(test2); is >> input;
	cout << "Test2: " << solve1(input) << endl;
}
