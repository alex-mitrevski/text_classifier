#ifndef LOGISTIC_REGRESSION_TRAINER_HPP
#define LOGISTIC_REGRESSION_TRAINER_HPP

#include "FrequencyHashTable.hpp"
#include "Structs.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::istringstream;
using std::cout;

class LogisticRegressionTrainer
{
public:
	LogisticRegressionTrainer(vector<string>,vector<string>,string);
	~LogisticRegressionTrainer();

	void trainClassifier(double learningRate=1.0);

private:
	void findDistinctCategories();
	void countVocabulary();
	void readTransformedDocumentsFromFile();
	void initializeWeights();
	double logisticFunction(double);
	void saveTrainedDataToFile();

	vector<string> documents;
	vector<string> categories;
	vector<string> distinctCategories;
	string encoding;
	vector< vector<transformedDocumentFeature> > transformedDocuments;
	double** weights;
	FrequencyHashTable* words;
	double vocabularySize;
	int transformedDocumentSize;
};

#endif
