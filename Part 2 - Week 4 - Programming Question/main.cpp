#include <iostream>
#include <vector>
#include <fstream>
#include <sstream> 
#include <string>
#include <unordered_map>
#include <map>

using namespace std;


void ReadFile(string filename, vector<vector<long int> >& graph, long int& numberOfVertex) {
	string strNumVertex;
	string strNumEdge;

	ifstream inputFile(filename);
	stringstream buffer;

	long int numVertex;
	long int numEdge;
	
	string strStartVertex;
	string strEndVertex;
	string strCost;

	long int startVertex;
	long int endVertex;
	long int cost;


	try {
		buffer << inputFile.rdbuf();
		buffer >> strNumVertex >> strNumEdge;
		numVertex = stol(strNumVertex);
		numEdge = stol(strNumEdge);

		numberOfVertex = numVertex;

		graph.resize(numberOfVertex);
		for (int i = 0; i < numberOfVertex;i++)
			graph[i].resize(numberOfVertex);

		for (int i = 0; i < numberOfVertex;i++)
			for (int j = 0; j < numberOfVertex;j++) 
				graph[i][j] = LONG_MAX;
			


		while (buffer >> strStartVertex >> strEndVertex >> strCost) {
			startVertex = stol(strStartVertex)-1;
			endVertex = stol(strEndVertex)-1;
			cost = stol(strCost);

			graph[startVertex][endVertex] = cost;
		}
		
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}



void ReadFile(string filename, map<long int, map<long int, long int> >& graph, long int& numberOfVertex) {
	string strNumVertex;
	string strNumEdge;

	ifstream inputFile(filename);
	stringstream buffer;

	long int numVertex;
	long int numEdge;

	// vertex 0
	/*long int v0  = 0;
	unordered_map<long int, long int> vE0;
	vE0.emplace(0,LONG_MAX);

	graph.emplace(v0,vE0);
    */
	
	string strStartVertex;
	string strEndVertex;
	string strCost;

	long int startVertex;
	long int endVertex;
	long int cost;


	try {
		buffer << inputFile.rdbuf();
		buffer >> strNumVertex >> strNumEdge;
		numVertex = stol(strNumVertex);
		numEdge = stol(strNumEdge);

		numberOfVertex = numVertex;

		while (buffer >> strStartVertex >> strEndVertex >> strCost) {
			startVertex = stol(strStartVertex)-1;
			endVertex = stol(strEndVertex)-1;
			cost = stol(strCost);

			auto itFV = graph.find(startVertex);

			if (itFV == graph.end()) {
				map<long int, long int> adjV;
				adjV.emplace(endVertex,cost);
				graph.emplace(startVertex, adjV);
			} else {
				itFV->second.emplace(endVertex,cost);
			}
		}
		
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}




//bool IsAdjecentVertex(long int vj, const unordered_map<long int, long int>& edges ) {
//
//	bool isNeighbor = false;
//	auto itFind = edges.find(vj);
//
//	if (itFind != edges.end())
//		isNeighbor = true;
//
//	return isNeighbor;
//}



// return true if no negative edge cycles
bool RunFloyWarshall(vector<vector<long int> >& graph, long int numberOfVertex, long int& shortestDist) {
	
	bool negativeEdgeExits = false;
	
	
	vector<vector<vector<long int> > > p3DArr;
	
	p3DArr.resize(2); // only need to save previous result
	p3DArr.resize(numberOfVertex);
	for (long int i = 0; i < numberOfVertex; ++i) {
		p3DArr[i].resize(numberOfVertex);

		for (int j = 0; j < numberOfVertex; ++j)
			p3DArr[i][j].resize(numberOfVertex);
	 }
	
	
	// init 0 based
	for (long int i = 0; i < numberOfVertex; i++) {
		for (long int j = 0; j < numberOfVertex; j++) {
			if (i == j) 
				p3DArr[0][i][j] = 0;
			else { // i != j
				p3DArr[0][i][j] = graph[i][j];
			}
		}
	}
	


	for (long int k = 1; k < numberOfVertex; k++) {
		int currDepth = k % 2;
		int prevDepth;

		if (currDepth == 1) prevDepth = 0;
		else if (currDepth == 0) prevDepth = 1;


		for (long int i = 0; i < numberOfVertex; i++) {
		
			for (long int j = 0; j < numberOfVertex; j++) {
				   
				long int minValue = p3DArr[prevDepth][i][j];


				long int throughInternal = LONG_MAX;
			   
				if ( (p3DArr[prevDepth][i][k] != LONG_MAX) && (p3DArr[prevDepth][k][j] != LONG_MAX) ) 
					throughInternal = p3DArr[prevDepth][i][k]  + p3DArr[prevDepth][k][j];

				if (minValue >  throughInternal)
					minValue = throughInternal;

				p3DArr[currDepth][i][j] = minValue;
				
				if (i == j) {			
					if (p3DArr[currDepth][i][j] < 0) {
						negativeEdgeExits = true;
						return true;
					}
				}
				if (k == numberOfVertex- 1) {
					if (i !=j) {
						if (shortestDist >  p3DArr[currDepth][i][j])
							shortestDist = p3DArr[currDepth][i][j];
						}
				}
			}
			
		}
		
	}
	
   return negativeEdgeExits;
}

// return true if no negative edge cycles
bool RunFloyWarshall(map<long int, map<long int, long int> >& graph, long int numberOfVertex, long int& shortestDist) {
	
	bool negativeEdgeExits = false;
	
	/*
	// 3-d array vector
	long int ***p3DArr;

	// allocate memory
	p3DArr = new long int**[numberOfVertex];
	for (long int i = 0; i < numberOfVertex; ++i) {
		p3DArr[i] = new long int*[numberOfVertex];

		for (int j = 0; j < numberOfVertex; ++j)
			p3DArr[i][j] = new long int [numberOfVertex];
	 }
	 */
	/*
	vector<vector<vector<long int> > > p3DArr;
	
	p3DArr.resize(numberOfVertex);
	for (long int i = 0; i < numberOfVertex; ++i) {
		p3DArr[i].resize(numberOfVertex);

		for (int j = 0; j < numberOfVertex; ++j)
			p3DArr[i][j].resize(numberOfVertex);
	 }
	*/
	vector<vector<vector<long int> > > p3DArr;
	
	p3DArr.resize(2); // only need to save previous result
	p3DArr.resize(numberOfVertex);
	for (long int i = 0; i < numberOfVertex; ++i) {
		p3DArr[i].resize(numberOfVertex);

		for (int j = 0; j < numberOfVertex; ++j)
			p3DArr[i][j].resize(numberOfVertex);
	 }
	
	
	// init 0 based
	for (long int i = 0; i < numberOfVertex; i++) {
		for (long int j = 0; j < numberOfVertex; j++) {
			if (i == j) 
				p3DArr[0][i][j] = 0;
			else { // i != j
				auto itFV = graph.find(i);

				// assume that i always in graph
				if (itFV != graph.end()) {
					auto itMap = itFV->second;
					
					auto itFindMap = itMap.find(j);
					if (itFindMap != itMap.end()) { // there is an edge between i->j
						p3DArr[0][i][j] = itFindMap->second;
					} else { // no edge from i->j
						p3DArr[0][i][j] = LONG_MAX;
					}

				} else { // there is no edge from i
					//
					//throw "Error occurs. Could not find exist vertex";
					// 
					p3DArr[0][i][j] = LONG_MAX;
				}
			}
		}
	}
	


	for (long int k = 1; k < numberOfVertex; k++) {
		int currDepth = k % 2;
		int prevDepth;

		if (currDepth == 1) prevDepth = 0;
		else if (currDepth == 0) prevDepth = 1;


		for (long int i = 0; i < numberOfVertex; i++) {
		
			for (long int j = 0; j < numberOfVertex; j++) {
				   
				long int minValue = p3DArr[prevDepth][i][j];


				long int throughInternal = LONG_MAX;
			   
				if ( (p3DArr[prevDepth][i][k] != LONG_MAX) && (p3DArr[prevDepth][k][j] != LONG_MAX) ) 
					throughInternal = p3DArr[prevDepth][i][k]  + p3DArr[prevDepth][k][j];

				if (minValue >  throughInternal)
					minValue = throughInternal;

				p3DArr[currDepth][i][j] = minValue;
				
				if (k == numberOfVertex- 1) {
					if (i !=j) {
						if (shortestDist >  p3DArr[currDepth][i][j])
							shortestDist = p3DArr[currDepth][i][j];
						}
					else {
						if (!negativeEdgeExits) 
							if (p3DArr[currDepth][i][j] < 0)
								negativeEdgeExits = true;
					}	
				}
			}
			
		}
		
	}
	

	//for (long int i = 0; i < numberOfVertex; i++) {
	//	for (long int j = 0; j < numberOfVertex; j++) {   
	//		if (i !=j) {
	//			if (shortestDist >  p3DArr[numberOfVertex-1][i][j])
	//				shortestDist = p3DArr[numberOfVertex-1][i][j];
	//		}
	//		else {
	//			if (!negativeEdgeExits) 
	//				if (p3DArr[numberOfVertex-1][i][j] < 0)
	//					negativeEdgeExits = true;
	//		}
	//	}
	//}

	//for (long int i = 0; i < numberOfVertex; ++i) {
	//	for (long int j = 0; j < numberOfVertex; ++j)
	//		delete [] p3DArr[i][j];

	//	delete [] p3DArr[i];
 //  }
 //  delete [] p3DArr;

   return negativeEdgeExits;
}


int main() {
	 
	long int numberOfVertex;
	long int shortestDist = LONG_MAX;
	string filename = "t3.txt";
	//map<long int, map<long int, long int> > graph;
	vector<vector<long int> > graph;

	ReadFile(filename,graph,numberOfVertex);

	bool hasNegativeCycle = RunFloyWarshall(graph, numberOfVertex,shortestDist);
	
	return 0;
}