#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <string>
#include "Structs.hpp"
using std::string;

class FrequencyList
{
public:
	//constructor
	FrequencyList();
	~FrequencyList();

	//accessor functions
	node* getHead();

	//data handling functions
	void insert(string);
	void remove(string);
	int findFrequency(string);
	string findMaxFrequency();
	int size();
	bool isEmpty();

private:
	node* head;
	node* tail;
	int listSize;
};

#endif
