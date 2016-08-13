#ifndef CLASSIFYING_HASH_TABLE_HPP
#define CLASSIFYING_HASH_TABLE_HPP

#include "ProbabilityList.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class ProbabilityHashTable
{
public:
	//constructors
	ProbabilityHashTable();
	ProbabilityHashTable(int);
	~ProbabilityHashTable();

	//accessor functions
	ProbabilityList** getEntries();
	int getNumberOfAllowedEntries();

	//data handling functions
	void insert(string,double);
	int hash(int);
	double countNonemptyEntries();
	double getProbability(string);

private:
	ProbabilityList** entries;
	int numberOfAllowedEntries;
};

#endif
