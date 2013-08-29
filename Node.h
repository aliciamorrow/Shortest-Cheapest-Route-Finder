#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>
#include <string>
#include <queue>
#include <vector>
#include "Edge.h"
#include <memory>
#include <map>
#include <functional>


using namespace std;



class Node
{
public:
	string country, city, latitude, longitude;


	Node(void);
	Node(string country, string city, string latitude, string longitude);
	~Node(void);
};

