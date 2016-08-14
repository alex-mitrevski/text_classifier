#ifndef CLASSIFIER_LIST_HPP
#define CLASSIFIER_LIST_HPP

#include <iostream>
#include <string>
#include "Structs.hpp"
using std::string;

class ProbabilityList
{
public:
	ProbabilityList();
	~ProbabilityList();

	probabilityNode* getHead() const;

	void insert(string,double);
	void remove(string);
	double getProbability(string) const;
	int size() const;
	bool isEmpty() const;

private:
	probabilityNode* head;
	probabilityNode* tail;
	int listSize;
};

#endif
