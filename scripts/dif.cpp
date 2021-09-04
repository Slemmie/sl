#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <fstream>
#include <set>

std::vector <std::vector <std::string>> files;
std::vector <std::string> file_names;
int max_line = 0;
std::vector <int> max_col;

struct Diff {
	
	int line, col;
	
	Diff(int _line, int _col) :
	line(_line),
	col(_col)
	{ }
	
	inline void print() {
		std::vector <std::stringstream> ss(files.size());
		std::stringstream stream;
		stream << "> [" << line << ":" << col << "]:";
		for (int f = 0; f < (int)files.size(); f++) {
			if (files[f][line].empty()) {
				ss[f] << "\033[31m" << "'-empty-'" << "\033[0m";
				stream << "\n" << ss[f].str();
				continue;
			}
			ss[f] << "    ";
			int name_ptr = 0;
			for (; name_ptr < std::min(6, (int)file_names[f].length()); name_ptr++) {
				ss[f] << file_names[f][name_ptr];
			}
			for (; name_ptr < 6; name_ptr++) {
				ss[f] << ".";
			}
			ss[f] << ": '";
			ss[f] << "\033[90m" << "..." << "\033[0m";
			assert(col < (int)files[f][line].length());
			for (int i = std::max(0, col - 5); i < col; i++) {
				ss[f] << files[f][line][i];
			}
			std::string diff;
			for (int i = col; i < std::min(col + 25, (int)files[f][line].length()); i++) {
				ss[f] << "\033[31m" << files[f][line][i] << "\033[0m";
			}
			ss[f] << "\033[90m" << "..." << "\033[0m";
			ss[f] << "'";
			stream << "\n" << ss[f].str();
		}
		stream << "\n";
		std::cerr << stream.str();
		std::cerr.flush();
	}
	
};

std::vector <Diff> diffs;

void read_file(const std::string& file) {
	file_names.push_back(file);
	files.push_back({ });
	std::ifstream inf(file);
	std::string s;
	int line = 0;
	while (std::getline(inf, s)) {
		files.back().push_back(s);
		max_line = std::max(max_line, ++line);
		if (line - 1 >= (int)max_col.size()) {
			max_col.resize(line, 0);
		}
		max_col[line - 1] = std::max(max_col[line - 1], (int)s.length());
	}
	inf.close();
}

void find_diffs() {
	for (int line = 0; line < max_line; line++) {
		for (int col = 0; col < max_col[line]; col++) {
			bool is_diff = false;
			std::set <char> st;
			for (int f = 0; f < (int)files.size(); f++) {
				if (col >= (int)files[f][line].length()) {
					is_diff = true;
					break;
				}
				st.insert(files[f][line][col]);
			}
			is_diff |= (int)st.size() > 1;
			if (is_diff) {
				diffs.push_back(Diff(line, col));
				break;
			}
		}
	}
}

int main(int argc, char** argv) {
	
	if (argc < 3) {
		std::cerr << "fatal: too few arguments (" << argc << ")" << std::endl;
		return 1;
	}
	
	for (int i = 1; i < argc; i++) {
		read_file(std::string(argv[i]));
	}
	
	find_diffs();
	
	if (diffs.empty()) {
		std::cerr << "\033[92m" << "no diffs" << "\033[0m" << std::endl;
		return 0;
	}
	
	for (auto& d : diffs) {
		d.print();
	}
	
}
