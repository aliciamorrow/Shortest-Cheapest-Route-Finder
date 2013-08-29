#pragma once
struct CompareNode
{
	  bool operator()(const Node* lhs, const Node* rhs) const  
  {  
    //if ( lhs -> distance < rhs -> distance ) return true;
    //if ( rhs -> distance  < lhs -> distance ) return false;
    return lhs -> distance > rhs -> distance;  
  } 
public:
	CompareNode(void);
	~CompareNode(void);
};

