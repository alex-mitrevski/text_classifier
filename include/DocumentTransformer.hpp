#ifndef DOCUMENT_TRANSFORMER_HPP
#define DOCUMENT_TRANSFORMER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "FrequencyHashTable.hpp"
#include "Structs.hpp"
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::istringstream;

class DocumentTransformer
{
public:
	DocumentTransformer(string);
	DocumentTransformer(vector<string>,vector<string>,string);
	~DocumentTransformer();

	double getVocabularySize();

	void transformDocuments();
	vector<transformedDocumentFeature> transformSingleDocument(string,bool addOneAdditional = false);

private:
	void countVocabulary();
	void getMostFrequentWordsFromVocabulary();
	void calculateInverseDocumentFrequency();
	void extractDocumentWords(unsigned int);
	void extractDocumentWords(string);
	void saveTransformedDocumentsInFile();
	void saveWordsAndFrequenciesInFile();
	void saveVocabularySizeInFile();
	void readVocabularySizeFromFile(const char*);
	void readMostFrequentWordsFromVocabulary();
	bool fileExists(const char* filename);

	vector<string> documents;
	vector<string> categories;
	vector< vector<transformedDocumentFeature> > transformedDocuments;	//used to store the feature vectors for each document
	FrequencyHashTable* words;	//used to store the most frequent words of the dictionary (or the whole dictionary, if its size is sufficiently small)
	FrequencyHashTable* documentWords;	//used to store the words of a document
	float* inverseDocumentFrequency;
	int vocabularySize;
	float numberOfDocumentWords;
	int transformedDocumentSize;
	documentFrequencyNode* mostFrequentWords;	//used to store the most frequent words of the dictionary (or the whole dictionary, if its size is sufficiently small)
	string encoding;
};

#endif
