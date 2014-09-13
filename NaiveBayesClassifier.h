#ifndef NAIVE_BAYES_CLASSIFIER_H
#define NAIVE_BAYES_CLASSIFIER_H

#include "ProbabilityHashTable.h"
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


NaiveBayesClassifier::NaiveBayesClassifier()
{
	this->categoryPrior = 0.0;
	this->numberOfCategoryWords = 0.0;
	this->readTrainedData();
	this->words = new ProbabilityHashTable*[this->distinctCategories.size()];
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		this->words[i] = new ProbabilityHashTable(500);
		this->readCategoryData(this->distinctCategories[i], i);
	}
}

NaiveBayesClassifier::~NaiveBayesClassifier()
{
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		delete this->words[i];
	delete this->words;
}

//this function assumes that the document is in the "correct" format,
//i.e. that it is in lowercase and that it has no punctuation
string NaiveBayesClassifier::classifyDocument(string document)
{
	vector<double> categoryProbabilities;
	string word;
	unsigned int position = 0;

	for(unsigned int i=0; i<distinctCategories.size(); i++)
		categoryProbabilities.push_back(0.0);

	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		categoryProbabilities[i] += log(this->categoryPrior);

		position = 0;
		for(unsigned int j=0; j<document.size(); j++)
		{
			if(document[j] == ' ' || j==document.size()-1)
			{
				word = document.substr(position,j-position);
				position = j + 1;
				if(word.length()>2)
				{
					double wordProbability = this->words[i]->getProbability(word);
					if(wordProbability<1e-10)
						wordProbability = 1.0/(this->vocabularySize + this->numberOfCategoryWords);
					categoryProbabilities[i] += log(wordProbability);
				}
			}
		}
	}

	return this->findMaximumProbabilityCategory(categoryProbabilities);
}

void NaiveBayesClassifier::readTrainedData()
{
	ifstream reader;
	string line, data;

	reader.open("trainedData.txt");

	//takes the categories from the file;
	//assumes that the categories are stored in line 1 in the form:
	//categories: category1, category2...
	getline(reader,line);
	unsigned int position = 0;
	while(line[position]!=' ')
		position++;
	position += 1;
	for(unsigned int i=position+1; i<line.size(); i++)
	{
		if(line[i]==' ')
		{
			data = line.substr(position,i-position);
			this->distinctCategories.push_back(data);
			position = i + 1;
		}
	}

	//takes the vocabulary size from the file;
	//assumes that this is stored in line 2 of the file in the form:
	//vocabularySize: 1234567890
	getline(reader,line);
	position = 0;
	while(line[position]!=' ')
		position++;
	position++;
	stringstream converter;
	converter << line.substr(position,line.size()-position);
	converter >> this->vocabularySize;

	reader.close();
}

void NaiveBayesClassifier::readCategoryData(string category, unsigned int categoryIndex)
{
	ifstream reader;
	istringstream converter;
	string line, word;
	double probability = 0.0;

	reader.open(category+".txt");
	
	//takes the category prior probability;
	//asssumes that the prior is in line 1 in the form:
	//prior: 0.1234567890
	getline(reader,line);
	unsigned int position = 0;
	while(line[position]!=' ')
		position++;
	position++;
	converter.str(line.substr(position,line.size()-position));
	converter >> this->categoryPrior;
	converter.clear();

	//takes the number of words in the category;
	//asssumes that the number is in line 2 in the form:
	//numberOfWords: 1234567890
	getline(reader,line);
	position = 0;
	while(line[position]!=' ')
		position++;
	position++;
	converter.str(line.substr(position,line.size()-position));
	converter >> this->numberOfCategoryWords;
	converter.clear();

	while(!reader.eof())
	{
		getline(reader,line);
		position = 0;
		while(line[position]!= ' ')
			position++;
		word = line.substr(0,position);
		position++;
		converter.str(line.substr(position,line.size()-position));
		converter >> probability;
		converter.clear();

		this->words[categoryIndex]->insert(word,probability);
	}

	reader.close();
}

string NaiveBayesClassifier::findMaximumProbabilityCategory(vector<double> categoryProbabilities)
{
	double maximumProbability = categoryProbabilities[0];
	string finalCategory = this->distinctCategories[0];

	for(unsigned int i=1; i<categoryProbabilities.size(); i++)
	{
		if(maximumProbability < categoryProbabilities[i])
		{
			maximumProbability = categoryProbabilities[i];
			finalCategory = this->distinctCategories[i];
		}
	}

	return finalCategory;
}

#endif