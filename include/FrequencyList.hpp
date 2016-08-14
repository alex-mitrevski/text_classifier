#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <string>
#include "Structs.hpp"
using std::string;

class FrequencyList
{
public:
	FrequencyList();
	~FrequencyList();

	node* getHead() const;

	void insert(string);
	void remove(string);
	int findFrequency(string) const;
	string findMaxFrequency() const;
	int size() const;
	bool isEmpty() const;

private:
	node* head;
	node* tail;
	int listSize;
};

#endif
