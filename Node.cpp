#include "stdafx.h"
#include "Node.h"



Node::Node(void)
{
}

Node::Node(string country, string city, string latitude, string longitude) {
	this -> country = country;
	this -> city = city;
	this -> latitude = latitude;
	this -> longitude = longitude;
}


Node::~Node(void)
{
}

