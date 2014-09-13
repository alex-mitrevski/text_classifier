#ifndef LOGISTIC_REGRESSION_TRAINER_H
#define LOGISTIC_REGRESSION_TRAINER_H

#include "FrequencyHashTable.h"
#include "WordHashTable.h"
#include <vector>
#include <string>
#include <fstream>
using std::vector;
using std::string;
using std::ofstream;

const double INFINITY = 1000000000000.0;

class LogisticRegressionTrainer
{
public:
	LogisticRegressionTrainer(vector<string>, vector<string>);
	~LogisticRegressionTrainer();

	void trainClassifier();

private:
	void findDistinctCategories();
	void countVocabulary();
	void initializeTransformedDocuments();
	void getMostFrequentWordsFromVocabulary();
	void convertCategoriesToNumbers();
	void transformDocuments();
	void findDocumentWords(unsigned int);
	void initializeWeights();
	void saveTrainedDataToFile();

	vector<string> documents;
	vector<string> categories;
	vector<string> distinctCategories;
	int* categoriesInNumbers;
	int** transformedDocuments;
	double** weights;
	FrequencyHashTable* words;
	FrequencyHashTable* documentWords;
	double vocabularySize;
	int transformedDocumentSize;
	string* mostFrequentWords;
};


LogisticRegressionTrainer::LogisticRegressionTrainer(vector<string> data, vector<string> categories)
{
	this->documents = data;
	this->categories = categories;
	this->categoriesInNumbers = new int[this->categories.size()];
	this->findDistinctCategories();
	this->countVocabulary();
	this->initializeTransformedDocuments();
	this->getMostFrequentWordsFromVocabulary();
	this->convertCategoriesToNumbers();
	this->transformDocuments();

	delete this->documentWords;
	this->weights = NULL;
}

LogisticRegressionTrainer::~LogisticRegressionTrainer()
{
	for(unsigned int i=0; i<this->documents.size(); i++)
		delete this->transformedDocuments[i];
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		delete this->weights[i];

	delete this->words;
	delete this->documentWords;
	delete this->transformedDocuments;
	delete this->weights;
	delete this->categoriesInNumbers;
	delete this->mostFrequentWords;
}

//uses the one-vs-all method for training the LogisticRegressionTrainer
void LogisticRegressionTrainer::trainClassifier()
{
	this->initializeWeights();
	int numberOfDistinctCategories = static_cast<int>(this->distinctCategories.size());
	unsigned int numberOfDocuments = this->documents.size();
	int iterations;
	bool change;
	double wTransposeTimesX;

	for(int i=0; i<numberOfDistinctCategories; i++)
	{
		iterations = 0;
		change = true;
		double cost = INFINITY;

		while(change && iterations<1000)
		{
			change = false;
			iterations++;
			for(unsigned int currentDocument=0; currentDocument<numberOfDocuments; currentDocument++)
			{
			}
		}
	}
	this->saveTrainedDataToFile();
}

//finds the distinct categories among those stored in this->categories
//and stores them into this->distinctCategories
void LogisticRegressionTrainer::findDistinctCategories()
{
	for(unsigned int i=0; i<this->categories.size(); i++)
	{
		bool sameCategoryFound = false;
		for(unsigned int j=0; j<this->distinctCategories.size(); j++)
		{
			if(this->categories[i]==this->distinctCategories[j])
			{
				sameCategoryFound = true;
				break;
			}
		}

		if(!sameCategoryFound)
			this->distinctCategories.push_back(this->categories[i]);
	}
}

void LogisticRegressionTrainer::countVocabulary()
{
	this->words = new FrequencyHashTable(400);
	string combinedDocuments = "";
	for(unsigned int i=0; i<this->documents.size(); i++)
		combinedDocuments += this->documents[i];

	int position = 0;
	string word;
	for(unsigned int i=0; i<documents.size(); i++)
	{
		if(combinedDocuments[i] == ' ' || i==combinedDocuments.size()-1)
		{
			word = combinedDocuments.substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
				this->words->insert(word);
		}
	}

	this->vocabularySize = this->words->countNonemptyEntries();
}

void LogisticRegressionTrainer::initializeTransformedDocuments()
{
	//we add 1 to the size because that will be used for calculating the b parameter of the decision line
	if(this->vocabularySize < 1000)
		this->transformedDocumentSize = static_cast<int>(this->vocabularySize) + 1;
	else
		this->transformedDocumentSize = 1001;

	this->transformedDocuments = new int*[this->documents.size()];
	for(unsigned int i=0; i<this->documents.size(); i++)
		this->transformedDocuments[i] = new int[this->transformedDocumentSize];
}

void LogisticRegressionTrainer::getMostFrequentWordsFromVocabulary()
{
	this->mostFrequentWords = this->words->getXMostFrequentWords(this->transformedDocumentSize-1);
}

void LogisticRegressionTrainer::convertCategoriesToNumbers()
{
	for(unsigned int i=0; i<this->documents.size(); i++)
	{
		for(unsigned int j=0; j<this->distinctCategories.size(); j++)
		{
			if(this->categories[i]==this->distinctCategories[j])
			{
				this->categoriesInNumbers[i] = j + 1;
				break;
			}
		}
	}
}

void LogisticRegressionTrainer::transformDocuments()
{
	for(unsigned int i=0; i<this->documents.size(); i++)
	{
		this->findDocumentWords(i);
		//this->transformedDocumentSize-1 because we have one additional position due to the vector transformation
		this->transformedDocuments[i][0] = 1;
		for(int j=1; j<this->transformedDocumentSize; j++)
		{
			this->transformedDocuments[i][j] = static_cast<int>(this->documentWords->getFrequency(this->mostFrequentWords[j-1]));
			if(this->transformedDocuments[i][j]==0)
				this->transformedDocuments[i][j] = -1;
		}

		delete this->documentWords;
		this->documentWords = NULL;
	}
}

void LogisticRegressionTrainer::findDocumentWords(unsigned int documentIndex)
{
	this->documentWords = new FrequencyHashTable(400);
	unsigned int position = 0;
	string word = "";
	for(unsigned int i=0; i<this->documents[documentIndex].size(); i++)
	{
		if(this->documents[documentIndex][i] == ' ' || i==this->documents[documentIndex].size()-1)
		{
			word = this->documents[documentIndex].substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
				this->documentWords->insert(word);
		}
	}
}

void LogisticRegressionTrainer::initializeWeights()
{
	unsigned int size = this->distinctCategories.size();
	this->weights = new double*[size];
	for(unsigned int i=0; i<size; i++)
	{
		this->weights[i] = new double[this->transformedDocumentSize];
		for(int j=0; j<this->transformedDocumentSize; j++)
			this->weights[i][j] = 0.0;
	}
}

void LogisticRegressionTrainer::saveTrainedDataToFile()
{
	ofstream writer;
	writer.open("LogisticRegressionTrainedData.txt");
	writer << "vocabularySize: " << this->vocabularySize;
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		writer << "\n";
		writer << this->distinctCategories[i] << " ";
		for(int j=0; j<this->transformedDocumentSize; j++)
			writer << this->weights[i][j] << " ";
	}
	writer.close();
	writer.clear();

	writer.open("LogisticRegressionMostFrequentWords.txt");
	writer << this->mostFrequentWords[0];
	for(int i=1; i<this->transformedDocumentSize-1; i++)
		writer << "\n" << this->mostFrequentWords[i];
	writer.close();
}

#endif