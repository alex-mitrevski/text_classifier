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
	NaiveBayesTrainer();
	NaiveBayesTrainer(vector<string>, vector<string>);
	~NaiveBayesTrainer();

	void trainClassifier();

private:
	double countCategoryDistinctWords(string) const;
	void calculateCategoryWordsProbabilities(string, double) const;
	double calculateWordProbability(double, double) const;
	double calculateCategoryPriorProbability(string) const;
	void findDistinctCategories();
	void findCategoryWords(string);
	void countVocabulary();
	void saveToFile() const;

	vector<string> categories;
	vector<string> documents;
	vector<string> distinctCategories;
	FrequencyHashTable* words;
	double vocabularySize;
};

#endif
