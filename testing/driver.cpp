#include <sl/graph.h>

int main() {
	
	Graph <int> graph(10);
	graph.add(2, 5);
	std::cout << graph << std::endl;
	
}
