#include <iostream> 
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

#include "PrimMinHeap.h";

using namespace std;

struct Edge {
	long int start;
	long int end;
	long int cost; 

	Edge(long int _start, long int _end, long int _cost) {
		start = _start;
		end = _end;
		cost = _cost;
	}
};


void ReadFile(string fileName, unordered_set<long int>& nodes, vector<Edge>& edges, unordered_map<long int, vector<Edge> >& mapEdge) {
	ifstream inputFile(fileName);
	stringstream buffer;

	string numberNodes;
	string numberEdges;

	string startVertex;
	string endVertex;
	string edgeCost;

	try {
		buffer << inputFile.rdbuf();
		
		buffer >> numberNodes >> numberEdges;
		//cout << numberNodes <<  " " << numberEdges << endl;

		while (buffer >> startVertex >> endVertex >> edgeCost) {
			long int _startVertex = stol(startVertex);
			long int _endVertex = stol(endVertex);
			long int _edgeCost = stol(edgeCost);

			Edge aEdge = Edge(_startVertex,_endVertex,_edgeCost);
			
			nodes.insert(_startVertex);
			nodes.insert(_endVertex);

			edges.push_back(aEdge);
			
			auto it = mapEdge.find(_startVertex);
			if (it == mapEdge.end()) {
				vector<Edge> vEdge;
				vEdge.push_back(aEdge);
				mapEdge.emplace(_startVertex,vEdge);
			}
			else {
				it->second.push_back(aEdge);
			}
			
			it = mapEdge.find(_endVertex);
			if (mapEdge.find(_endVertex) == mapEdge.end()) {
				vector<Edge> vEdge;
				vEdge.push_back(aEdge);
				mapEdge.emplace(_endVertex,vEdge);
			} else {
				it->second.push_back(aEdge);
			}

		}

	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}

void InitPrimMinHeap(long int startIndex, vector<Edge>& edges, PrimMinHeap& pMinHeap) {

	for (auto it = edges.begin(); it != edges.end(); it++) {
		if (it->start == startIndex) {
			pMinHeap.SetKey(it->cost, it->end);			
		}
		else if (it->end == startIndex) {
			pMinHeap.SetKey(it->cost, it->start);
		}
	}
}

void UpdateAdjacentVertex(long int startVertex, PrimMinHeap& pMinHeap, unordered_set<long int> & spannedVertex,unordered_map<long int, vector<Edge> > & mapEdge) {
	auto itMap = mapEdge.find(startVertex);

	for (auto itV = itMap->second.begin(); itV != itMap->second.end(); itV++) {
		long int updateVertex = -1;
		
		if (itV->start == startVertex) {
			updateVertex = itV->end;
		} else if (itV->end = startVertex)  {
			updateVertex = itV->start;
		}

		if (spannedVertex.find(updateVertex) == spannedVertex.end()) {
			long int heapIndex = pMinHeap.GetHeapIndex(updateVertex);
			long int currentValue = pMinHeap.GetHeapValue(heapIndex);

			if (currentValue > itV->cost) {
				pMinHeap.SetKey(itV->cost,heapIndex);
				pMinHeap.BubleUp(heapIndex);
			}
		}

	}
}

long long int RunPrimAlgorithm(unordered_set<long int> & nodes, PrimMinHeap& pMinHeap, unordered_set<long int> & spannedVertex,unordered_map<long int, vector<Edge> > & mapEdge) {
	long long int mst = 0;

	while(nodes.size() != spannedVertex.size()) {
		auto extractedVertex = pMinHeap.ExtratMin(); // pair 1st is value, 2nd is index
		
		mst += extractedVertex.first;
		spannedVertex.emplace(extractedVertex.second);

		UpdateAdjacentVertex(extractedVertex.second,pMinHeap,spannedVertex,mapEdge);

		//cout << "Current Spanning Size: " << spannedVertex.size() << endl;
	}

	return mst;
}

int main() {
	long int startIndex = 1; // fix startIndex

	unordered_set<long int> nodes;
	vector<Edge> edges;
	unordered_map<long int, vector<Edge> > mapEdge;

	unordered_set<long int> spannedVertex;
	spannedVertex.emplace(startIndex);

	ReadFile("edges.txt", nodes, edges,mapEdge);
	

	PrimMinHeap pMinHeap = PrimMinHeap(nodes.size() ); // start at index 1;
	InitPrimMinHeap(startIndex,edges,pMinHeap);
	//pMinHeap.PrintHeap();
	
	pMinHeap.Heapify();
	//pMinHeap.PrintHeap();
	
	long long int mst = RunPrimAlgorithm(nodes,pMinHeap,spannedVertex,mapEdge);
	cout << "Prim MST: " <<  mst << endl;

	pMinHeap.~PrimMinHeap();
	
	return 0;
}
