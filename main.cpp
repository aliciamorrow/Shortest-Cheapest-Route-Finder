#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>      
#include <stdlib.h>
#include "Node.h"
#include <vector>
#include <memory>
#include <algorithm>

#define INFINITY 9999999999999999999U;

using namespace std;

struct CompareEdge                                                                                   
{  
  bool operator()( Edge* lhs,   Edge* rhs) const  
  {  
		 return (lhs -> getWeight() > rhs -> getWeight());  
  }  
};



int main(int argc, char* argv[])
{
	string country, city, latitude, longitude;
	string start, destination, outputFile, criteria, startingCity, endCity, startingPoint, endDestination;
	string transportation, aveTime, aveCost, junk;
	map<string, vector<Edge*> > graph; 
	map<string, Node*> nodeGraph;

	vector<Edge*> *child = new vector<Edge*>();
	char* convertString(string s);
	priority_queue<Edge*, vector<Edge*>, CompareEdge > *q = new priority_queue<Edge*, vector<Edge*>, CompareEdge >();
	map<string, double> distance;
	map<Node*, Edge*> previous;
	map<string, bool> visited;
	double length, newTime, newCost;
	string cityFile, routeFile;



	if(argc > 2) {
		cityFile = argv[1];
		routeFile = argv[2];
		outputFile = argv[3];
		startingPoint = argv[4];
		endDestination = argv[5]; 
		criteria = argv[6];
		startingPoint.erase(remove(startingPoint.begin(), startingPoint.end(), '\''), startingPoint.end());
		endDestination.erase(remove(endDestination.begin(), endDestination.end(), '\''), endDestination.end());
	}


	//-----------------------------------------------------------------------------------------

	//PARSE NODES
	ifstream nodefile(cityFile.c_str());
	while(nodefile.good())
	{
		while(!nodefile.eof()) {
		 getline (nodefile, country, ',' ); 
		 getline (nodefile, city, ',' ); 
		 getline (nodefile, latitude, ',' ); 
		 getline (nodefile, longitude); 
		 
		 if(country == "") {
			 break;
		 }
		
		 country.erase(remove(country.begin(), country.end(), '\''), country.end());
		 city.erase(remove(city.begin(), city.end(), '\''), city.end());
		 
		 Node *n = new Node(country, city, latitude, longitude);
		 nodeGraph[city] = n;
		 distance[city] = INFINITY;
		 previous[n] = NULL;
		 visited[city] = false;
		 if(country == startingPoint) {
			 startingCity = city;
		 } 

		 if(country == endDestination) {
			 endCity = city;
		 }
		}
	} 

	
	//PARSE EDGES
	ifstream edgefile(routeFile.c_str());
	while(edgefile.good()) 
	{	
		while(!edgefile.eof()) {
			getline(edgefile, start, ',');
			getline(edgefile, destination, ',');
			getline(edgefile, transportation, ',');
			getline(edgefile, aveTime, ',');
			getline(edgefile, aveCost, ',');
			getline(edgefile, junk);

			start.erase(remove(start.begin(), start.end(), '\''), start.end());
			destination.erase(remove(destination.begin(), destination.end(), '\''), destination.end());

			newTime = atof(aveTime.c_str());
			newCost = atof(aveCost.c_str());

			if(start == "") {
				break;
			} 
			
			Edge *e = new Edge(start, destination, transportation, newTime, newCost);
			vector<Edge*> *child = new vector<Edge*>();
			*child = graph[e -> start];
			child -> push_back(e);
			graph[start] = *child;
			e -> mark = false;
		} 
	}
	

	//-----------------------------------------------------------------------------------------


	//DIJKSTRA'S ALGORITHM
	distance[startingCity] = 0;
	
	//Initializing and pushing all starting edges from the starting city onto min heap
	for(int i = 0; i < graph[startingCity].size(); i++) {
		double compare = graph[startingCity].at(i) -> setVariables(criteria);
		graph[startingCity].at(i) -> setWeight(compare);
		distance[graph[startingCity].at(i) -> destination] = compare;
		q -> push(graph[startingCity].at(i));
	}
	
	
	while(!q -> empty()) {
		//reheap if top edge has been changed
		if(q -> top() -> mark == true) {
			Edge *b = q -> top();
			b -> mark =  false;
			q -> pop();
			q -> push(b);
		}

		//if the destination is already visited don't visit again
		while(visited[q-> top() -> destination] == true) {
			q-> pop();
		}

		//unvisited top edge is current edge being visited
		Edge *current = q -> top();
		visited[current -> start]  = true;
		q -> pop();

		//if it is the end destination break
		if(current -> destination == endCity) {
			break;
		}
		
		//setting previous edge for starting city
		if(current -> start == startingCity) {
			previous[nodeGraph[current -> destination]] = current;
		}

		//if edge has not been visited, compare distances and set variables and push onto heap
		for(int j = 0; j < graph[current -> destination].size(); j++) {
			Edge *currentEdge = graph[current -> destination].at(j);
			if(visited[currentEdge -> destination] == false) {
 			double compare = graph[current -> destination].at(j) -> setVariables(criteria);
				double newWeight = distance[currentEdge -> start] + compare;
				if(newWeight < distance[currentEdge -> destination]) {
					currentEdge -> mark = true;
					distance[currentEdge -> destination] = newWeight;
					currentEdge -> setWeight(newWeight);
					previous[nodeGraph[currentEdge -> destination]] = currentEdge;
					q -> push(currentEdge);
				}
			} 
		} 
	}


	//MAKING HTML FILE
	Node *node = nodeGraph[endCity];
	ofstream myFile;
	myFile.open(outputFile.c_str());
	myFile << "<HTML><HEAD><TITLE>Shortest path from " << startingPoint << " to " << endDestination << "</TITLE></HEAD><script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script><script>function initialize() { var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};var map=new google.maps.Map(document.getElementById('map'), myOptions);\n";
	int count = 0;
	int contentString = 0;
	double total = 0;
	Node *copy;
	while(previous[node] != NULL) {
		myFile << "\nvar marker" << count << " = new google.maps.Marker({ position: new google.maps.LatLng(" << node -> latitude << ", " << node -> longitude << "), map: map, title: '" << node->city << ", " << node-> country << "'});";
		count++;
		copy = node;
		node = nodeGraph[previous[node] -> start];
		myFile << "\nvar marker" << count << " = new google.maps.Marker({ position: new google.maps.LatLng(" << node -> latitude << ", " << node -> longitude << "), map: map, title: '" << node->city << ", " << node-> country << "'});" << endl;
		myFile << "var contentString" << contentString <<" = \"";
		myFile << node -> city << ", " << node -> country << " --> " << copy -> city << ", " << copy -> country << "(" << previous[copy] -> getTransportation() << " - " << previous[copy] -> getTime() << " hours - $" << previous[copy] -> getCost() << ")\"";
		myFile << "; var path" << contentString <<" = new google.maps.Polyline({ path: [new google.maps.LatLng(" << node -> latitude << ", " << node -> longitude << "), new google.maps.LatLng(" << copy -> latitude << ", "<< copy -> longitude<< ")], strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2}); path" << contentString << ".setMap(map); google.maps.event.addListener(path" << contentString << ", 'click', function(event) { alert(contentString" << contentString << "); });" << endl;
		count++;
		contentString++;
	}

	myFile <<"\n\n} google.maps.event.addDomListener(window, 'load', initialize); </script></HEAD><BODY><div id='map' style='width:100%;height:100%;'></div></BODY></HTML>";
	myFile.close();
	return 0;
}

