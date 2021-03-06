#include "WordHashTable.hpp"

WordHashTable::WordHashTable()
{
	this->numberOfAllowedEntries = 0;
	this->entries = NULL;
}

WordHashTable::WordHashTable(int numberOfAllowedEntries)
{
	this->numberOfAllowedEntries = numberOfAllowedEntries;
	this->entries = new WordList*[this->numberOfAllowedEntries];
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		this->entries[i] = new WordList();
}

WordHashTable::~WordHashTable()
{
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		delete this->entries[i];
	delete this->entries;
	this->numberOfAllowedEntries = 0;
}

WordList** WordHashTable::getEntries() const
{
	return this->entries;
}

int WordHashTable::getNumberOfAllowedEntries() const
{
	return this->numberOfAllowedEntries;
}


void WordHashTable::insert(string data)
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

int WordHashTable::hash(int key) const
{
	return key % this->numberOfAllowedEntries;
}

double WordHashTable::countNonemptyEntries() const
{
	int counter = 0;
	for(int i=0; i<this->numberOfAllowedEntries; i++)
		counter += this->entries[i]->size();
	return counter;
}

bool WordHashTable::exists(string data) const
{
	int letterSum = 0;
	for(unsigned int i=0; i<data.size(); i++)
	{
		data[i] = tolower(data[i]);
		letterSum += data[i] - 96; //96 is the character before 'a'
	}

	int key = this->hash(letterSum);
	if(this->entries[key]->exists(data))
		return true;
	else
		return false;
}
