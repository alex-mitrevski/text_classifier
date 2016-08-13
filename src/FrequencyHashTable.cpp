#include "FrequencyHashTable.hpp"

//constructors
FrequencyHashTable::FrequencyHashTable()
{
	this->numberOfAllowedEntries = 0;
	this->entries = NULL;
}

FrequencyHashTable::FrequencyHashTable(int numberOfAllowedEntries)
{
	this->numberOfAllowedEntries = numberOfAllowedEntries;
	this->entries = new FrequencyList*[this->numberOfAllowedEntries];
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		this->entries[i] = new FrequencyList();
}

FrequencyHashTable::~FrequencyHashTable()
{
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		delete this->entries[i];
	delete this->entries;
	this->numberOfAllowedEntries = 0;
}

//accessor functions
FrequencyList** FrequencyHashTable::getEntries()
{
	return this->entries;
}

int FrequencyHashTable::getNumberOfAllowedEntries()
{
	return this->numberOfAllowedEntries;
}


//data handling functions
void FrequencyHashTable::insert(string data)
{
	int letterSum = 0;
	for(unsigned int i=0; i<data.size(); i++)
	{
		data[i] = tolower(data[i]);
		letterSum += data[i] - 96; //96 is the character before 'a'
	}

	int key = this->hash(letterSum);
	this->entries[key]->insert(data);
}

int FrequencyHashTable::hash(int key)
{
	return key % this->numberOfAllowedEntries;
}

double FrequencyHashTable::countNonemptyEntries()
{
	int counter = 0;
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		counter += this->entries[i]->size();
	return counter;
}

double FrequencyHashTable::getFrequency(string data)
{
	int letterSum = 0;
	for(unsigned int i=0; i<data.size(); i++)
	{
		data[i] = tolower(data[i]);
		letterSum += data[i] - 96; //96 is the character before 'a'
	}

	int key = this->hash(letterSum);
	return this->entries[key]->findFrequency(data);
}

string* FrequencyHashTable::getXMostFrequentWords(int x)
{
	Heap* maxHeap = this->convertToMaxHeap();
	string* words = new string[x];

	for(int i=0; i<x; i++)
		words[i] = maxHeap->extractMaximum();
	delete maxHeap;
	maxHeap = NULL;

	return words;
}

Heap* FrequencyHashTable::convertToMaxHeap()
{
	Heap* heap = new Heap();
	node* traversalNode;

	for(int i=0; i<this->numberOfAllowedEntries; i++)
	{
		traversalNode = this->entries[i]->getHead();

		while(traversalNode!=NULL)
		{
			heap->insert(*traversalNode);
			traversalNode = traversalNode->next;
		}
	}

	return heap;
}
