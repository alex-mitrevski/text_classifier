#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "NaiveBayesTrainer.h"
#include "NaiveBayesClassifier.h"
#include "PerceptronTrainer.h"
#include "PerceptronClassifier.h"
#include "KNNClassifier.h"
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

vector<string> categories;
vector<string> documents;
vector<string> testSet;
vector<string> testCategories;

void readTrainingSetFromFile(const char* fileName);
void readTestSetFromFile(const char* fileName);

int main()
{
	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

	readTrainingSetFromFile("{some file}.txt");
	readValidationSetFromFile("{some file}.txt");

    //Naive Bayes test
	NaiveBayesTrainer* naiveBayes = new NaiveBayesTrainer(categories,documents);
	naiveBayes->trainClassifier();

	NaiveBayesClassifier* naiveBayesClassifier = new NaiveBayesClassifier();
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = naiveBayesClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Naive Bayes - correctly classified documents: " << correctlyClassifiedPercent;

    //KNN test
    correctlyClassified = 0.0;
	KNNClassifier* knnClassifier = new KNNClassifier(documents, categories, 20);
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = knnClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "KNN - correctly classified documents: " << correctlyClassifiedPercent;

    //perceptron test
    correctlyClassified = 0.0;
	PerceptronTrainer* perceptronTrainer = new PerceptronTrainer(documents,categories);
	perceptronTrainer->trainClassifier();

	PerceptronClassifier* perceptronClassifier = new PerceptronClassifier();
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = perceptronClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Perceptron - correctly classified documents: " << correctlyClassifiedPercent;

	getchar();
	return 0;
}

void readTrainingSetFromFile(const char* fileName)
{
	ifstream reader;
	string line;

	reader.open(fileName);
	while(!reader.eof())
	{
		getline(reader,line);

		unsigned int i=0;
		while(line[i] != '\t')
			i++;

		categories.push_back(line.substr(0,i));
		documents.push_back(line.substr(i+1,line.size()-i-2));
	}
	reader.close();
}

void readTestSetFromFile(const char* fileName)
{
	ifstream reader;
	string line;

	reader.open(fileName);
	while(!reader.eof())
	{
		getline(reader,line);

		unsigned int i=0;
		while(line[i] != '\t')
			i++;

		testCategories.push_back(line.substr(0,i));
		testSet.push_back(line.substr(i+1,line.size()-i-2));
	}
	reader.close();
}