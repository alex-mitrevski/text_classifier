#ifndef CLASSIFIER_LIST_HPP
#define CLASSIFIER_LIST_HPP

#include <iostream>
#include <string>
#include "Structs.hpp"
using std::string;

class ProbabilityList
{
public:
	//constructor
	ProbabilityList();
	~ProbabilityList();

	//accessor functions
	probabilityNode* getHead();

	//data handling functions
	void insert(string,double);
	void remove(string);
	double getProbability(string);
	int size();
	bool isEmpty();

private:
	probabilityNode* head;
	probabilityNode* tail;
	int listSize;
};

#endif
