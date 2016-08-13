#ifndef TRAINING_HASH_TABLE_HPP
#define TRAINING_HASH_TABLE_HPP

#include "FrequencyList.hpp"
#include "Heap.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class FrequencyHashTable
{
public:
	//constructors
	FrequencyHashTable();
	FrequencyHashTable(int);
	~FrequencyHashTable();

	//accessor functions
	FrequencyList** getEntries();
	int getNumberOfAllowedEntries();

	//data handling functions
	void insert(string);
	int hash(int);
	double countNonemptyEntries();
	double getFrequency(string);
	string* getXMostFrequentWords(int x);

private:
	Heap* convertToMaxHeap();

	FrequencyList** entries;
	int numberOfAllowedEntries;
};

#endif
