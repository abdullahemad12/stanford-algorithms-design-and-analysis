#include <iostream>

struct heapNode {
	long int key;
	long int startVertex;
	long int endVertex;
};

class myheap {
public:
	explicit myheap();
	myheap(long int capacity);
	~myheap();
	void deleteMyHeap();

	void insertNode(heapNode & aNewHeapNode);
	heapNode extractMin();
	long int getSize();

	void print();
private:
	heapNode * _arr;
	long int _capacity; //full all nodes
	long int _currSize; //currSize;

	void heapify(long int fromPos);
	long int getLeftKid(long int parentPos);
	long int getRightKid(long int parentPos);

	long int getParent(long int kidPos);
	void copyNode(heapNode & src, heapNode & des);
	void swapNode(heapNode & node1, heapNode & node2);
};