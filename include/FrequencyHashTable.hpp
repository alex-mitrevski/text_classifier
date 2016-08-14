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
	FrequencyHashTable();
	FrequencyHashTable(int);
	~FrequencyHashTable();

	FrequencyList** getEntries() const;
	int getNumberOfAllowedEntries() const;

	void insert(string);
	int hash(int) const;
	double countNonemptyEntries() const;
	double getFrequency(string) const;
	string* getXMostFrequentWords(int x) const;

private:
	Heap* convertToMaxHeap() const;

	FrequencyList** entries;
	int numberOfAllowedEntries;
};

#endif
