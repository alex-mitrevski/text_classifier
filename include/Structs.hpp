#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
using std::string;

struct documentFrequencyNode
{
	documentFrequencyNode() { this->data = ""; this->frequency = 0.0f; }
	documentFrequencyNode(string data) { this->data = data; this->frequency = 0.0f; };

	string data;
	float frequency;
};


struct transformedDocumentFeature
{
	transformedDocumentFeature() { this->index = 0; this->frequency = 0.0f; }
	transformedDocumentFeature(unsigned int index, float frequency) { this->index = index; this->frequency = frequency; }

	unsigned int index;
	float frequency;
};


//this struct is used in a doubly linked list
//which stores a string and the number of repetitions of that string
struct node
{
	//constructor
	node(string data) { this->next = NULL; this->previous = NULL; this->data = data; this->frequency = 1; }

	string data;
	int frequency;
	node* next;
	node* previous;
};


//this struct is used in a doubly linked list
//which stores a string and the probability of that string
struct probabilityNode
{
	//constructor
	probabilityNode(string data, double probability) { this->next = NULL; this->data = data; this->probability = probability; }

	string data;
	double probability;
	probabilityNode* next;
	probabilityNode* previous;
};


//this struct is used in a doubly linked list which stores a string
struct wordNode
{
	wordNode(string data) { this->data = data; this->next = NULL; }

	string data;
	wordNode* next;
	wordNode* previous;
};

#endif
