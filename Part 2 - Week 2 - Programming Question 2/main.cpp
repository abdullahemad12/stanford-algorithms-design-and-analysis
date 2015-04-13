#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>    
#include <unordered_set>
#include <unordered_map>
#include <list>

using namespace std;

struct Edge {
	long int startV;
	long int endV;
	long int cost;

	Edge (long int startVertex, long int endVertex, long int edgeCost) {
		startV = startVertex;
		endV = endVertex;
		cost = edgeCost;
	}
};

struct EdgeCompare {
	bool operator()(Edge& lhs, Edge& rhs) {
		return (lhs.cost > rhs.cost);
	}
};


struct EdgeDistance {
	long int edgeMask;
	long int value;
	EdgeDistance(long int _edgeMask, long int _value) {
		edgeMask = _edgeMask;
		value = _value;
	}

};

struct Node {
	long int parentNode;
	long int size;

	Node(long int ParentNode, long int Size) : size(Size) {parentNode = ParentNode; }
};

long int ConvertBitToInt(const string& bitArr) {
	long int value = 0;

	for (auto it = bitArr.rbegin(); it != bitArr.rend(); it++) {
		value += pow(2,it-bitArr.rbegin()) * (*it - '0');

	}

	return value;
}

long int GetEdgeDistance(long int maskEdge) {
	long int count = 0;
	while (maskEdge > 0) {
		maskEdge = (maskEdge - 1) & (maskEdge);
		count++;
	}

	return count;
}

void GenerateMask(vector<EdgeDistance>& maskTable, long int numberBitsPerNode) {
	
	unordered_set<long int> maskTableTmp;

	// Generate 1 distance 
	long int init1D = 0;

	maskTableTmp.emplace(init1D);

	init1D = init1D | 1;
	for (long int i = 1; i <= numberBitsPerNode;i++) {
		maskTableTmp.emplace(init1D);
		init1D = init1D << 1;
	}

	// Generate 2 distance
	long int init2D = 0;
	maskTableTmp.emplace(init2D);
	
	for (long int i = 0; i < numberBitsPerNode - 1;i++) {
		init2D = 0;
		init2D = init2D | 1;
		init2D = init2D << i;

		for (int j = i + 1; j < numberBitsPerNode; j++) {
			long int anotherBit = 1 << j;
			long int init2DCombine = init2D | anotherBit;
			maskTableTmp.emplace(init2DCombine);
		}
	}

	for(auto it = maskTableTmp.begin(); it != maskTableTmp.end(); it++) {
		EdgeDistance ed = EdgeDistance(*it,GetEdgeDistance(*it));
		maskTable.push_back(ed);
	}

}

void ReadFile(string filename, unordered_set<long int>& nodes, long int& numberBitsPerNode) {
	string strNumberOfNodes;
	long int numberOfNodes;

	string strNumberBitsPerNode;

	ifstream inputFile(filename);
	stringstream buffer;
	
	string firstBit;
	string nextBit;
	
	
	try {
		buffer << inputFile.rdbuf();
		
		buffer >> strNumberOfNodes >> strNumberBitsPerNode;
		numberOfNodes = stol(strNumberOfNodes);
		numberBitsPerNode = stol(strNumberBitsPerNode);
		
		while (buffer >> firstBit) { // see if first bit is available
			string arrBit;
			arrBit.append(firstBit);

			for ( int i = 1; i <= numberBitsPerNode - 1; i++) {
				buffer >> nextBit;
				arrBit.append(nextBit);
			}
			
			long int bitValue = ConvertBitToInt(arrBit);
			nodes.emplace(bitValue);

		}

		
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}


bool CheckEdgeAlreadyExist(Edge& aEdgeS, unordered_map<long int, unordered_set<long int> >&  existEdge) {
	bool alreadyExit = true;

	// check startVertex
	auto itFindS = existEdge.find(aEdgeS.startV);

	if (itFindS == existEdge.end()) {
		unordered_set<long int> setVertexS;
		setVertexS.emplace(aEdgeS.endV);

		existEdge.emplace(aEdgeS.startV,setVertexS);
	
		alreadyExit = false;
	
	} else { // vertex one already there. Check it adjacent
		auto itFindSetS = itFindS->second.find(aEdgeS.endV);
		if (itFindSetS == itFindS->second.end()) {
			itFindS->second.emplace(aEdgeS.endV);
			alreadyExit = false;
		}
		else {
			alreadyExit = true;
		}
	}


	// check end Vertex
	auto itFindE = existEdge.find(aEdgeS.endV);

	if (itFindE == existEdge.end()) {
		unordered_set<long int> setVertexE;
		setVertexE.emplace(aEdgeS.startV);
		
		existEdge.emplace(aEdgeS.endV,setVertexE);		

		alreadyExit = false;
	
	} else { // vertex one already there. Check it adjacent
		auto itFindSetS = itFindE->second.find(aEdgeS.startV);
		if (itFindSetS == itFindE->second.end()) {
			itFindE->second.emplace(aEdgeS.startV);
			alreadyExit = false;
		}
		else {
			alreadyExit = true;
		}
	}

	return alreadyExit;

}


void BuildGraph(vector<EdgeDistance>& maskTable,unordered_set<long int>& nodes, list<Edge>& graphEdges) {
	
	long int count = 0;

	unordered_map<long int, unordered_set<long int> >  existEdge;

	for (auto itNode = nodes.begin(); itNode != nodes.end();  itNode++) {
		count++;

		if ( (count % 2000) == 0 ) 
			cout << "Building graph for: " << count << " vertexes " << endl;

		for (auto itMask = maskTable.begin(); itMask != maskTable.end(); itMask++) {
			
			long int startVertex = *itNode;
			long int endVertex = (*itNode) ^ (itMask->edgeMask);

			if (startVertex != endVertex) {
				auto itFind = nodes.find(endVertex);
				if ( itFind != nodes.end() ) {
					Edge aEdgeS = Edge(startVertex,endVertex,itMask->value);
					
					if (!CheckEdgeAlreadyExist(aEdgeS,existEdge)) {
						if (itMask->value == 1)
							graphEdges.push_front(aEdgeS);
						else if (itMask->value == 2)
							graphEdges.push_back(aEdgeS);


					}

					//graphEdges.emplace(aEdgeS);
					
				}
			}
		}
	}
}


// direct union
long int FindUnion(unordered_map<long int, Node>& nodes, long int aNode) {
	auto itFind = nodes.find(aNode);

	long int parentNode = itFind->second.parentNode;

	while (parentNode != aNode) {
		aNode = parentNode;
		itFind = nodes.find(aNode);
		parentNode = itFind->second.parentNode;
	}

	return parentNode;
}

// assume that node1.size < node2.size --> update all children of node1 and node1 itself
void UpdateMergeNode(unordered_map<long int, Node>& nodes, Node& node1, Node& node2) {
	
	node2.size = node1.size + node2.size; // only parent node update size
	
	long int node1V = node1.parentNode; // node1 is parent of itself
	long int node2V = node2.parentNode; // node 2 is parent of itself too

	// update all children of node1 points to node2
	// not use direct union
	/*
	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		if (it->second.parentNode == node1V)
			it->second.parentNode = node2V;
	}
	*/
	node1.parentNode = node2V;

}

bool MergeCluster(Edge aEdge, unordered_map<long int, Node>& nodes) {
	bool doMerge = false;
	long int node1 = aEdge.startV;
	long int node2 = aEdge.endV;

	long int parentNode1 = FindUnion(nodes, node1);
	long int parentNode2 = FindUnion(nodes, node2);

	if (parentNode1 != parentNode2) {
		doMerge = true;
		auto pNode1 = nodes.find(parentNode1);
		auto pNode2 = nodes.find(parentNode2);

		if (pNode1->second.size < pNode2->second.size) {
			UpdateMergeNode(nodes, pNode1->second, pNode2->second);
		} else {
			UpdateMergeNode(nodes, pNode2->second, pNode1->second);
		}
	}
	else {
		// nothing to do
	}
	return doMerge;
}


long int RunClusteringAlgo(unordered_map<long int, Node>& nodes, list<Edge>& edges) {
	long int maxDistance = 0;
	long int numCluster = nodes.size();
	long int count = 0;

	cout << "Brutal force total edges: " << edges.size() << endl;
	
	while (edges.size() > 0) {
		count++;
		if ( (count % 2000) == 0 ) 
			cout << "Already did for: " << count << " edges " << endl;

		Edge smallestEdge = edges.front();
		edges.pop_front();

		bool doMerge = MergeCluster(smallestEdge, nodes);
		
		if (doMerge) 
			numCluster--;
	}
		
	return numCluster;
}




int main() {

	//string bitArray = "111000001101001111001111";
	//long int value = ConvertBitToInt(bitArray);
	//cout << value << endl;

	vector<EdgeDistance> maskTable;
	unordered_set<long int> nodes;
	
	long int numberBitsPerNode;

	ReadFile("clustering_big.txt",nodes,numberBitsPerNode);
	cout << "Finish Reading File ... " << endl;

	GenerateMask(maskTable,numberBitsPerNode);
	cout << "Finish Generating Mask ... " << endl;

	//long int edgeDist =  GetEdgeDistance(2);
	//edgeDist =  GetEdgeDistance(1);
	//edgeDist =  GetEdgeDistance(0);
	//edgeDist =  GetEdgeDistance(3);
	
	//priority_queue<Edge, vector<Edge>,EdgeCompare> graphEdges;
	list<Edge> graphEdges;

	BuildGraph(maskTable,nodes, graphEdges);
	cout << "Finish Building Graph ... " << endl;

	unordered_map<long int, Node> graphNode;

	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		Node aNode = Node(*it,1); // parent itself
		graphNode.emplace(*it,aNode);
	}
	
	long int numCluster = RunClusteringAlgo(graphNode,graphEdges);
	cout << "Number of Clusters: " << numCluster << endl;
	return 0;
}