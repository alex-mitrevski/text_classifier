#include "PerceptronClassifier.hpp"

PerceptronClassifier::PerceptronClassifier()
{
	this->readTrainedDataFromFile();
}

PerceptronClassifier::~PerceptronClassifier()
{
	delete this->mostFrequentWords;
	delete this->documentWords;
}

string PerceptronClassifier::classifyDocument(string document)
{
	this->extractDocumentWords(document);
	int* documentInNumbers = new int[this->transformedDocumentSize];

	//this->transformedDocumentSize-1 because we have one additional position due to the vector transformation
	for(int i=0; i<this->transformedDocumentSize-1; i++)
	{
		if(this->documentWords->exists(this->mostFrequentWords[i]))
			documentInNumbers[i] = 1;
		else
			documentInNumbers[i] = 0;
	}
	documentInNumbers[this->transformedDocumentSize-1] = 1;

	vector<double> wTransposeTimesX;
	double currentWTransposeTimesX;
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		currentWTransposeTimesX = 0.0;
		for(int j=0; j<this->transformedDocumentSize; j++)
			currentWTransposeTimesX += this->weights[i][j] * documentInNumbers[j];
		wTransposeTimesX.push_back(currentWTransposeTimesX);
	}

	unsigned int maximumCategory = 0;
	double maximum = wTransposeTimesX[0];
	for(unsigned int i=1; i<wTransposeTimesX.size(); i++)
	{
		if(maximum < wTransposeTimesX[i])
		{
			maximum = wTransposeTimesX[i];
			maximumCategory = i;
		}
	}

	delete this->documentWords;
	return this->distinctCategories[maximumCategory];
}

//this function assumes that PerceptronTrainedData.txt is in the following form:
//vocabularySize: 1234567890
//category1: 0 0 0 1 1 1
//category2: 1 0 0 1 0 1 etc.
//
//and that PerceptronMostFrequentWords.txt is in the following form
//word1
//word2 etc.
void PerceptronClassifier::readTrainedDataFromFile()
{
	ifstream reader;
	string line;
	istringstream converter;
	double number;
	unsigned int position = 0;

	reader.open("PerceptronTrainedData.txt");
	getline(reader,line);
	while(line[position]!=' ')
		position++;
	position++;
	converter.str(line.substr(position,line.size()-position));
	converter >> this->vocabularySize;
	converter.clear();

	//we add 1 to the size because that will be used for calculating the b parameter of the decision line
	if(this->vocabularySize < 1000)
		this->transformedDocumentSize = static_cast<int>(this->vocabularySize) + 1;
	else
		this->transformedDocumentSize = 1001;
	this->mostFrequentWords = new string[this->transformedDocumentSize-1];

	while(!reader.eof())
	{
		vector<double> tempVector;
		getline(reader,line);
		position = 0;
		while(line[position]!=' ')
			position++;
		this->distinctCategories.push_back(line.substr(0,position));
		position++;

		for(unsigned int i=position; i<line.size(); i++)
		{
			if(line[i]==' ' || i==line.size()-1)
			{
				converter.str(line.substr(position,i-position));
				converter >> number;
				tempVector.push_back(number);
				position = i+1;
			}
		}

		this->weights.push_back(tempVector);
		converter.clear();
	}
	reader.close();
	reader.clear();

	int wordCounter = 0;
	reader.open("PerceptronMostFrequentWords.txt");
	while(!reader.eof())
	{
		getline(reader,line);
		this->mostFrequentWords[wordCounter] = line;
		wordCounter++;
	}
	reader.close();
}

void PerceptronClassifier::extractDocumentWords(string document)
{
	int position = 0;
	string word;
	this->documentWords = new WordHashTable(500);
	for(unsigned int i=0; i<document.size(); i++)
	{
		if(document[i] == ' ' || i==document.size()-1)
		{
			word = document.substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
				this->documentWords->insert(word);
		}
	}
}
