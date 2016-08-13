#ifndef NAIVE_BAYES_TRAINER_HPP
#define NAIVE_BAYES_TRAINER_HPP

#include "FrequencyHashTable.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using std::cout;
using std::string;
using std::vector;
using std::ofstream;
using std::ios;

class NaiveBayesTrainer
{
public:
	//constructors
	NaiveBayesTrainer();
	NaiveBayesTrainer(vector<string>, vector<string>);
	~NaiveBayesTrainer();

	void trainClassifier();

private:
	//training functions
	double countCategoryDistinctWords(string);
	void calculateCategoryWordsProbabilities(string, double);
	double calculateWordProbability(double, double);
	double calculateCategoryPriorProbability(string);
	void findDistinctCategories();
	void findCategoryWords(string);
	void countVocabulary();
	void saveToFile();

	vector<string> categories;
	vector<string> documents;
	vector<string> distinctCategories;
	FrequencyHashTable* words;
	double vocabularySize;
};

#endif
