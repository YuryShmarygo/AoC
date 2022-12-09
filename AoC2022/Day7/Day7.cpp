#include "../common.h"

struct file
{
	string name;
	string full_name;
	int size;
};
struct dir
{
	dir* parent;
	string name;
	string full_name;
	map<string, file*> files;
	map<string, dir*> dirs;
	int size;
	dir() : parent(nullptr), name("/"), full_name("/"), size(0) {}
};
struct fs
{
	dir root;
	map<string, dir*> dirs;
	map<string, file*> files;
	fs() { dirs[root.name] = &root; }
	void add_dir(dir* parent, string name)
	{
		dir* d = new dir();
		d->name = name;
		d->full_name = parent->full_name + "/" + name;
		d->parent = parent;
		parent->dirs[name] = d;
		dirs[d->full_name] = d;
	}
	void add_file(dir* parent, int size, string name)
	{
		file* f = new file{ name, parent->full_name + "/" + name, size };
		parent->files[name] = f;
		files[f->full_name] = f;
	}
	int calc_size(dir* cur)
	{
		cur->size = accumulate(cur->files.begin(), cur->files.end(), 0, [](auto sum, auto& f) {return sum + f.second->size; });
		cur->size += accumulate(cur->dirs.begin(), cur->dirs.end(), 0, [&](auto sum, auto& d) {return sum + calc_size(d.second); });
		return cur->size;
	}
	~fs()
	{
		dirs.erase(root.full_name);
		for (auto d : dirs)
			delete d.second;
		for (auto f : files)
			delete f.second;
	}
};
istream& operator>>(istream& is, fs& f)
{
	string line;
	dir* cur = &f.root;
	while(getline(is, line) && !line.empty())
	{
		smatch m;
		if (regex_match(line, m, regex("\\$ cd (.+)")))
		{
			if (m[1] == "/")
				cur = &f.root;
			else if (m[1] == "..")
				cur = cur->parent;
			else
				cur = cur->dirs[m[1]];
		}
		else if (regex_match(line, m, regex("\\$ ls")))
			;
		else if (regex_match(line, m, regex("dir (.+)")))
			f.add_dir(cur, m[1]);
		else if (regex_match(line, m, regex("(\\d+) (.+)")))
			f.add_file(cur, stoi(m[1]), m[2]);
	}
	f.calc_size(&f.root);
	return is;
}
ostream& operator<<(ostream& os, const file& f)
{ return os << f.full_name << " size: " << f.size << endl; }
ostream& operator<<(ostream& os, const dir& d)
{
	os << d.full_name << " size: " << d.size << endl;
	for (auto f : d.files)
		os << *f.second;
	for (auto it : d.dirs)
		os << *it.second;
	return os;
}
ostream& operator<<(ostream& os, const fs& f)
{ return os << f.root; }
int sum_1(fs& f)
{
	return accumulate(f.dirs.begin(), f.dirs.end(), 0, [](auto sum, auto& d) {  return sum + (d.second->size <= 100000 ? d.second->size : 0); });
}
int find_dir(fs& f)
{
	//cout << f << endl;
	int free = 70000000 - f.root.size;
	int to_clean = 30000000 - free;
	set<int> sizes;
	for (auto d: f.dirs)
	{
		if (d.second->size >= to_clean)
			sizes.insert(d.second->size);
	}
	return *sizes.begin();
}

int main()
{
	test();
	ifstream is("Day7.txt");
	fs f;
	is >> f;
	
	cout << "Day7 answer1: " << sum_1(f) << endl;
	cout << "Day7 answer2: " << find_dir(f) << endl;
}
void test()
{
	stringstream is(R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)");
	fs f;
	is >> f;
	cout << sum_1(f) << endl;
}
