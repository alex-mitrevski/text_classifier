#ifndef LOGISTIC_REGRESSION_CLASSIFIER_HPP
#define LOGISTIC_REGRESSION_CLASSIFIER_HPP

#include "LogisticRegressionTrainer.hpp"
#include "FrequencyHashTable.hpp"
#include "DocumentTransformer.hpp"
#include "Structs.hpp"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::istringstream;

class LogisticRegressionClassifier
{
public:
	LogisticRegressionClassifier(string);
	~LogisticRegressionClassifier();

	string classifyDocument(string) const;

private:
	void readTransformedDocumentsFromFile();

	vector< vector<double> > weights;
	string encoding;
	vector<string> distinctCategories;
	DocumentTransformer* documentTransformer;
};

#endif
