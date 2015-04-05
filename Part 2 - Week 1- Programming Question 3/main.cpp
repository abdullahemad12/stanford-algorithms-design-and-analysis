#include <iostream> 
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
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


void ReadFile(string fileName, unordered_set<long int>& nodes, vector<Edge>& edges) {
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
			
		}

	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}


// assume that startVertex already in spannedVertex. Now get the other Vertex
// find minEdge based on one span vertex
vector<Edge>::iterator GetMinEdge(vector<Edge>& edges, long int startVertex, unordered_set<long int>& spannedVertex) 
{
	vector<Edge>::iterator minEdge = edges.end();
	long int minEdgeCost = LONG_MAX;

	for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
		if ( (it->start == startVertex)  && (spannedVertex.find(it->end) == spannedVertex.end())
			|| ( (it->end == startVertex) && (spannedVertex.find(it->start) == spannedVertex.end()) )
			)
		{
			if (minEdgeCost > it->cost) {
				minEdge = edges.begin() + (it - edges.begin());
				minEdgeCost = it->cost;
			}
			
		}
	}

	return minEdge;
}

// insert vertex based on all span vertex
long int InsertNewVertexIntoSpannedTree(vector<Edge>& edges,unordered_set<long int>& spannedVertex) {
	
	vector<Edge>::iterator minEdgeOne;
	vector<Edge>::iterator minEdgeAll;
	long int minEdgeCost = LONG_MAX;

	for (unordered_set<long int>::iterator itSpanV = spannedVertex.begin(); itSpanV != spannedVertex.end(); itSpanV++) {
		minEdgeOne = GetMinEdge(edges, *itSpanV, spannedVertex);
		
		if (minEdgeOne != edges.end() ) {
			if (minEdgeOne->cost < minEdgeCost) {
				minEdgeAll = minEdgeOne;
				minEdgeCost = minEdgeOne->cost;
			}
		}
	}
	
	minEdgeCost = minEdgeAll->cost;
	
	// here minEdge overall
	spannedVertex.insert(minEdgeAll->start);
	spannedVertex.insert(minEdgeAll->end);

	//cout << minEdgeAll->start << " "  << minEdgeAll->end << endl;
	// after get minEdge, pop out
	edges.erase(minEdgeAll, minEdgeAll + 1);

	return minEdgeCost;
}

long int GetMST(unordered_set<long int>  nodes, vector<Edge> & edges) {
	long long int mst = 0;
	unordered_set<long int> spannedVertex;
	
	spannedVertex.emplace(*nodes.begin());

	while(spannedVertex.size() != nodes.size()) {
		mst += InsertNewVertexIntoSpannedTree(edges,spannedVertex);
		cout << "spanned size: " << spannedVertex.size() << " " << "edge size: " << edges.size() <<  endl;
		
		/*if (spannedVertex.size() == 184) {
			cout << "debug here " << endl;
		}*/

	}

	return mst;
}

int main() {
	unordered_set<long int> nodes;
	vector<Edge> edges;

	ReadFile("edges2.txt", nodes, edges);
	
	long long int mst = GetMST(nodes, edges);

	cout << "mst value: " << mst << endl;
	return 0;
}