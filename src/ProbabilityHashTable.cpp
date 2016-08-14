#include "ProbabilityHashTable.hpp"

ProbabilityHashTable::ProbabilityHashTable()
{
	this->numberOfAllowedEntries = 0;
	this->entries = NULL;
}

ProbabilityHashTable::ProbabilityHashTable(int numberOfAllowedEntries)
{
	this->numberOfAllowedEntries = numberOfAllowedEntries;
	this->entries = new ProbabilityList*[this->numberOfAllowedEntries];
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		this->entries[i] = new ProbabilityList();
}

ProbabilityHashTable::~ProbabilityHashTable()
{
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		delete this->entries[i];
	delete this->entries;
	this->numberOfAllowedEntries = 0;
}

ProbabilityList** ProbabilityHashTable::getEntries() const
{
	return this->entries;
}

int ProbabilityHashTable::getNumberOfAllowedEntries() const
{
	return this->numberOfAllowedEntries;
}


void ProbabilityHashTable::insert(string data, double probability)
{
	int letterSum = 0;
	for(unsigned int i=0; i<data.size(); i++)
	{
		data[i] = tolower(data[i]);
		letterSum += data[i] - 96; //96 is the character before 'a'
	}

	int key = this->hash(letterSum);
	this->entries[key]->insert(data,probability);
}

int ProbabilityHashTable::hash(int key) const
{
	return key % this->numberOfAllowedEntries;
}

double ProbabilityHashTable::countNonemptyEntries() const
{
	int counter = 0;
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		counter += this->entries[i]->size();
	return counter;
}

double ProbabilityHashTable::getProbability(string data) const
{
	int letterSum = 0;
	for(unsigned int i=0; i<data.size(); i++)
	{
		data[i] = tolower(data[i]);
		letterSum += data[i] - 96; //96 is the character before 'a'
	}

	int key = this->hash(letterSum);
	return this->entries[key]->getProbability(data);
}
