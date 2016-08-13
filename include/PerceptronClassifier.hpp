#ifndef PERCEPTRON_CLASSIFIER_HPP
#define PERCEPTRON_CLASSIFIER_HPP

#include "PerceptronTrainer.hpp"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::istringstream;

class PerceptronClassifier
{
public:
	PerceptronClassifier();
	~PerceptronClassifier();

	string classifyDocument(string);

private:
	void readTrainedDataFromFile();
	void extractDocumentWords(string);

	vector< vector<double> > weights;
	double vocabularySize;
	int transformedDocumentSize;
	vector<string> distinctCategories;
	string* mostFrequentWords;
	WordHashTable* documentWords;
};

#endif
