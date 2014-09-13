#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <string>
using std::string;

struct node
{
	//constructor
	node(string data) { this->next = NULL; this->previous = NULL; this->data = data; this->frequency = 1; }

	string data;
	int frequency;
	node* next;
	node* previous;
};

class FrequencyList
{
public:
	//constructor
	FrequencyList();
	~FrequencyList();

	//accessor functions
	node* getHead();

	//data handling functions
	void insert(string);
	void remove(string);
	int findFrequency(string);
	string findMaxFrequency();
	int size();
	bool isEmpty();

private:
	node* head;
	node* tail;
	int listSize;
};


FrequencyList::FrequencyList()
{
	this->head = NULL;
	this->listSize = 0;
}

FrequencyList::~FrequencyList()
{
	while(this->listSize>0)
		this->remove(this->head->data);
}

node* FrequencyList::getHead()
{
	return this->head;
}

//the insert function doesn't allow duplicates in the list
void FrequencyList::insert(string data)
{
	if(this->head==NULL)
	{
		this->head = new node(data);
		this->tail = this->head;
		this->listSize++;
	}
	else
	{
		node* traversalNode = this->head;
		bool sameDataFound = false;

		//traversalNode->data==data because the data can match the data in the head, which we will miss without this condition;
		//in other words, if the data matches the field in the head, then we go inside the loop and go out immediately;
		//otherwise, the loop continues until a match is found or the end of the list is reached
		while(traversalNode->next != NULL || traversalNode->data==data)
		{
			if(traversalNode->data==data)
			{
				traversalNode->frequency++;
				traversalNode = NULL;
				sameDataFound = true;
				break;
			}
			traversalNode = traversalNode->next;
		}

		if(!sameDataFound)
		{
			traversalNode->next = new node(data);
			traversalNode->next->previous = traversalNode;
			this->tail = traversalNode->next;
			this->listSize++;
		}
	}
}

void FrequencyList::remove(string data)
{
	if(this->head!=NULL)
	{
		node* traversalNode = this->head;
		while(traversalNode!=NULL && traversalNode->data!=data)
			traversalNode = traversalNode->next;

		if(traversalNode!=NULL)
		{
			//if the node we are deleting is not the head, we assign 'next' of the node before traversalNode to 'next' of traversalNode
			if(traversalNode->previous != NULL)
			{
				traversalNode->previous->next = traversalNode->next;

				//if we are not deleting the tail
				if(traversalNode->next != NULL)
					traversalNode->next->previous = traversalNode->previous;
				else
					this->tail = traversalNode->previous;
			}
			else
			{
				this->head = traversalNode->next;

				if(traversalNode->next != NULL)
					this->head->previous = NULL;

				if(this->head == NULL)
				{
					this->head = NULL;
					this->tail = NULL;
				}
				else if(this->head->next == NULL)
					this->tail = this->head;
			}

			delete traversalNode;
		}

		this->listSize--;
	}
}

int FrequencyList::findFrequency(string data)
{
	node* traversalNode = this->head;
	while(traversalNode!=NULL && traversalNode->data!=data)
		traversalNode = traversalNode->next;
	if(traversalNode==NULL)
		return 0;
	else
	{
		int frequency = traversalNode->frequency;
		traversalNode = NULL;
		return frequency;
	}
}

string FrequencyList::findMaxFrequency()
{
	if(this->head != NULL)
	{
		node* traversalNode = this->head->next;
		string maximumFrequencyData = this->head->data;
		int maxFrequency = this->head->frequency;

		while(traversalNode!=NULL)
		{
			if(traversalNode->frequency > maxFrequency)
			{
				maxFrequency = traversalNode->frequency;
				maximumFrequencyData = traversalNode->data;
			}
			traversalNode = traversalNode->next;
		}

		return maximumFrequencyData;
	}
	else
		return "";
}

int FrequencyList::size()
{
	return this->listSize;
}

bool FrequencyList::isEmpty()
{
	return this->head==NULL;
}

#endif