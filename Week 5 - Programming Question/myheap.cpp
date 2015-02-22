#include<iostream>
#include"myheap.h";

using namespace std;

myheap::myheap() {
	this->_currSize = 0;
	this->_capacity = 0;
	this->_arr = NULL;
}

myheap::myheap(long int capacity) {
	this->_capacity = capacity;
	this->_arr = (heapNode*) malloc(sizeof(heapNode) * capacity);
	this->_currSize = 0; // not an element yet
}

myheap::~myheap() {
	delete(this->_arr);
}

void myheap::deleteMyHeap() {
	this->~myheap();
}

long int myheap::getSize() {
	return this->_currSize;
}

long int myheap::getLeftKid(long int parentPos) {
	if (parentPos * 2 + 1 > this->_currSize - 1) {
		return -1;
	} else 
		return parentPos * 2 + 1;
}

long int myheap::getRightKid(long int parentPos) {
	if (parentPos * 2 + 2 > this->_currSize - 1) {
		return -1;
	} else 
		return parentPos * 2 + 2;
}

long int myheap::getParent(long int kidPos) {
	return (kidPos - 1) / 2;
}


void myheap::copyNode(heapNode & src, heapNode & des) {
	des.key = src.key;
	des.startVertex = src.startVertex;
	des.endVertex = src.endVertex;
}

void myheap::swapNode(heapNode & node1, heapNode & node2) {
	heapNode tmpNode;
	copyNode(node1,tmpNode);
	copyNode(node2,node1);
	copyNode(tmpNode,node2);
}

void myheap::heapify(long int fromPos) {
	long int leftKid = this->getLeftKid(fromPos);
	long int rightKid = this->getRightKid(fromPos);

	long int smallestPos = fromPos;

	if ( (leftKid != -1) && (_arr[leftKid].key < _arr[fromPos].key) ) {
		smallestPos =  leftKid;
	}

	if ( (rightKid != -1) && (_arr[rightKid].key < _arr[smallestPos].key) ) {
		smallestPos =  rightKid;
	}

	if (smallestPos != fromPos) {
		swapNode(_arr[smallestPos], _arr[fromPos]);
		heapify(smallestPos);
	}
}

// assume that size > 0
heapNode myheap::extractMin() {
	heapNode retNode;
	
	copyNode(this->_arr[0], retNode);
	swapNode(this->_arr[0], this->_arr[_currSize - 1]);
	
	this->_currSize--;
	heapify(0);

	return retNode;
}

void myheap::insertNode(heapNode & aNewNode) {
	_currSize++;

	long int currPos = _currSize - 1;
	copyNode(aNewNode,_arr[currPos]);
	
	long int parentPos = getParent(currPos);
	while( (currPos > parentPos) && (_arr[currPos].key < _arr[parentPos].key) ) {
		swapNode(_arr[currPos], _arr[parentPos]);
		
		currPos = parentPos;
		parentPos = getParent(currPos);
	}
}

void myheap::print() {
	for (int i = 0; i < this->_currSize; i++) {
		cout << _arr[i].key << " ";
	}
	cout << endl;
}