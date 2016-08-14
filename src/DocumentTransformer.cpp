#include "DocumentTransformer.hpp"

DocumentTransformer::DocumentTransformer(vector<string> documents, vector<string> categories, string encoding)
{
	this->documents = documents;
	this->categories = categories;
	this->encoding = encoding;
	if(!this->fileExists("VocabularySize.txt"))
	{
		this->countVocabulary();
		this->transformedDocumentSize = static_cast<int>(std::min(this->vocabularySize, 50000));
		this->getMostFrequentWordsFromVocabulary();
		this->calculateInverseDocumentFrequency();
	}
	else
	{
		this->readVocabularySizeFromFile("VocabularySize.txt");
		this->transformedDocumentSize = static_cast<int>(std::min(this->vocabularySize, 50000));
		this->readMostFrequentWordsFromVocabulary();
	}
}

DocumentTransformer::DocumentTransformer(string encoding)
{
	this->encoding = encoding;
	this->readVocabularySizeFromFile("VocabularySize.txt");
	this->transformedDocumentSize = static_cast<int>(this->vocabularySize);
	this->readMostFrequentWordsFromVocabulary();
	this->documentWords = NULL;
}

DocumentTransformer::~DocumentTransformer()
{
	delete this->words;
	delete this->documentWords;
	delete this->mostFrequentWords;
	delete this->inverseDocumentFrequency;
}

double DocumentTransformer::getVocabularySize() const
{
	return this->vocabularySize;
}

//this function finds the size of the vocabulary given all training documents;
//the vocabulary is found such that a metadocument of all training documents is constructed first;
//then, each word of the metadocument is extracted and stored in a hash table;
//finally, the distinct entries in the hash table are counted, which gives us the size of the vocabulary;
//note that this function creates a new pointer of the FrequencyHashTable class and assings it to this->words;
//since the words of the metadocument are not needed when the function end, this function performs explicit memory cleanup on this->words
//after finishing its operations;
//this is the same comment as in the function NaiveBayesTrainer::countVocabulary
void DocumentTransformer::countVocabulary()
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
	this->saveVocabularySizeInFile();
}

//finds the x most frequent words in the vocabulary;
//x, the number of words taken, is stored in this->transformedDocumentSize
void DocumentTransformer::getMostFrequentWordsFromVocabulary()
{
	this->mostFrequentWords = new documentFrequencyNode[this->transformedDocumentSize];
	string* frequentWords = this->words->getXMostFrequentWords(this->transformedDocumentSize);

	for(int i=0; i<this->transformedDocumentSize; i++)
		this->mostFrequentWords[i].data = frequentWords[i];

	delete this->words;
}

void DocumentTransformer::calculateInverseDocumentFrequency()
{
	float frequency;
	unsigned int numberOfDocuments = this->documents.size();

	for(unsigned int i=0; i<numberOfDocuments; i++)
	{
		this->numberOfDocumentWords = 0.0f;
		this->extractDocumentWords(i);

		for(int j=0; j<this->transformedDocumentSize; j++)
		{
			//we calculate the term frequency in the document
			frequency = static_cast<float>(this->documentWords->getFrequency(this->mostFrequentWords[j].data));
			if(frequency>0.0000f)
				this->mostFrequentWords[j].frequency++;
		}

		delete this->documentWords;
		this->documentWords = NULL;
	}

	//we calculate the inverse document frequency of the words
	this->inverseDocumentFrequency = new float[this->transformedDocumentSize];
	for(int i=0; i<this->transformedDocumentSize; i++)
	{
		if(this->mostFrequentWords[i].frequency>0.0000000000000f)
			this->inverseDocumentFrequency[i] = log(numberOfDocuments / this->mostFrequentWords[i].frequency);
		else
			this->inverseDocumentFrequency[i] = 0.0000f;
	}
}

//transforms the documents into feature vectors Xi, such that the value of a dimension j
//is given by the frequency of the word this->mostFrequentWords(j) in document i
void DocumentTransformer::transformDocuments()
{
	float frequency;
	transformedDocumentFeature feature;
	vector<transformedDocumentFeature> documentFeatures;

	ofstream writer;
	writer.open("TransformedDocuments.txt");

	unsigned int numberOfDocuments = this->documents.size();
	for(unsigned int i=0; i<numberOfDocuments; i++)
	{
		this->extractDocumentWords(i);
		for(int j=0; j<this->transformedDocumentSize; j++)
		{
			//we calculate the term frequency in the document
			frequency = static_cast<float>(this->documentWords->getFrequency(this->mostFrequentWords[j].data));
			if(frequency>0.0000f)
			{
				feature.frequency = frequency * this->inverseDocumentFrequency[j];
				feature.index = j;
				documentFeatures.push_back(feature);
			}

			//this->transformedDocuments[i][j] = static_cast<float>(this->documentWords->getFrequency(this->mostFrequentWords[j].data));// / this->numberOfDocumentWords);
			//if(this->transformedDocuments[i][j]>0.0000f)
			//	this->mostFrequentWords[j].frequency++;
		}

		//we normalize the document vector
		float norm = 0.0f;
		for(unsigned int k=0; k<documentFeatures.size(); k++)
			norm += documentFeatures[k].frequency;
		for(unsigned int k=0; k<documentFeatures.size(); k++)
			documentFeatures[k].frequency /= norm;

		//we make sure that the file is well-structured, with no unwanted line breaks and spaces;
		//that is why we write the first line out of the main writing loop
		if(documentFeatures.size()>0)
			writer << documentFeatures[0].index << "\t" << documentFeatures[0].frequency;
		for(unsigned int j=1; j<documentFeatures.size(); j++)
			writer << " " << documentFeatures[j].index << "\t" << documentFeatures[j].frequency;
		writer << "\n";

		//this->transformedDocuments.push_back(documentFeatures);
		documentFeatures.clear();

		delete this->documentWords;
		this->documentWords = NULL;
	}
	writer.close();

//	this->saveTransformedDocumentsInFile();
	this->saveWordsAndFrequenciesInFile();
}

vector<transformedDocumentFeature> DocumentTransformer::transformSingleDocument(string document, bool addOneAdditional)
{
	vector<transformedDocumentFeature> transformedNewDocument;
	this->extractDocumentWords(document);

	//we calculate the tf-idf index of each word
	for(int i=0; i<this->transformedDocumentSize; i++)
	{
		transformedDocumentFeature feature;
		feature.frequency = static_cast<float>(this->documentWords->getFrequency(this->mostFrequentWords[i].data));
		if(feature.frequency>0.0000f)
		{
			feature.index = i;
			feature.frequency *= this->mostFrequentWords[i].frequency;
			transformedNewDocument.push_back(feature);
		}
	}
	if(addOneAdditional)
	{
		transformedDocumentFeature feature(this->transformedDocumentSize-1,1.0f);
		transformedNewDocument.push_back(feature);
	}

	delete this->documentWords;
	this->documentWords = NULL;

	return transformedNewDocument;
}

//extracts the words of the training document at position 'documentIndex'
//and stores them into this->documentWords
void DocumentTransformer::extractDocumentWords(unsigned int documentIndex)
{
	if(this->encoding=="ascii")
		this->documentWords = new FrequencyHashTable(500);
	else if(this->encoding=="utf")
		this->documentWords = new FrequencyHashTable(1000);

	unsigned int position = 0;
	string word = "";
	for(unsigned int i=0; i<this->documents[documentIndex].size(); i++)
	{
		if(this->documents[documentIndex][i] == ' ' || i==this->documents[documentIndex].size()-1)
		{
			word = this->documents[documentIndex].substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
			{
				this->documentWords->insert(word);
				this->numberOfDocumentWords++;
			}
		}
	}
}

void DocumentTransformer::extractDocumentWords(string document)
{
	int position = 0;
	string word;
	if(this->encoding=="ascii")
		this->documentWords = new FrequencyHashTable(500);
	else if(this->encoding=="utf")
		this->documentWords = new FrequencyHashTable(1000);

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

void DocumentTransformer::saveTransformedDocumentsInFile() const
{
	ofstream writer;
	writer.open("TransformedDocuments.txt");

	//we make sure that the file is well-structured, with no unwanted line breaks and spaces;
	//that is why we write the first line out of the main writing loop
	if(this->transformedDocuments[0].size()>0)
		writer << this->transformedDocuments[0][0].index << "\t" << this->transformedDocuments[0][0].frequency;
	for(unsigned int j=1; j<this->transformedDocuments[0].size(); j++)
		writer << " " << this->transformedDocuments[0][j].index << "\t" << this->transformedDocuments[0][j].frequency;

	unsigned int numberOfDocuments = this->documents.size();
	unsigned int numberOfFeatures;
	for(unsigned int i=1; i<numberOfDocuments; i++)
	{
		writer << "\n";
		if(this->transformedDocuments[i].size()>0)
			writer << this->transformedDocuments[i][0].index << "\t" << this->transformedDocuments[i][0].frequency;
		numberOfFeatures = this->transformedDocuments[i].size();
		for(unsigned int j=1; j<numberOfFeatures; j++)
			writer << " " << this->transformedDocuments[i][j].index << "\t" << this->transformedDocuments[i][j].frequency;
	}
	writer.close();
}

void DocumentTransformer::saveWordsAndFrequenciesInFile() const
{
	//unsigned int numberOfDocuments = this->documents.size();
	////we calculate the inverse document frequency of the words
	//float* inverseDocumentFrequency = new float[this->transformedDocumentSize];
	//for(int i=0; i<this->transformedDocumentSize; i++)
	//{
	//	if(this->mostFrequentWords[i].frequency > 0.0001f)
	//		inverseDocumentFrequency[i] = log(numberOfDocuments / this->mostFrequentWords[i].frequency);
	//	else
	//		inverseDocumentFrequency[i] = 0.0000f;
	//}

	ofstream writer;
	writer.open("MostFrequentWords.txt");

	writer << this->mostFrequentWords[0].data << " " << this->inverseDocumentFrequency[0];
	for(int i=1; i<this->transformedDocumentSize; i++)
		writer << "\n" << this->mostFrequentWords[i].data << " " << this->inverseDocumentFrequency[i];

	writer.close();
}

void DocumentTransformer::saveVocabularySizeInFile() const
{
	ofstream writer;
	writer.open("VocabularySize.txt");
	writer << "vocabularySize: " << this->vocabularySize;
	writer.close();
}

void DocumentTransformer::readVocabularySizeFromFile(const char* filename)
{
	ifstream reader;
	string line;
	istringstream converter;
	unsigned int position = 0;

	reader.open(filename);

	//we read the vocabulary size
	//which, as assumed, is stored on the first line of the file
	//in the form: vocabularySize: 218972314892
	getline(reader,line);
	while(line[position]!=' ')
		position++;
	position++;
	converter.str(line.substr(position,line.size()-position));
	converter >> this->vocabularySize;
	converter.clear();

	reader.close();
}

void DocumentTransformer::readMostFrequentWordsFromVocabulary()
{
	this->mostFrequentWords = new documentFrequencyNode[this->transformedDocumentSize];

	ifstream reader;
	istringstream converter;
	string line;
	unsigned int position;
	float number;

	reader.open("MostFrequentWords.txt");
	int wordCounter = 0;
	while(wordCounter<this->transformedDocumentSize)//(!reader.eof())
	{
		getline(reader,line);

		position = 0;
		while(line[position]!=' ')
			position++;

		this->mostFrequentWords[wordCounter].data = line.substr(0,position);
		converter.str(line.substr(position+1,line.size()-position-1));
		converter >> number;
		converter.clear();
		this->mostFrequentWords[wordCounter].frequency = number;

		wordCounter++;
	}
	reader.close();
}

bool DocumentTransformer::fileExists(const char* filename) const
{
	ifstream file(filename);
	bool exists = file.good();
	file.close();
	return exists;
}
