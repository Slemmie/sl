#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>

const std::string path_to_sl = "/usr/local/include/";

void help() {
	std::cerr << "  list of commands:\n";
	std::cerr << "  --help    (-h)    =    get help\n";
	std::cerr << "  in=[some file]    =    specify input file\n";
	std::cerr << "  out=[some file]   =    specify output file\n";
	std::cerr << "  --include_std     =    include all standard libraries present in included sl files (default is to skip them)\n";
	std::cerr << "  --no_sl_include   =    do not recursivly include any sl files\n";
	std::cerr << "  --no_prune        =    do not prune several empty lines in a row (default is yes)\n";
}

std::string get_file(int argc, char** argv, const std::string& which) {
	std::string result = "[not found]";
	for (int i = 0; i < argc; i++) {
		std::string s = std::string(argv[i]);
		if (s.length() > which.length() && s.substr(0, which.length()) == which) {
			result = s.substr(which.length());
		}
	}
	return result;
}

bool has_flag(int argc, char** argv, const std::string& flag) {
	for (int i = 0; i < argc; i++) {
		std::string s = std::string(argv[i]);
		if (s == flag) {
			return true;
		}
	}
	return false;
}

std::vector <std::string> lines_to_skip = {
	"//-//",
	"#pragma once"
};

std::set <std::string> std_libs;

void load_std_libs() {
	std::ifstream inf(path_to_sl + "sl/scripts/std_libs");
	if (!inf.is_open()) {
		std::cerr << "fatal: failed to open file '" << path_to_sl + "sl/scripts/std_libs" << "'" << std::endl;
		exit(0);
	}
	std::string s;
	while (std::getline(inf, s)) {
		std_libs.insert(s);
	}
	inf.close();
}

bool is_pref(const std::string& prefix, const std::string& str) {
	return str.length() >= prefix.length() && str.substr(0, prefix.length()) == prefix;
}

std::string get_incl_file(const std::string& incl_line) {
	std::string result;
	bool take = false;
	for (char c : incl_line) {
		if (take && c == '>') {
			take = false;
			break;
		}
		if (take) {
			result += c;
		}
		if (!take && c == '<') {
			take = true;
		}
	}
	return result;
}

std::string interpret(const std::string& in_file, bool include_std,
bool no_sl_include, std::set <std::string>& already_included, int argc, char** argv) {
	std::string out;
	std::ifstream inf(in_file);
	if (!inf.is_open()) {
		std::cerr << "fatal: failed to open file '" << in_file << "'" << std::endl;
		exit(0);
	}
	std::string s;
	while (std::getline(inf, s)) {
		bool contin = false;
		for (auto& t : lines_to_skip) {
			if (is_pref(t, s)) {
				contin = true;
				break;
			}
		}
		if (contin) {
			continue;
		}
		if (is_pref("#include <", s)) {
			std::string incl_file = get_incl_file(s);
			if (std_libs.count(incl_file)) {
				if (!include_std) {
					continue;
				}
			}
			if (!is_pref("#include <sl/", s) && !already_included.count(incl_file)) {
				already_included.insert(incl_file);
			}
		}
		if (is_pref("#include <sl/", s)) {
			std::string incl_file = get_incl_file(s);
			if (no_sl_include || already_included.count(incl_file)) {
				continue;
			}
			already_included.insert(incl_file);
			incl_file = path_to_sl + incl_file;
			out += interpret(incl_file, has_flag(argc, argv, "--include_std"),
			has_flag(argc, argv, "--no_sl_include"), already_included, argc, argv) + "\n";
			continue;
		}
		out += s + "\n";
	}
	inf.close();
	return out;
}

int main(int argc, char** argv) {
	
	if (has_flag(argc, argv, "--help") || has_flag(argc, argv, "-h")) {
		help();
		exit(0);
	}
	
	std::string in_file = get_file(argc, argv, "in=");
	std::string ou_file = get_file(argc, argv, "out=");
	if (in_file == "[not found]" || ou_file == "[not found]") {
		std::cerr << "fatal: in/out file not specified" << std::endl;
		exit(0);
	}
	
	load_std_libs();
	
	std::ofstream ouf(ou_file);
	if (!ouf.is_open()) {
		std::cerr << "fatal: failed to open file '" << ou_file << "'" << std::endl;
		exit(0);
	}
	
	std::cerr << "interpreting..." << std::endl;
	
	std::set <std::string> tmp;
	std::string finished = interpret(in_file, true, has_flag(argc, argv, "--no_sl_include"),
	tmp, argc, argv);
	
	std::string to_swap;
	if (!has_flag(argc, argv, "--no_prune")) {
		for (int i = 0; i < (int)finished.length(); i++) {
			if (i + 2 < (int)finished.length() &&
			finished[i] == '\n' && finished[i] == finished[i + 1]
			&& finished[i] == finished[i + 2]) {
				continue;
			}
			to_swap += finished[i];
		}
		while (!to_swap.empty() && to_swap[0] == '\n') {
			to_swap.erase(to_swap.begin());
		}
	} else {
		to_swap = finished;
	}
	
	ouf << to_swap;
	
	ouf.close();
	
	std::cerr << "done" << std::endl;
	
}
