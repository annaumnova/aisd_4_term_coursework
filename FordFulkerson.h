#pragma once
#include"List.h"
#include<stdio.h>
#include<fstream>
#include<string>

//consts
const int NA = INT_MIN; //no arrive
const int number = -1;
const int zero = 0;

const int maximumEdge = (int)1e6;
const int maximumVertex = (int)1e3;
const int infinity = INT_MAX;

int flowCapacity[maximumEdge]; //the throughput
int endStraightPath[maximumEdge]; //the end of straight path
int nextPath[maximumEdge]; //the next path (path of graph)

int firstPath[maximumVertex]; //the new path
int visitedVertex[maximumVertex]; //the visited vertex

int numberVertex; //the number of vertex
int numberPath; //the number of path (path of graph)
int start; //the beginning vetrex
int arrivalVertex; //the vertex to arrive
int counterPath; //the counter for paths


//class graph
class GraphCities {

public:

	//members
	int price; //the price of flight
	std::string departureCity; //the city you go from
	int departNumber = number;
	std::string arrivalCity; //the city you go to
	int arriveNumber = number;

	//constructor
	GraphCities(std::string departureCity, std::string arrivalCity, int price, int departNumber, int arriveNumber) {
		this->departureCity = departureCity;
		this->arrivalCity = arrivalCity;
		this->price = price;
		this->departNumber = departNumber;
		this->arriveNumber = arriveNumber;
	}

	//destructor
	GraphCities() {
		this->departureCity = "\0";
		this->arrivalCity = "\0";
		this->price = 0;
		this->departNumber = number;
		this->arriveNumber = number;
	}
};

//forming graph, name the vertexes, destination "from" "to"
void createGraph(List<GraphCities>* namesPath, List<std::string>* amountStrings) {

	int amountPaths = namesPath->getSize(); //how are paths named there?
	int sizeAmountStrings = amountStrings->getSize(); //how are strings named there?

	//"ordered pair"
	for (int i = 0; i < amountPaths; i++) {
		for (int j = 0; j < sizeAmountStrings; j++) {

			//depart city
			if ((*namesPath)[i].departureCity == (*amountStrings)[j])
				(*namesPath)[i].departNumber = j;

			//arrive city
			if ((*namesPath)[i].arrivalCity == (*amountStrings)[j])
				(*namesPath)[i].arriveNumber = j;
		}
	}
}

//get vertexes (tops of graph), there're cities counted
int getVertexesCities(List<std::string>* cities) {

	if (cities->getSize() == 0) //empty?
		return number;
	else {
		int amount = cities->getSize(); //how are cities there?

		for (int i = 0; i < amount - 1; i++) {
			for (int j = i + 1; j < amount; j++) {
				if ((*cities)[i] == (*cities)[j]) {
					cities->remove(j);
					amount--;
					j--;
				}
			}
		}

		return cities->getSize();
	}
}



//to add straight path and reverse path
// u == the beginning end of the rib
// v == the ending end of the rib
void addPath(int u, int v, int throughput) {
	
	//to form straight path
	endStraightPath[counterPath] = v; //the end of straight
	nextPath[counterPath] = firstPath[u]; //add in beginning for u
	firstPath[u] = counterPath; //the new path
	
	//fix throughput
	flowCapacity[counterPath++] = throughput;
	
	//the same actions for reverse path
	endStraightPath[counterPath] = u;
	nextPath[counterPath] = firstPath[v];
	firstPath[v] = counterPath;
	flowCapacity[counterPath++] = zero;
}

//find any path from beginning to stok
int findFlowPath(int u, int flow) {

	//return the minimum flow 
	if (u == arrivalVertex)
		return flow;

	//change visited vertex (it's already visited)
	visitedVertex[u] = true;

	//to pass the paths
	for (int path = firstPath[u]; path != number; path = nextPath[path]) {
		
		int arrive = endStraightPath[path]; //arrival point

		if (!visitedVertex[arrive] && flowCapacity[path] > 0) {
			
			//to find minimum
			int minimum = findFlowPath(arrive, std::min(flow, flowCapacity[path]));

			//minimum is not zero
			if (minimum > zero) {
				
				//change throughput
				flowCapacity[path] -= minimum; //for straight
				flowCapacity[path ^ 1] += minimum; //for reserve
				
				return minimum;
			}
		}
	}

	return zero; //didn't find, return zero to vertex
}

//Ford-Fulkerson algorithm
int FordFulkerson(List<GraphCities>* namesPath, List<std::string>* amountStrings) {
	
	//fix size of paths
	int sizePaths = namesPath->getSize();

	//to populate default value
	std::fill(firstPath, firstPath + maximumVertex, number);

	//fix start position
	start = namesPath->head->inf.departNumber;

	//fix end position
	arrivalVertex = namesPath->end->inf.arriveNumber;

	//to add paths
	for (int i = 0; i < sizePaths; i++)
		addPath((*namesPath)[i].departNumber,
			(*namesPath)[i].arriveNumber,
			(*namesPath)[i].price);

	//fix the maximum flow and counter
	int maximumFlow = 0;
	int maximumCounter = 0;

	//define the maximum flow
	while ((maximumCounter = findFlowPath(start, infinity)) > zero) {

		std::fill(visitedVertex, visitedVertex + maximumVertex, false);
		maximumFlow += maximumCounter;

	}
	
	std::cout << "Maximum flow: " << maximumFlow << "\n";
	return maximumFlow;
}


//parse strings in file
void parse(List<GraphCities>* namesPath, List<std::string>* amountStrings) {

	//open file
	std::fstream file;
	file.open("flight.txt", std::ios_base::in);

	//check, then parse
	if (file.good()) {

		//elements for separate, counters
		int counterPos = 0;
		std::string line;
		std::string subline;

		while (std::getline(file, line)) { //read

			//set the paths
			GraphCities firstCityPath;
			GraphCities secondCityPath;

			//go ;
			subline = line;
			counterPos = line.find(';');

			//new size and add (increase amount counter)
			subline.resize(counterPos);
			amountStrings->pushBack(subline);

			//there are cities
			firstCityPath.departureCity = subline;
			secondCityPath.arrivalCity = subline;

			//clear at position in line
			line.erase(0, counterPos + 1);

			//the back path
			subline = line;
			counterPos = line.find(';');
			subline.resize(counterPos);
			amountStrings->pushBack(subline);
			firstCityPath.arrivalCity = subline;
			secondCityPath.departureCity = subline;
			line.erase(0, counterPos + 1);

			//definite the price for straight and back paths
			subline = line;
			subline.resize(counterPos);

			if (subline != "N/A") { //it's arrive
				firstCityPath.price = stoi(subline);
				namesPath->pushBack(firstCityPath);
			}

			secondCityPath.price = number;
		}
	}
	else
		std::cout << "Error";

	file.close(); //close
}