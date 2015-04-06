#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// start at index 1
// i --> left: (2*i) and right (2*i)  + 1
class PrimMinHeap {
private:
	long int _totalVertex; // total number of vertex or size of heap
	long int _activeVertex; // number of currenly active vertex

	vector<long int> arrKey;
	vector<long int> arrBookKey; // map position -> key: answer question: in index i of heap, which vertex in
	unordered_map<long int, long int> mapKeyPos; // map key->pos: answer question: vertex i in which heap position

	void SwapKey(long int index1, long int index2);
public:
	PrimMinHeap(long int numVertex);
	long int GetTotalSize();
	long int GetCurrentSize();
	~PrimMinHeap();

	void Heapify();
	void HeapifyAdHoc(long int atIndex);

	void PrintHeap();

	vector<long int>& GetArrKey() { return arrKey; }
	//vector<long int>& GetBookKey() { return arrBookKey; }

	void SetKey(long int keyValue, long int keyIndex) { arrKey[keyIndex] = keyValue; }

	pair<long int,long int> ExtratMin();

	long int GetHeapIndex(long int vertextIndex);
	long int GetHeapValue(long int heapIndex);

	//unordered_map<long int, long int> GetMapVertexPos();

	void BubleUp(long int heapIndex);

};