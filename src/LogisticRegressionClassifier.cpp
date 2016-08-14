#include "LogisticRegressionClassifier.hpp"

//the constructor only calls the function this->readTrainedDataFromFile()
//where all necessary initializations are made
LogisticRegressionClassifier::LogisticRegressionClassifier(string encoding)
{
	this->encoding = encoding;
	this->documentTransformer = new DocumentTransformer(encoding);
	this->readTransformedDocumentsFromFile();
}

LogisticRegressionClassifier::~LogisticRegressionClassifier()
{
	delete this->documentTransformer;
}

//this function performs the classification of the input document;
//it first extracts the words from the document and then transforms the document into a feature vector X
//with 1 in position i if word i from the dictionary occurs in the document and 0 otherwise;
//we then calculate transpose(Wj) * X, where Wj is the respective weights vector for category j;
//the document is classified in the category for which transpose(Wj) * X is maximum
string LogisticRegressionClassifier::classifyDocument(string document) const
{
	//we transform the document into a feature vector;
	//after transformation, the result will be stored in this->transformedNewDocument
	vector<transformedDocumentFeature> transformedNewDocument = this->documentTransformer->transformSingleDocument(document,true);

	//we take the number of nonzero features because that is how many entries this->tranformedNewDocument will have after transformation
	unsigned int numberOfNonzeroFeatures = transformedNewDocument.size();

	//we calculate transpose(Wj) * X for each category j
	vector<double> wTransposeTimesX;
	double currentWTransposeTimesX;
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		currentWTransposeTimesX = 0.0;
		for(unsigned int j=0; j<numberOfNonzeroFeatures; j++)
			currentWTransposeTimesX += (this->weights[i][transformedNewDocument[j].index] * transformedNewDocument[j].frequency);
		wTransposeTimesX.push_back(currentWTransposeTimesX);
	}

	//we find the category that maximizes transpose(Wj) * X and classify X in it
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

	return this->distinctCategories[maximumCategory];
}

//this function assumes that LogisticRegressionTrainedData.txt is in the following form:
//vocabularySize: 1234567890
//category1: 0 0 0 1 1 1
//category2: 1 0 0 1 0 1 etc.
void LogisticRegressionClassifier::readTransformedDocumentsFromFile()
{
	ifstream reader;
	string line;
	istringstream converter;
	double number;
	unsigned int position = 0;

	//we read the vocabulary size first,
	//which, as assumed, is stored on the first line of the file
	//in the form: vocabularySize: 218972314892
	reader.open("LogisticRegressionTrainedData.txt");
	while(!reader.eof())
	{
		vector<double> tempVector;
		getline(reader,line);
		position = 0;
		//we avoid any blank spaces at the beginning of a line
		while(line[position]!=' ')
			position++;
		//we take the category name and store it in this->distinctCategories
		this->distinctCategories.push_back(line.substr(0,position));
		position++;

		//we take each weight and append it to tempVector
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
}
