#ifndef WORD_LIST_HPP
#define WORD_LIST_HPP

#include <string>
#include "Structs.hpp"
using std::string;

class WordList
{
public:
	//constructor
	WordList();

	//accessor function
	wordNode* getHead();

	//data access functions
	void insert(string);
	void remove(string);
	bool exists(string data);
	int size();
	bool isEmpty();

private:
	wordNode* head;
	int listSize;
};

#endif
