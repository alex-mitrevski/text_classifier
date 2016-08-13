#ifndef WordHashTable_HPP
#define WordHashTable_HPP

#include "WordList.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class WordHashTable
{
public:
	//constructors/destructor
	WordHashTable();
	WordHashTable(int);
	~WordHashTable();

	//accessor functions
	WordList** getEntries();
	int getNumberOfAllowedEntries();

	//data handling functions
	void insert(string);
	int hash(int);
	double countNonemptyEntries();
	bool exists(string);

private:
	WordList** entries;
	int numberOfAllowedEntries;
};

#endif
