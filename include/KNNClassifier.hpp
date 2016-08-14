#ifndef KNNCLASSIFIER_HPP
#define KNNCLASSIFIER_HPP

#include "WordHashTable.hpp"
#include "FrequencyList.hpp"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
using std::vector;
using std::string;

class KNNClassifier
{
public:
	KNNClassifier(vector<string>, vector<string>,int kParameter = 1);
	string classifyDocument(string) const;

private:
	void quickSort(vector<unsigned int>&, vector<int>&, unsigned int, unsigned int) const;
	unsigned int partition(vector<unsigned int>&, vector<int>&, unsigned int, unsigned int) const;
	void swap(int& a, int& b) const;
	void swap(unsigned int& a, unsigned int& b) const;

	vector<string> categories;
	vector<string> documents;
	WordHashTable** documentWords;
	unsigned int kParameter;
};

#endif
