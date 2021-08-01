#include <fstream>
#include <iostream>
#include <string>

const std::string in_file = "/usr/local/include/sl/templates/template.cpp";

int main(int argc, char** argv) {
	
	if (argc != 2) {
		std::cerr << "fatal: bad number of arguments (" << argc << ")" << std::endl;
		exit(0);
	}
	
	std::string ou_file = std::string(argv[1]) + "/template.cpp";
	
	std::string result;
	
	std::ifstream inf(in_file);
	if (!inf.is_open()) {
		std::cerr << "fatal: failed to open file '" << in_file << "'" << std::endl;
		exit(0);
	}
	
	std::string s;
	while (std::getline(inf, s)) {
		result += s + "\n";
	}
	
	inf.close();
	
	std::ofstream ouf(ou_file);
	if (!ouf.is_open()) {
		std::cerr << "fatal: failed to open file '" << ou_file << "'" << std::endl;
		exit(0);
	}
	
	ouf << result;
	
	ouf.close();
	
}
