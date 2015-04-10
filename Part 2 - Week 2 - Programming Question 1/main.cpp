#include <iostream>
#include <vector>
#include <queue> // priority queue heap
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


struct Edge {
	long int startNode;
	long int endNode;
	long int cost;

	Edge(long int StartNode, long int EndNode, long int Cost) {
		startNode = StartNode;
		endNode = EndNode;
		cost = Cost;
	}
};

// clustering 
// applied union find algorithm


struct EdgeCompare {
	bool operator()(Edge& lhs, Edge& rhs) {
		return (lhs.cost > rhs.cost);
	}
};


struct Node {
	long int node;
	long int size;

	Node(long int Node, long int Size) : node(Node), size(Size) {}
};

// nodes[i] (start at 1) =  j means
// j is parent of i
void ReadFile(string fileName, vector<Node>& nodes, priority_queue<Edge,vector<Edge>, EdgeCompare>& edges) {
	ifstream inputFile(fileName);
	stringstream buffer;

	string numberNodes;
	string numberEdges;

	string startVertex;
	string endVertex;
	string edgeCost;

	Node Node0 = Node(0,0);
	nodes.push_back(Node0);

	try {
		buffer << inputFile.rdbuf();
		
		buffer >> numberNodes;
		for (int i = 1; i <= stol(numberNodes); i++)  {
			Node nodeI =  Node(i,1);
			nodes.push_back(nodeI);
		}

		while (buffer >> startVertex >> endVertex >> edgeCost) {
			long int _startVertex = stol(startVertex);
			long int _endVertex = stol(endVertex);
			long int _edgeCost = stol(edgeCost);

			Edge aEdge = Edge(_startVertex,_endVertex,_edgeCost);
			edges.emplace(aEdge);
			
		}

	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}

// direct union
long int FindUnion(vector<Node>& nodes, long int aNode) {
	return nodes[aNode].node;
}

// assume that node1.size < node2.size --> update all children of node1 and node1 itself
void UpdateMergeNode(vector<Node>& nodes, Node& node1, Node& node2) {
	// only parent node update size
	node2.size = node1.size + node2.size;

	// update all children of node1 points to node2
	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		if (it->node == node1.node)
			it->node = node2.node;
	}

}





bool MergeCluster(Edge aEdge, vector<Node>& nodes) {
	bool doMerge = false;
	long int node1 = aEdge.startNode;
	long int node2 = aEdge.endNode;

	long int parentNode1 = FindUnion(nodes, node1);
	long int parentNode2 = FindUnion(nodes, node2);

	if (parentNode1 != parentNode2) {
		doMerge = true;
		Node& pNode1 = nodes[parentNode1];
		Node& pNode2 = nodes[parentNode2];

		if (pNode1.size < pNode2.size) {
			UpdateMergeNode(nodes, pNode1, pNode2);
		} else {
			UpdateMergeNode(nodes, pNode2, pNode1);
		}
	}
	else {
		// nothing to do
	}
	return doMerge;
}


long int RunClusteringAlgo(vector<Node>& nodes, priority_queue<Edge,vector<Edge>,EdgeCompare>& edges, int k) {
	long int maxDistance = 0;
	long int numCluster = nodes.size();
	
	
	while (numCluster > k + 1) {
		Edge smallestEdge = edges.top();
		edges.pop();

		bool doMerge = MergeCluster(smallestEdge,nodes);
		
		if (doMerge) 
			numCluster--;
	}
	
	// next edge is the max of min distance
	while (true) {
		Edge smallestEdge = edges.top();
		edges.pop();

		long int node1 = smallestEdge.startNode;
		long int node2 = smallestEdge.endNode;

		long int parentNode1 = FindUnion(nodes, node1);
		long int parentNode2 = FindUnion(nodes, node2);

		if (parentNode1 != parentNode2) {
			maxDistance= smallestEdge.cost;
			break;
		}
		
	}

	return maxDistance;
}


int main() {
	int k = 4;
	vector<Node> nodes;
	priority_queue<Edge,vector<Edge>,EdgeCompare> edges;

	ReadFile("clustering1.txt", nodes, edges);
	long int maxd = RunClusteringAlgo(nodes,edges,k);
	cout << maxd << endl;

	return 0;
}