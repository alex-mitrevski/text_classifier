#include "ProbabilisticNeuralNetwork.hpp"

ProbabilisticNeuralNetwork::ProbabilisticNeuralNetwork(unsigned int numberOfDocuments, vector<string> categories, string encoding, float sigmaSquared)
{
	this->numberOfDocuments = numberOfDocuments;
	this->categories = categories;
	this->encoding = encoding;
	this->sigmaSquared = sigmaSquared;
	this->documentTransformer = new DocumentTransformer(encoding);
	this->transformedDocumentSize = static_cast<unsigned int>(this->documentTransformer->getVocabularySize());
	this->readTransformedDocumentsFromFile();
	this->findDistinctCategories();
	this->convertCategoriesToNumbers();
	this->countDocumentsInEachCategory();
}

ProbabilisticNeuralNetwork::~ProbabilisticNeuralNetwork()
{
	delete this->categoriesInNumbers;
	delete this->documentsInEachCategory;
}

string ProbabilisticNeuralNetwork::classifyDocument(string document)
{
	unsigned int numberOfDistinctCategories = this->distinctCategories.size();
	vector<float> categorySum;

	for(unsigned int i=0; i<numberOfDistinctCategories; i++)
		categorySum.push_back(0.0f);

	float distance;
	this->transformedNewDocument = this->documentTransformer->transformSingleDocument(document);

	for(unsigned int i=0; i<this->numberOfDocuments; i++)
	{
		distance = 0.0f;
		unsigned int numberOfNonzeroFrequencies = this->transformedDocuments[i].size();
		unsigned int currentTrainingDocumentIndex = 0;
		unsigned int currentNewDocumentIndex = 0;

		if(this->transformedDocuments[i].size()!=0)
		{
			for(unsigned int j=0; j<this->transformedDocumentSize; j++)
			{
				if(j==this->transformedDocuments[i][currentTrainingDocumentIndex].index && j==this->transformedNewDocument[currentNewDocumentIndex].index)
				{
					distance += ((this->transformedDocuments[i][currentTrainingDocumentIndex].frequency - this->transformedNewDocument[currentNewDocumentIndex].frequency) * (this->transformedDocuments[i][currentTrainingDocumentIndex].frequency - this->transformedNewDocument[currentNewDocumentIndex].frequency));
					if(currentTrainingDocumentIndex<this->transformedDocuments[i].size()-1)
						currentTrainingDocumentIndex++;
					if(currentNewDocumentIndex<this->transformedNewDocument.size()-1)
						currentNewDocumentIndex++;
				}
				else if(j==this->transformedDocuments[i][currentTrainingDocumentIndex].index)
				{
					distance += (this->transformedDocuments[i][currentTrainingDocumentIndex].frequency * this->transformedDocuments[i][currentTrainingDocumentIndex].frequency);
					if(currentTrainingDocumentIndex<this->transformedDocuments[i].size()-1)
						currentTrainingDocumentIndex++;
				}
				else if(j==this->transformedNewDocument[currentNewDocumentIndex].index)
				{
					distance += (this->transformedNewDocument[currentNewDocumentIndex].frequency * this->transformedNewDocument[currentNewDocumentIndex].frequency);
					if(currentNewDocumentIndex<this->transformedNewDocument.size()-1)
						currentNewDocumentIndex++;
				}
			}

			distance /= (2.0f * this->sigmaSquared);
			distance = 1.0f/exp(distance/1000);
			categorySum[this->categoriesInNumbers[i]-1] += distance; //we increment the sum in the appropriate category
		}
	}

	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		categorySum[i] /= this->documentsInEachCategory[i];

	unsigned int maximumCategory = 0;
	float maximum = categorySum[0];
	for(unsigned int i=1; i<this->distinctCategories.size(); i++)
	{
		if(maximum<categorySum[i])
		{
			maximum = categorySum[i];
			maximumCategory = i;
		}
	}

	this->transformedNewDocument.clear();

	return this->distinctCategories[maximumCategory];
}

//finds the distinct categories among those stored in this->categories
//and stores them into this->distinctCategories
void ProbabilisticNeuralNetwork::findDistinctCategories()
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

//converts each category to a number and stores the number in this->categoriesInNumbers;
//the numbering of categories is one-based
void ProbabilisticNeuralNetwork::convertCategoriesToNumbers()
{
	this->categoriesInNumbers = new int[this->categories.size()];
	for(unsigned int i=0; i<this->numberOfDocuments; i++)
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

void ProbabilisticNeuralNetwork::countDocumentsInEachCategory()
{
	this->documentsInEachCategory = new int[this->distinctCategories.size()];
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		this->documentsInEachCategory[i] = 0;

	for(unsigned int i=0; i<this->categories.size(); i++)
		this->documentsInEachCategory[this->categoriesInNumbers[i]-1]++;
}

void ProbabilisticNeuralNetwork::readTransformedDocumentsFromFile()
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

		this->transformedDocuments.push_back(tempVector);
		converter.clear();
	}
	reader.close();
}
