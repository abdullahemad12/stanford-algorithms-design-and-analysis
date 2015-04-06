#include <iostream>
#include "PrimMinHeap.h"

using namespace std;

PrimMinHeap::PrimMinHeap(long int numVertex) {
	_activeVertex = numVertex;
	_totalVertex = numVertex;

	for (long int i = 0; i <= _activeVertex; i++) {
		arrKey.push_back(LONG_MAX);
		arrBookKey.push_back(i);
		mapKeyPos.emplace(i,i);
	}
	
	
}

PrimMinHeap::~PrimMinHeap() {
}

long int PrimMinHeap::GetCurrentSize() {
	return _activeVertex;
}

long int PrimMinHeap::GetTotalSize() {
	return _totalVertex;
}

void PrimMinHeap::SwapKey(long int index1, long int index2) {
	long int tmpIndex = arrBookKey[index1];
	long int tmpValue = arrKey[index1];

	arrBookKey[index1] = arrBookKey[index2];
	arrKey[index1] = arrKey[index2];

	arrBookKey[index2] = tmpIndex;
	arrKey[index2] = tmpValue;

	auto itIndex1 = mapKeyPos.find(arrBookKey[index1]);
	itIndex1->second = index1;

	auto itIndex2 = mapKeyPos.find(arrBookKey[index2]);
	itIndex2->second = index2;
}

void PrimMinHeap::HeapifyAdHoc(long int atIndex) {
	if (atIndex > _activeVertex)
		return;

	if (atIndex * 2 > _activeVertex)
		return;

	long int smallestIndex = atIndex;
	
	if ( (atIndex * 2 <= _activeVertex) && (arrKey[atIndex] > arrKey[atIndex * 2]) )
		smallestIndex = atIndex * 2;

	if ( (atIndex * 2 + 1 <= _activeVertex) && (arrKey[smallestIndex] > arrKey[atIndex * 2 + 1]) )
		smallestIndex = atIndex * 2 + 1;

	if (smallestIndex != atIndex) {
		SwapKey(smallestIndex, atIndex);
		HeapifyAdHoc(smallestIndex);
	}
}

void PrimMinHeap::Heapify() {
	long int halfArr = _activeVertex / 2;

	for (int i = halfArr; i >= 1; i --) {
		HeapifyAdHoc(i);
	}
}

void PrimMinHeap::PrintHeap() {
	cout << "Total vertexes: " << _totalVertex << " Total active: " << _activeVertex << endl;

	for (int i = 1; i <= _activeVertex;i++) {
		cout << "(keyValue: " << arrKey[i] << " currently heapIndex: " << GetHeapIndex(i) << "  )" << endl;
	}
}

long int PrimMinHeap::GetHeapIndex(long int vertextIndex){
	auto itFind = mapKeyPos.find(vertextIndex);

	return itFind->second;
}


long int PrimMinHeap::GetHeapValue(long int heapIndex){
	return arrKey[heapIndex];
}

pair<long int, long int> PrimMinHeap::ExtratMin() {
		// save value of vertex min

	long int minVertex  = arrBookKey[1];
	long int minValue = arrKey[1];
	
	SwapKey(1, _activeVertex);

	_activeVertex--;
		
	HeapifyAdHoc(1);
	
	return make_pair(minValue, minVertex);

		//auto itFind = mapKeyPos.find(arrBookKey[_activeVertex + 1]);

		//return make_pair(arrKey[_activeVertex + 1], itFind->first);
}

//unordered_map<long int, long int> PrimMinHeap::GetMapVertexPos() {
//	return mapKeyPos;
//}

void PrimMinHeap::BubleUp(long int heapIndex) {
	long int kidIndex = heapIndex  / 2;

	while (kidIndex >= 1) {
		if (arrKey[kidIndex] > arrKey[heapIndex]) {
			SwapKey(kidIndex,heapIndex);
			heapIndex = kidIndex;
			kidIndex = heapIndex / 2;
		}
		else 
			break;
	}
}