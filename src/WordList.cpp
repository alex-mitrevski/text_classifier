#include "WordList.hpp"

WordList::WordList()
{
	this->head = NULL;
	this->listSize = 0;
}

wordNode* WordList::getHead() const
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

bool WordList::exists(string data) const
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

int WordList::size() const
{
	return this->listSize;
}

bool WordList::isEmpty() const
{
	return this->head==NULL;
}
