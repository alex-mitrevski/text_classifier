#include "KNNClassifier.hpp"

KNNClassifier::KNNClassifier(vector<string> documents, vector<string> categories, int kParameter)
{
	this->documents = documents;
	this->categories = categories;
	this->kParameter = kParameter;
	this->documentWords = new WordHashTable*[this->documents.size()];
	for(unsigned int i=0; i<this->documents.size(); i++)
	{
		this->documentWords[i] = new WordHashTable(500);

		unsigned int position = 0;
		string word = "";
		for(unsigned int j=0; j<this->documents[i].size(); j++)
		{
			if(this->documents[i][j] == ' ' || j==this->documents[i].size()-1)
			{
				word = this->documents[i].substr(position,j-position);
				position = j + 1;
				if(word.length()>2)
					this->documentWords[i]->insert(word);
			}
		}
	}
}

string KNNClassifier::classifyDocument(string document)
{
	vector<string> newDocumentWords;
	vector<int> distances;
	vector<unsigned int> indices;
	string word;
	unsigned int position = 0;

	for(unsigned int i=0; i<document.size(); i++)
	{
		if(document[i] == ' ' || i==document.size()-1)
		{
			word = document.substr(position,i-position);
			position = i + 1;
			if(word.length()>2)
				newDocumentWords.push_back(word);
		}
	}

	vector<unsigned int> randomIndices;
	srand(static_cast<unsigned int>(time(NULL)));
	int counter = 0;
	while(counter<300)
	{
		unsigned int randomIndex = rand()%this->documents.size();
		bool alreadyExists = false;
		for(unsigned int j=0; j<randomIndices.size(); j++)
		{
			if(randomIndices[j]==randomIndex)
			{
				alreadyExists = true;
				break;
			}
		}
		if(!alreadyExists)
		{
			randomIndices.push_back(rand()%this->documents.size());
			counter++;
		}
	}

	for(unsigned int i=0; i<randomIndices.size(); i++)
	{
		indices.push_back(randomIndices[i]);
		distances.push_back(0);
		for(unsigned int j=0; j<newDocumentWords.size(); j++)
		{
			if(!this->documentWords[randomIndices[i]]->exists(newDocumentWords[j]))
				distances[i]++;
		}
	}

	quickSort(indices,distances,0,distances.size());
	FrequencyList* list = new FrequencyList();
	for(unsigned int i=0; i<this->kParameter; i++)
		list->insert(this->categories[indices[i]]);
	string maximumFrequency = list->findMaxFrequency();
	list = NULL;

	return maximumFrequency;
}

//string KNNClassifier::classifyDocument(string document)
//{
//	string word;
//	unsigned int position = 0;
//	vector<int> distances;
//	vector<unsigned int> indices;
//	WordHashTable* documentWords = new WordHashTable(500);
//
//	//extracts each word from the document that should be classified and stores it into a hash table
//	for(unsigned int i=0; i<document.size(); i++)
//	{
//		if(document[i] == ' ' || i==document.size()-1)
//		{
//			word = document.substr(position,i-position);
//			position = i + 1;
//			if(word.length()>2)
//				documentWords->insert(word);
//		}
//	}
//
//	for(unsigned int i=0; i<this->documents.size(); i++)
//	{
//		position = 0;
//		distances.push_back(0);
//		indices.push_back(i);
//		for(unsigned int j=0; j<this->documents[i].size(); j++)
//		{
//			if(this->documents[i][j] == ' ' || j==this->documents[i].size()-1)
//			{
//				word = this->documents[i].substr(position,j-position);
//				position = j + 1;
//				if(word.length()>2)
//				{
//					if(!documentWords->exists(word))
//						distances[i]++;
//				}
//			}
//		}
//	}
//
//	quickSort(indices,distances,0,distances.size());
//	FrequencyList* list = new FrequencyList();
//	for(unsigned int i=0; i<this->kParameter; i++)
//		list->insert(this->categories[indices[i]]);
//	string maximumFrequency = list->findMaxFrequency();
//	list = NULL;
//
//	return maximumFrequency;
//}

void KNNClassifier::quickSort(vector<unsigned int>& indices, vector<int>& numbers, unsigned int start, unsigned int end)
{
	unsigned int pivot;
	if(start<end && end-start!=1)
	{
		pivot = partition(indices,numbers,start,end);
		quickSort(indices,numbers,start,pivot);
		quickSort(indices,numbers,pivot+1,end);
	}
}

unsigned int KNNClassifier::partition(vector<unsigned int>& indices, vector<int>& numbers, unsigned int start, unsigned int end)
{
	unsigned int i = start + 1;
	int pivot = numbers[start];

	for(unsigned int j=start+1; j<end; j++)
	{
		if(numbers[j]<pivot)
		{
			swap(numbers[i],numbers[j]);
			swap(indices[i],indices[j]);
			i++;
		}
	}

	swap(numbers[start],numbers[i-1]);
	swap(indices[start],indices[i-1]);
	return i-1;
}

void KNNClassifier::swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

void KNNClassifier::swap(unsigned int &a, unsigned int &b)
{
	int temp = a;
	a = b;
	b = temp;
}
