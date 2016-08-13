#ifndef HEAP_HPP
#define HEAP_HPP

#include "FrequencyList.hpp"
#include <vector>
using std::vector;

class Heap
{
public:
	Heap();
	~Heap();
	void insert(node);
	string extractMaximum();
	void bubbleUp();
	void bubbleDown();
	void swap(unsigned int, unsigned int);

private:
	vector<node>* heap;
};

#endif
