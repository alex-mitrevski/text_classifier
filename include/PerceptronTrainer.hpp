#ifndef PERCEPTRON_TRAINER_HPP
#define PERCEPTRON_TRAINER_HPP

#include "FrequencyHashTable.hpp"
#include "WordHashTable.hpp"
#include <vector>
#include <string>
#include <fstream>
using std::vector;
using std::string;
using std::ofstream;

class PerceptronTrainer
{
public:
	PerceptronTrainer(vector<string>, vector<string>);
	~PerceptronTrainer();

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
	void normalizeCategories(int);
	void saveTrainedDataToFile() const;

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

#endif
