#include "stdafx.h"
#include "Edge.h"



Edge::Edge(void)
{
}

Edge::Edge(string start, string destination, string transportation, double newTime, double newCost) {
	this -> start = start;
	this -> destination = destination;
	this -> transportation = transportation;
	this -> newTime = newTime;
	this -> newCost = newCost;
}



Edge::~Edge(void)
{
}

void Edge::setCost(double newCost) {
	this -> newCost = newCost;
}

void Edge::setTime(double newTime) {
	this -> newTime = newTime;
}

double Edge::getCost() {
	return newCost;
}

double Edge::getTime() {
	return newTime;
}

void Edge::setTransportation(string transportation) {
	this -> transportation = transportation;
}

string Edge::getTransportation() {
	return transportation;
}

double Edge::getWeight() {
	return weight;
}

void Edge::setWeight(double weight) {
	this -> weight = weight;
}

double Edge::setVariables(string criteria) {
	if(criteria == "cheapest") {
		double weights = getCost();
		if(getTransportation() == "plane") {
			weights *= 500;
		}
		return weights;
	} else if(criteria == "fastest") {
		double weights = getTime();
		if(getTransportation() == "plane") {
			weights *= 500;
		}
		return weights;
	}
}