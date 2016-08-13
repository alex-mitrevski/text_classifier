#ifndef PROBABILISTIC_NEURAL_NETWORK_H
#define PROBABILISTIC_NEURAL_NETWORK_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include "DocumentTransformer.hpp"
#include "Structs.hpp"
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::istringstream;

class ProbabilisticNeuralNetwork
{
public:
	ProbabilisticNeuralNetwork(unsigned int,vector<string>,string,float);
	~ProbabilisticNeuralNetwork();

	string classifyDocument(string);

private:
	void findDistinctCategories();
	void readMostFrequentWordsFromVocabulary();
	void convertCategoriesToNumbers();
	void countDocumentsInEachCategory();
	void readTransformedDocumentsFromFile();

	unsigned int numberOfDocuments;
	vector<string> categories;
	vector<string> distinctCategories;
	vector< vector<transformedDocumentFeature> > transformedDocuments;	//used to store the feature vectors for each document
	vector<transformedDocumentFeature> transformedNewDocument;
	int* categoriesInNumbers;
	int* documentsInEachCategory;
	unsigned int transformedDocumentSize;
	DocumentTransformer* documentTransformer;
	string encoding;
	float sigmaSquared;
};

#endif
