#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <string>
using std::string;

struct wordNode
{
	wordNode(string data) { this->data = data; this->next = NULL; }

	string data;
	wordNode* next;
};

class WordList
{
public:
	//constructor
	WordList();

	//accessor function
	wordNode* getHead();

	//data access functions
	void insert(string);
	void remove(string);
	bool exists(string data);
	int size();
	bool isEmpty();

private:
	wordNode* head;
	int listSize;
};


WordList::WordList()
{
	this->head = NULL;
	this->listSize = 0;
}

wordNode* WordList::getHead()
{
	return this->head;
}

//the insert function doesn't allow duplicates in the list
void WordList::insert(string data)
{
	if(this->head==NULL)
	{
		this->head = new wordNode(data);
		this->listSize++;
	}
	else
	{
		wordNode* traversalNode = this->head;
		bool sameDataFound = false;

		//traversalNode->data==data because the data can match the data in the head, which we will miss without this condition;
		//in other words, if the data matches the field in the head, then we go inside the loop and go out immediately;
		//otherwise, the loop continues until a match is found or the end of the list is reached
		while(traversalNode->next != NULL || traversalNode->data==data)
		{
			if(traversalNode->data == data)
			{
				traversalNode = NULL;
				sameDataFound = true;
				break;
			}
			traversalNode = traversalNode->next;
		}

		if(!sameDataFound)
		{
			traversalNode->next = new wordNode(data);
			this->listSize++;
		}
	}
}

void WordList::remove(string data)
{
	if(this->head!=NULL)
	{
		wordNode* traversalNode = this->head;
		wordNode* previousNode = this->head;
		while(traversalNode!=NULL && traversalNode->data!=data)
		{
			previousNode = traversalNode;
			traversalNode = traversalNode->next;
		}

		if(traversalNode!=NULL)
		{
			previousNode->next = traversalNode->next;
			delete traversalNode;
		}
	}
}

bool WordList::exists(string data)
{
	wordNode* traversalNode = this->head;
	while(traversalNode!=NULL && traversalNode->data!=data)
		traversalNode = traversalNode->next;
	if(traversalNode==NULL)
		return false;
	else
	{
		traversalNode = NULL;
		return true;
	}
}

int WordList::size()
{
	return this->listSize;
}

bool WordList::isEmpty()
{
	return this->head==NULL;
}

#endif