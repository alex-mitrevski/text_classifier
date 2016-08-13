#include "LogisticRegressionTrainer.hpp"

LogisticRegressionTrainer::LogisticRegressionTrainer(vector<string> data, vector<string> categories, string encoding)
{
	this->documents = data;
	this->categories = categories;
	this->encoding = encoding;

	this->findDistinctCategories();
	//this->countVocabulary();
	this->vocabularySize = 50000;
	this->transformedDocumentSize = static_cast<int>(this->vocabularySize) + 1;
	this->readTransformedDocumentsFromFile();
	this->initializeWeights();
}

LogisticRegressionTrainer::~LogisticRegressionTrainer()
{
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		delete this->weights[i];

	delete this->weights;
	delete this->words;
}

//uses the one-vs-all method for training the LogisticRegressionTrainer
void LogisticRegressionTrainer::trainClassifier(double learningRate)
{
	int numberOfDistinctCategories = static_cast<int>(this->distinctCategories.size());
	unsigned int numberOfDocuments = this->documents.size();
	int iterations;
	bool change;
	double wTransposeTimesX;
	double* previousWeights = new double[this->transformedDocumentSize];

	double threshold = 1e-6;
	cout << "Training phase started...\n";
	for(int i=0; i<numberOfDistinctCategories; i++)
	{
		iterations = 0;
		change = true;
		double cost = INFINITY;

		while(change && iterations<100)
		{
			change = false;
			iterations++;

			for(int j=0; j<this->transformedDocumentSize; j++)
				previousWeights[j] = this->weights[i][j];

			for(int j=0; j<this->transformedDocumentSize; j++)
			{
				double sum = 0.0;
				double yi;

				for(unsigned int currentDocument=0; currentDocument<numberOfDocuments; currentDocument++)
				{
					if(this->categories[currentDocument]==this->distinctCategories[i])
					{
						int indexWhereJFound = -1;
						for(unsigned int k=0; k<this->transformedDocuments[currentDocument].size(); k++)
						{
							if(this->transformedDocuments[currentDocument][k].index==j)
							{
								indexWhereJFound = k;
								break;
							}
						}

						if(indexWhereJFound>-1)
						{
							wTransposeTimesX = 0.0;
							unsigned int numberOfNonzeroFeatures = this->transformedDocuments[currentDocument].size();
							for(unsigned int k=0; k<numberOfNonzeroFeatures; k++)
								wTransposeTimesX += (this->transformedDocuments[currentDocument][k].frequency * this->weights[i][this->transformedDocuments[currentDocument][k].index]);

							if(this->categories[currentDocument]==this->distinctCategories[i])
								yi = 1.0;
							else
								yi = 0.0;

							sum += ((this->logisticFunction(wTransposeTimesX) - yi) * this->transformedDocuments[currentDocument][indexWhereJFound].frequency);
						}
					}

					this->weights[i][j] -= (learningRate * sum);
				}
			}

			for(int j=0; j<this->transformedDocumentSize; j++)
			{
				if(abs(this->weights[i][j]-previousWeights[j])>threshold)
				{
					change = true;
					break;
				}
			}
		}
	}

	this->saveTrainedDataToFile();
	cout << "Training phase finished.\n";
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
	if(this->encoding=="ascii")
		this->words = new FrequencyHashTable(500);
	else if(this->encoding=="utf")
		this->words = new FrequencyHashTable(1000);

	string combinedDocuments = "";
	for(unsigned int i=0; i<this->documents.size(); i++)
		combinedDocuments += " " + this->documents[i];

	int position = 0;
	string word;
	for(unsigned int i=0; i<combinedDocuments.size(); i++)
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

void LogisticRegressionTrainer::readTransformedDocumentsFromFile()
{
	ifstream reader;
	string line;
	istringstream converter;
	int featureCounter, documentCounter = 0;
	float number;
	unsigned int position, index;
	
	reader.open("TransformedDocuments.txt");
	while(!reader.eof())
	{
		getline(reader,line);
		position = 0;
		featureCounter = 0;

		vector<transformedDocumentFeature> tempVector;
		//we take each weight and append it to tempVector
		for(unsigned int i=position; i<line.size(); i++)
		{
			if(line[i]==' ' || i==line.size()-1)
			{
				transformedDocumentFeature feature;
				for(unsigned int j=position; j<i; j++)
				{
					if(line[j]=='\t')
					{
						//we extract the index of the nonzero frequency
						converter.str(line.substr(position,j-position));
						converter >> index;
						feature.index = index;
						converter.clear();

						//we extract the frequency of the word at that index
						converter.str(line.substr(j+1,i-j-1));
						converter >> number;
						feature.frequency = number;
						converter.clear();
						tempVector.push_back(feature);
					}
				}
				position = i+1;
			}
		}

		transformedDocumentFeature feature(this->transformedDocumentSize-1,1.0f);
		this->transformedDocuments.push_back(tempVector);
		converter.clear();
	}
	reader.close();
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

double LogisticRegressionTrainer::logisticFunction(double value)
{
	return (1.0 / (1.0 + (1.0/exp(value))));
}

//LogisticRegressionTrainedData.txt will have the form:
//category1 1.0 2.0 3.0 and so on
//category2 1.0 2.0 3.0 and so on
//... one category per line
void LogisticRegressionTrainer::saveTrainedDataToFile()
{
	ofstream writer;
	writer.open("LogisticRegressionTrainedData.txt");
	writer << this->distinctCategories[0] << " " << this->weights[0][0];
	for(int j=1; j<this->transformedDocumentSize; j++)
		writer << " " << this->weights[0][j];
	for(unsigned int i=1; i<this->distinctCategories.size(); i++)
	{
		writer << "\n";
		writer << this->distinctCategories[i] << " " << this->weights[i][0];
		for(int j=1; j<this->transformedDocumentSize; j++)
			writer << " " << this->weights[i][j];
	}
	writer.close();
}
