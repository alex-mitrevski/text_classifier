#ifndef WORD_LIST_HPP
#define WORD_LIST_HPP

#include <string>
#include "Structs.hpp"
using std::string;

class WordList
{
public:
	WordList();

	wordNode* getHead() const;

	void insert(string);
	void remove(string);
	bool exists(string data) const;
	int size() const;
	bool isEmpty() const;

private:
	wordNode* head;
	int listSize;
};

#endif
