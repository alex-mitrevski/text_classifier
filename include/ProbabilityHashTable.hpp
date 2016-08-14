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
	ProbabilityHashTable();
	ProbabilityHashTable(int);
	~ProbabilityHashTable();

	ProbabilityList** getEntries() const;
	int getNumberOfAllowedEntries() const;

	void insert(string,double);
	int hash(int) const;
	double countNonemptyEntries() const;
	double getProbability(string) const;

private:
	ProbabilityList** entries;
	int numberOfAllowedEntries;
};

#endif
