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
	WordHashTable();
	WordHashTable(int);
	~WordHashTable();

	WordList** getEntries() const;
	int getNumberOfAllowedEntries() const;

	void insert(string);
	int hash(int) const;
	double countNonemptyEntries() const;
	bool exists(string) const;

private:
	WordList** entries;
	int numberOfAllowedEntries;
};

#endif
