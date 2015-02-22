#include <iostream> 
#include <string>
#include <fstream>
#include <sstream>      // std::stringstream
#include <vector>
#include <unordered_map>
#include "myheap.h";

#define const long int MAX_DISTNCE 1000000

using namespace std;

struct AdjVertex {
	long int nextVertex;
	long int distace;
};


vector<string> splitString(string &src, char delim) {
	vector<string> vStr;

	size_t firstPos = 0;
	size_t found = src.find(delim,firstPos);

	while(found != string::npos) {
		vStr.push_back(src.substr(firstPos,found - firstPos));
		firstPos = found + 1;

		found = src.find(delim,firstPos);
	}
	vStr.push_back(src.substr(firstPos,vStr.size() - 1));

	return vStr;
}


void parseEachWord(string Word, AdjVertex &adj) {
	for (string::iterator it = Word.begin(); it != Word.end(); it++) {
		if (*it == ',') {
			adj.nextVertex = stol(string(Word.begin(), it));
			adj.distace = stol(string(it + 1, Word.end()));
		}
	}
}

bool isNumberOrComma(char cat) {
	return ( ((cat > '0') &&  (cat <'9')) || (cat == ',') );
}

void parseLine(string line, unordered_map<long int,vector<AdjVertex> > & adjVertex ) {
	long int startVertex = -1;
	long int nextVertex = -1;
	long int distance = -1;

	long int startWord = 0;
	long int endWord = 0;

	AdjVertex aVertex;
	vector<AdjVertex> vAdj;

	// seperate by tab
	vector<string> vStrAdj = splitString(line,'\t');
	startVertex = stol(*vStrAdj.begin());

	for (vector<string>::iterator it = vStrAdj.begin()  + 1; it != vStrAdj.end(); it++) {
		aVertex.nextVertex = -1;
		aVertex.distace = -1;

		parseEachWord(*it,aVertex);
		if ( (aVertex.nextVertex != -1) && (aVertex.distace != -1)) {
			vAdj.emplace_back(aVertex);
		}
	}

	adjVertex.emplace(startVertex,vAdj);

}

void readDijkstra(string filename,unordered_map<long int,vector<AdjVertex> > & adjVertex) {
	ifstream inputFile(filename);

	string line;

	try {
		while(getline(inputFile,line)) {
			parseLine(line, adjVertex);
		}

	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}


long int coundEdge(unordered_map<long int,vector<AdjVertex> > & adjVertex)  {
	long int edgeCount  = 0;
	for (unordered_map<long int,vector<AdjVertex> >::iterator it = adjVertex.begin(); 	it != adjVertex.end(); it++) {
		edgeCount += it->second.size();
	}
	return (edgeCount / 2);
}

void printMap(unordered_map<long int, long int> &visited) {
	for (unordered_map<long int, long int>::iterator it = visited.begin(); it != visited.end(); it++) {
		cout << it->first << ":" << it->second << endl;		
	}
	cout << endl;
}


void printMap(unordered_map<long int, long int> &visited, long int atKey) {
	for (unordered_map<long int, long int>::iterator it = visited.begin(); it != visited.end(); it++) {
		if (it->first == atKey)
			cout << it->first << ":" << it->second << endl;		
	}
	cout << endl;
}


void shortestPathDijsktra(unordered_map<long int,vector<AdjVertex> > & adjVertex,  unordered_map<long int, long int> &visited, myheap & myMinHeap) {
	
	visited.emplace(1,0); // start at 1;
	unordered_map<long int,vector<AdjVertex> >::iterator itF = adjVertex.find(1);
	
	// for first node
	for (vector<AdjVertex>::iterator it = itF->second.begin(); it != itF->second.end(); it++) {
		heapNode aNewHeapNode;
		aNewHeapNode.key = it->distace;
		aNewHeapNode.startVertex = 1;
		aNewHeapNode.endVertex = it->nextVertex;

		myMinHeap.insertNode(aNewHeapNode);
	}

	// for other nodes
	while(visited.size() < adjVertex.size() ) {
		
		if (myMinHeap.getSize() > 0) {
			// extract mean edge
			heapNode minEdge = myMinHeap.extractMin();
			unordered_map<long int, long int>::iterator itInV = visited.find(minEdge.endVertex);
			
			while( (itInV != visited.end() && (myMinHeap.getSize() > 0)) ) { // to check not a visited end node
				minEdge = myMinHeap.extractMin();
				itInV = visited.find(minEdge.endVertex);
			}

			// here got minEdge and new node
			if ( (myMinHeap.getSize() > 0) && (itInV == visited.end()) )  {
				itF = adjVertex.find(minEdge.endVertex);
				visited.emplace(minEdge.endVertex, minEdge.key);

				for (vector<AdjVertex>::iterator it = itF->second.begin(); it != itF->second.end(); it++) {
					unordered_map<long int, long int>::iterator itAddV = visited.find( it->nextVertex);

					if (itAddV == visited.end()) {
						long int prevShortest = minEdge.key;

						heapNode aNewHeapNode;
						aNewHeapNode.key = prevShortest + it->distace;
						aNewHeapNode.startVertex = minEdge.endVertex;
						aNewHeapNode.endVertex = it->nextVertex;
					
					
						myMinHeap.insertNode(aNewHeapNode);
					}
				}
			} else {
				break;
			}

		} // end myMinHeap.getSize() > 0

	}
}

int main() {
	
	/*
	string Word = "123,3234";
	AdjVertex adjVertex;

	parseEachWord(Word,adjVertex);
	cout << adjVertex.nextVertex << "," << adjVertex.distace << endl;
	*/

	unordered_map<long int,vector<AdjVertex> > adjVertex;
	//readDijkstra("dijkstraDataSmall.txt",adjVertex);
	readDijkstra("dijkstraData.txt",adjVertex);

	// min heap test
	/*
	myheap myMinHeap(10);
	
	heapNode node1; node1.key = 3; node1.endVertex=3; node1.startVertex = 3;
	heapNode node2; node2.key = 5; node2.endVertex=5; node2.startVertex = 5;
	heapNode node3; node3.key = 7; node3.endVertex=7; node3.startVertex = 7;
	heapNode node4; node4.key = 8; node4.endVertex=8; node4.startVertex = 8;

	myMinHeap.insertNode(node1);
	myMinHeap.insertNode(node2);
	myMinHeap.insertNode(node3);
	myMinHeap.insertNode(node4);
	myMinHeap.print();
	
	heapNode aNode = myMinHeap.extractMin();
	cout << "Extract: " << aNode.key << endl;

	myMinHeap.print();
	*/
	
	
	long int numEdge = coundEdge(adjVertex);
	//cout << numEdge << endl;
	
	myheap myMinHeap(numEdge);
	unordered_map<long int, long int> visited;
	
	shortestPathDijsktra(adjVertex,visited,myMinHeap);
	//printMap(visited);
	printMap(visited,7);
	printMap(visited,37);
	printMap(visited,59);
	printMap(visited,82);
	printMap(visited,99);
	printMap(visited,115);
	printMap(visited,133);
	printMap(visited,165);
	printMap(visited,188);
	printMap(visited,197);
	return 0;
}