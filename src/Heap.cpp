#include "Heap.hpp"

Heap::Heap()
{
	this->heap = NULL;
}

Heap::~Heap()
{
	delete this->heap;
}

void Heap::insert(node newNode)
{
	if(this->heap==NULL)
		this->heap = new vector<node>();

	this->heap->push_back(newNode);
	this->bubbleUp();
}

string Heap::extractMaximum()
{
	if(this->heap==NULL)
		return "";

	string word = (*this->heap)[0].data;
	this->bubbleDown();
	return word;
}

void Heap::bubbleUp()
{
	unsigned int currentChild = this->heap->size() - 1;
	unsigned int currentParent = currentChild / 2;

	while((*this->heap)[currentChild].frequency > (*this->heap)[currentParent].frequency)
	{
		this->swap(currentChild,currentParent);
		currentChild = currentParent;
		currentParent = currentChild / 2;
	}
}

void Heap::bubbleDown()
{
	this->swap(0,this->heap->size()-1);
	this->heap->pop_back();

	unsigned int size = this->heap->size();
	unsigned int currentParent = 0;
	unsigned int currentChild = 0;
	unsigned int leftChild = currentParent * 2 + 1;
	unsigned int rightChild = currentParent * 2 + 2;

	if(leftChild < size && rightChild < size)
	{
		if((*this->heap)[leftChild].frequency>=(*this->heap)[rightChild].frequency)
			currentChild = leftChild;
		else
			currentChild = rightChild;
	}
	else if(leftChild < size)
		currentChild = leftChild;
	else if(leftChild < size)
		currentChild = rightChild;
	else
		return;

	while(currentChild < size && ((*this->heap)[currentParent].frequency < (*this->heap)[currentChild].frequency))
	{
		this->swap(currentParent,currentChild);
		currentParent = currentChild;

		leftChild = currentParent * 2 + 1;
		rightChild = currentParent * 2 + 2;

		if(leftChild < size && rightChild < size)
		{
			if((*this->heap)[leftChild].frequency>=(*this->heap)[rightChild].frequency)
				currentChild = leftChild;
			else
				currentChild = rightChild;
		}
		else if(leftChild < size)
			currentChild = leftChild;
		else if(leftChild < size)
			currentChild = rightChild;
	}
}

void Heap::swap(unsigned int index1, unsigned int index2)
{
	node temp = (*this->heap)[index1];
	(*this->heap)[index1] = (*this->heap)[index2];
	(*this->heap)[index2] = temp;
}
