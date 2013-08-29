#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class Edge
{
public:
	string start, destination, transportation, aveTime, aveCost;
	double newTime, newCost;
	double weight;
	bool mark;
	map<string, vector<Edge*> > graph;
	map<string, bool> visited;
	map<string, double> distance; 

	Edge(void);
	Edge(string start, string destination, string transportation, double newTime, double newCost);
	~Edge(void);
	void setTime(double newTime);
	double getTime();
	void setCost(double newCost);
	double getCost();
	double getWeight();
	void setTransportation(string setTransportation);
	string getTransportation();
	void setWeight(double weight);
	double setVariables(string criteria);
};

