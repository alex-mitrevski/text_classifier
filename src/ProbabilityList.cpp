#include "ProbabilityList.hpp"

ProbabilityList::ProbabilityList()
{
	this->head = NULL;
	this->listSize = 0;
}

ProbabilityList::~ProbabilityList()
{
	while(this->listSize > 0)
		this->remove(this->head->data);
}

probabilityNode* ProbabilityList::getHead()
{
	return this->head;
}

//the insert function doesn't allow duplicates in the list
void ProbabilityList::insert(string data, double probability)
{
	if(this->head==NULL)
	{
		this->head = new probabilityNode(data,probability);
		this->tail = this->head;
		this->listSize++;
	}
	else
	{
		probabilityNode* traversalNode = this->head;
		bool sameDataFound = false;

		//traversalNode->data==data because the data can match the data in the head, which we will miss without this condition;
		//in other words, if the data matches the field in the head, then we go inside the loop and go out immediately;
		//otherwise, the loop continues until a match is found or the end of the list is reached
		while(traversalNode->next != NULL)
			traversalNode = traversalNode->next;

		traversalNode->next = new probabilityNode(data,probability);
		traversalNode->next->previous = traversalNode;
		this->tail = traversalNode->next;
		this->listSize++;
	}
}

void ProbabilityList::remove(string data)
{
	if(this->head!=NULL)
	{
		probabilityNode* traversalNode = this->head;
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

double ProbabilityList::getProbability(string data)
{
	probabilityNode* traversalNode = this->head;
	while(traversalNode!=NULL && traversalNode->data!=data)
		traversalNode = traversalNode->next;

	if(traversalNode==NULL)
		return 0.0;
	else
	{
		double probability = traversalNode->probability;
		traversalNode = NULL;
		return probability;
	}
}

int ProbabilityList::size()
{
	return this->listSize;
}

bool ProbabilityList::isEmpty()
{
	return this->head==NULL;
}
