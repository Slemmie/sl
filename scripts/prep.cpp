#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>

const std::string sl_path = "/usr/local/include/";

void help() {
	std::cerr << "  syntax:\n";
	std::cerr << "  prep ([flags] [cpp files] in any order (without '()' and '[]')\n";
	std::cerr << "  first cpp file gets interpreted, all others (or out.cpp if none) gets result written to them\n";
	std::cerr << "  list of commands:\n";
	std::cerr << "  --help    (-h)    =    get help\n";
	std::cerr << "  something.cpp     =    file gets added to the list of cpp files, in order\n";
	std::cerr << "  --include_std     =    keep all standard libraries included in sl files (default: skip them)\n";
	std::cerr << "  --no_sl_include   =    do not recursivly include any sl files\n";
	std::cerr << "  --no_prune        =    do not prune many consecutive newlines (default: prune them)\n";
	std::cerr.flush();
}

bool has_flag(int argc, char** argv, const std::string& target) {
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == target) {
			return true;
		}
	}
	return false;
}

bool has_suffix(const std::string& s, const std::string& suf) {
	if (s.length() < suf.length()) {
		return false;
	}
	return s.substr(s.length() - suf.length()) == suf;
}

bool has_prefix(const std::string& s, const std::string& pre) {
	if (s.length() < pre.length()) {
		return false;
	}
	return s.substr(0, pre.size()) == pre;
}

std::vector <std::string> cpp_files(int argc, char** argv) {
	std::vector <std::string> ret;
	for (int i = 0; i < argc; i++) {
		std::string s = std::string(argv[i]);
		if (has_suffix(s, ".cpp")) {
			ret.push_back(s);
		}
	}
	return ret;
}

std::string get_include_file(const std::string& s) {
	int open = s.find_first_of('<');
	int close = s.find_first_of('>');
	if (open == -1 || close == -1 || open > close) {
		std::cerr << "fatal: bad syntax (line: '" << s << "')" << std::endl;
		exit(0);
	}
	return s.substr(open + 1, close - open - 1);
}

struct Params {
	bool include_std, include_sl, prune;
	std::set <std::string> included;
	std::set <std::string> std_libs;
	std::set <std::string> ignore_lines;
	Params(bool _include_std, bool _include_sl, bool _prune) :
	include_std(_include_std),
	include_sl(_include_sl),
	prune(_prune)
	{
		m_load_std_libs(std_libs);
		m_load_ignore_lines(ignore_lines);
	}
private:
	void m_load_std_libs(std::set <std::string>& container,
	const std::string& file = sl_path + "sl/scripts/std_libs") {
		std::ifstream inf(file);
		if (!inf.is_open()) {
			std::cerr << "fatal: failed to open file '" << file << "'" << std::endl;
			exit(0);
		}
		std::string s;
		while (std::getline(inf, s)) {
			container.insert(s);
		}
		inf.close();
	}
	void m_load_ignore_lines(std::set <std::string>& container,
	const std::string& file = sl_path + "sl/scripts/ignore_lines") {
		std::ifstream inf(file);
		if (!inf.is_open()) {
			std::cerr << "fatal: failed to open file '" << file << "'" << std::endl;
			exit(0);
		}
		std::string s;
		while (std::getline(inf, s)) {
			container.insert(s);
		}
		inf.close();
	}
};

std::string interpret(int argc, char** argv, const std::string& file, Params& params) {
	bool is_sl = has_prefix(file, sl_path);
	std::string ret;
	std::ifstream inf(file);
	if (!inf.is_open()) {
		std::cerr << "fatal: failed to open file '" << file << "'" << std::endl;
		exit(0);
	}
	std::string s;
	while (std::getline(inf, s)) {
		if (params.ignore_lines.count(s)) {
			continue;
		}
		if (has_prefix(s, "#include")) {
			std::string incl = get_include_file(s);
			if ((params.included.count(incl)) ||
			(!params.include_std && is_sl && params.std_libs.count(incl)) ||
			(!params.include_sl && has_prefix(incl, "sl/"))) {
				continue;
			}
			params.included.insert(incl);
			if (has_prefix(incl, "sl/")) {
				ret += interpret(argc, argv, sl_path + incl, params) + "\n";
				continue;
			}
		}
		ret += s + "\n";
	}
	inf.close();
	return ret;
}

void prune_string(std::string& s) {
	std::string to_swap;
	int i = 0;
	while (i < (int)s.length() && s[i] == '\n') {
		i++;
	}
	for (; i < (int)s.length(); i++) {
		if (s[i] == '\n' && s[i] == s[i + 1] && s[i] == s[i + 2]) {
			continue;
		}
		to_swap += s[i];
	}
	std::swap(s, to_swap);
}

int main(int argc, char** argv) {
	if (has_flag(argc, argv, "--help") || has_flag(argc, argv, "-h")) {
		help();
		return 0;
	}
	auto cpp = cpp_files(argc, argv);
	if (cpp.empty()) {
		std::cerr << "fatal: missing input file" << std::endl;
		exit(0);
	}
	if ((int)cpp.size() == 1) {
		cpp.push_back("out.cpp");
	}
	Params params(has_flag(argc, argv, "--include_std"),
	!has_flag(argc, argv, "--no_include_sl"),
	!has_flag(argc, argv, "--no_prune"));
	std::cerr << "interpreting..." << std::endl;
	std::string result = interpret(argc, argv, cpp[0], params);
	if (params.prune) {
		prune_string(result);
	}
	std::cerr << "writing to files..." << std::endl;
	for (int i = 1; i < (int)cpp.size(); i++) {
		std::ofstream ouf(cpp[i]);
		if (!ouf.is_open()) {
			std::cerr << "fatal: failed to open file '" << cpp[i] << "'" << std::endl;
			return 0;
		}
		ouf << result;
		ouf.close();
	}
	std::cerr << "done" << std::endl;
}
