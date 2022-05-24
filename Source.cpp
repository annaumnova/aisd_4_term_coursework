#include "FordFulkerson.h"

int main() {

    //information about graph
    List<GraphCities> vertexes;
    List<std::string> str;

    //parse the information
    parse(&vertexes, &str);

    //create graph
    createGraph(&vertexes, &str);

    //Ford-Fulkerson algorithm
     FordFulkerson(&vertexes, &str);
    
	return 0;
}