#ifndef NAIVE_BAYES_CLASSIFIER_HPP
#define NAIVE_BAYES_CLASSIFIER_HPP

#include "ProbabilityHashTable.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;
using std::istringstream;

class NaiveBayesClassifier
{
public:
	NaiveBayesClassifier();
	~NaiveBayesClassifier();

	string classifyDocument(string);

private:
	void readTrainedData();
	void readCategoryData(string,unsigned int);
	string findMaximumProbabilityCategory(vector<double>);

	vector<string> distinctCategories;
	ProbabilityHashTable** words;
	double vocabularySize;
	double categoryPrior;
	double numberOfCategoryWords;
};

#endif
