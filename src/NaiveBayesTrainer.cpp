#include "NaiveBayesTrainer.hpp"

NaiveBayesTrainer::NaiveBayesTrainer() { }

NaiveBayesTrainer::NaiveBayesTrainer(vector<string> categories, vector<string> documents)
{
	this->categories = categories;
	this->documents = documents;
	this->findDistinctCategories();
	this->words = NULL;
	this->countVocabulary();
}

NaiveBayesTrainer::~NaiveBayesTrainer()
{
	delete this->words;
}

void NaiveBayesTrainer::trainClassifier()
{
	cout << "Training phase started...\n";
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
	{
		double categoryPrior = this->calculateCategoryPriorProbability(this->distinctCategories[i]);
		this->findCategoryWords(distinctCategories[i]);
		double categoryDistinctWords = this->countCategoryDistinctWords(this->distinctCategories[i]);
		this->calculateCategoryWordsProbabilities(this->distinctCategories[i], categoryDistinctWords);

		delete this->words;
		this->words = NULL;
	}

	this->saveToFile();
	cout << "Training phase finished.\n";
}

void NaiveBayesTrainer::countVocabulary()
{
	this->words = new FrequencyHashTable(500);
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
	delete this->words;
	this->words = NULL;
}

//finds the distinct categories among those stored in this->categories
//and stores them into this->distinctCategories
void NaiveBayesTrainer::findDistinctCategories()
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

void NaiveBayesTrainer::findCategoryWords(string category)
{
	//combines all the documents in the category into one big document
	string categoryDocuments = "";
	for(unsigned int i=0; i<this->categories.size(); i++)
	{
		if(this->categories[i] == category)
			categoryDocuments += documents[i];
	}

	this->words = new FrequencyHashTable(500);
	int position = 0;
	string word;
	for(unsigned int i=0; i<categoryDocuments.size(); i++)
	{
		if(categoryDocuments[i] == ' ' || i==categoryDocuments.size()-1)
		{
			word = categoryDocuments.substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
				this->words->insert(word);
		}
	}
}

double NaiveBayesTrainer::countCategoryDistinctWords(string category) const
{
	double distinctWords = this->words->countNonemptyEntries();
	return distinctWords;
}

void NaiveBayesTrainer::calculateCategoryWordsProbabilities(string category, double categoryDistinctWords) const
{
	ofstream writer;
	node* traversalNode;

    string categoryFileName = category + ".txt";
	writer.open(categoryFileName.c_str(), ios::app);
	writer << "\nnumberOfWords: " << categoryDistinctWords;

	for(int i=0; i<words->getNumberOfAllowedEntries(); i++)
	{
		FrequencyList* wordsAtPosition = this->words->getEntries()[i];
		traversalNode = wordsAtPosition->getHead();

		while(traversalNode!=NULL)
		{
			double wordFrequency = traversalNode->frequency;
			string word = traversalNode->data;
			double wordProbability = this->calculateWordProbability(wordFrequency, categoryDistinctWords);
			writer << "\n" << word << " " << wordProbability;
			traversalNode = traversalNode->next;
		}
	}

	writer.close();
	delete traversalNode;
}

double NaiveBayesTrainer::calculateWordProbability(double numberOfOccurences, double totalWords) const
{
	return (numberOfOccurences + 1.0) / (totalWords + this->vocabularySize);
}

double NaiveBayesTrainer::calculateCategoryPriorProbability(string category) const
{
	double prior = 0.0;
	unsigned int numberOfCategoryEntries = 0;

	for(unsigned int i=0; i<this->categories.size(); i++)
	{
		if(this->categories[i]==category)
			numberOfCategoryEntries++;
	}

	prior = numberOfCategoryEntries / (this->categories.size() * 1.0);
	ofstream writer;
    string categoryFileName = category + ".txt";
	writer.open(categoryFileName.c_str());
	writer << "prior: " << prior;
	writer.close();

	return prior;
}

void NaiveBayesTrainer::saveToFile() const
{
	ofstream writer;

	writer.open("trainedData.txt");
	writer << "categories: ";
	for(unsigned int i=0; i<this->distinctCategories.size(); i++)
		writer << distinctCategories[i] << " ";
	writer << "\nvocabularySize: " << this->vocabularySize;
	writer.close();
}
