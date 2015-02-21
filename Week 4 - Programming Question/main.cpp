#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm> // reverse
#include <sstream>      // std::stringstream

using namespace std;


void printAdjacentVertex(vector<long int>& vec) {
	
	for (vector<long int>::iterator it = vec.begin(); it != vec.end(); it++) {
		std::cout << *it << " ";
	}
	
}

void printAdjacentListFromFile(vector<vector<long int> >& adjacentList) {
	for (vector<vector<long int> >::iterator it = adjacentList.begin(); it != adjacentList.end();  it++) {
		printAdjacentVertex(*it);
		std::cout << endl;
	}
	std::cout << endl;
}

void printAdjacentList(map<long int, vector<long int> >& adjacentList) {
	for (map<long int, vector<long int> >::iterator it = adjacentList.begin(); it != adjacentList.end();  it++) {
		cout << it->first << " ---> ";
		
		printAdjacentVertex(it->second);
		std::cout << endl;
	}
	std::cout << endl;
}

void printMapTime(map<long int, long int> & timeNode) {
	for (map<long int, long int>::iterator it = timeNode.begin(); it != timeNode.end(); it++) {
		std::cout << it->first << " " << it->second << endl;
	}
	std::cout << endl;

}

void readAdjacentList(string filename, vector<vector<long int> > & graphFromFile, map<long int,bool> & visited) {

	string head, tail;
	ifstream inputFile(filename);
	stringstream buffer;
	
	try {
		buffer << inputFile.rdbuf();
	
		while(buffer >> head >> tail) {
			vector<long int> item;
			long int headV = stol(head);
			long int tailV = stol(tail);

			item.emplace_back(headV);
			item.emplace_back(tailV);

			visited.emplace(headV, false);
			visited.emplace(tailV, false);

			graphFromFile.emplace_back(item);
		}
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

	/*
	try {
		ifstream inputFile;

		while(inputFile >> head >> tail) {
				
			vector<long int> item;
			long int headV = stol(head);
			long int tailV = stol(tail);

			item.emplace_back(headV);
			item.emplace_back(tailV);

			visited.emplace(headV, false);
			visited.emplace(tailV, false);
		}

	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
	*/
}

void bulidGraph( map<long int, vector<long int> > & adjacentList, map<long int, vector<long int> > & rAdjacentList, vector<vector<long int> > & graphFromFile) {

	for (vector<vector<long int> >::iterator it = graphFromFile.begin(); it != graphFromFile.end();  it++) {
		map<long int, vector<long int > >::iterator fIt;
		map<long int, vector<long int > >::iterator rIt;

		const long int head = (*it).at(0);
		const long int tail = (*it).at(1);

		fIt = adjacentList.find(head);
		if (fIt != adjacentList.end()) {
			fIt->second.emplace_back(tail);
		}
		else {
			vector<long int> vTail; 
			vTail.emplace_back(tail);

			adjacentList.emplace(head, vTail);

			vector<long int> vHead;
			adjacentList.emplace(tail,vHead);
		}

		rIt = adjacentList.find(tail);
		if (rIt == adjacentList.end()) {
			vector<long int> vTail; 
			adjacentList.emplace(tail,vTail);
		}

		
		
		rIt = rAdjacentList.find(tail);
		if (rIt != rAdjacentList.end()) {
			rIt->second.emplace_back(head);
		}
		else {
			vector<long int> vHead; 
			vHead.emplace_back(head);

			rAdjacentList.emplace(tail, vHead);

			vector<long int> vTail;
			rAdjacentList.emplace(head,vTail);
		}
		
		rIt = rAdjacentList.find(head);
		if (rIt == rAdjacentList.end()) {
			vector<long int> vHead; 
			rAdjacentList.emplace(head,vHead);
		}
	}

	// processing blank adjacent

}

void DFSLoopOrderHelper(map<long int,vector< long int> > & rAdjacentList, map<long int, bool> & visited, long int & t, const long int & startNode, map<long int, long int> & tNode) {

	visited.at(startNode) = true; // marked visited

	map<long int, vector<long int > >::iterator start = rAdjacentList.find(startNode);

	if (start == rAdjacentList.end()) {
		cout << " Found no instance of startNode " << startNode << endl;
		return;
	} else {
		for (vector<long int>::iterator it = start->second.begin(); it !=  start->second.end();  it++) {
		
			// find outoing arc start from StartNode
			map<long int, bool>::iterator itV = visited.find(*it);
			if (itV == visited.end()) {
				cout << " Found no instance of itV" << *it << endl;
				return;		
			}

			if (!itV->second) { // not visited
				long int endPoint = itV->first;
				if ( !visited.at(endPoint) ) {
					DFSLoopOrderHelper(rAdjacentList,visited,t,endPoint,tNode);
				}
			}
		}
	}
	t++;
	tNode.emplace(t,startNode);
}


void DFSMagicOrder(map < long int, vector<long int> > & rAdjacentList, map<long int, bool> & visited, map<long int, long int> & tNode) {
	long int t = 0;

	for (map<long int, bool>::iterator it = visited.begin(); it != visited.end(); it++) {
		if (!it->second) {
			DFSLoopOrderHelper(rAdjacentList, visited,t, it->first,tNode);
		}
	}
}


void resetVisited(map<long int, bool> & visited) {
	for (map<long int, bool>::iterator it = visited.begin(); it != visited.end(); it++) {
		it->second =  false;
	}

}


void DFSLoopCountHelper(map<long int, vector< long int> > & adjacenetList, map<long int, bool> & visited, const long int & startNode, long int & currentCount) {
	visited.at(startNode) = true; // marked visited
	currentCount++;

	map<long int, vector<long int> >::iterator start = adjacenetList.find(startNode);
	if (start == adjacenetList.end()) {
		cout << " Found no instance of startNode " << startNode << endl;
		return;
	} else {	
		for (vector<long int>::iterator it = start->second.begin(); it !=  start->second.end();  it++) {
		
			// find outoing arc start from StartNode
			map<long int, bool>::iterator itV = visited.find(*it);
			if (itV == visited.end()) {
				cout << " Found no instance of itV" << *it << endl;
				return;		
			}

			if (!itV->second) { // not visited
				long int endPoint = itV->first;
				if ( !visited.at(endPoint) ) {
					DFSLoopCountHelper(adjacenetList,visited,endPoint,currentCount);
				}
			}
		}
	}
}


vector<long int> DFSSCC(map<long int, vector<long int> > & adjacentList, map<long int, bool> & visited, map<long int, long int> & tNode) {
	vector<long int> sccCount;
	long int currentSCCCount;

	resetVisited(visited);

	// DFS from biggest time
	for (map<long int, long int>::reverse_iterator it = tNode.rbegin(); it != tNode.rend(); it++) {
		map<long int, bool>::iterator itAt = visited.find(it->second);
		if (!itAt->second) {
			currentSCCCount = 0;
			DFSLoopCountHelper(adjacentList,visited,itAt->first,currentSCCCount);
			sccCount.emplace_back(currentSCCCount);
		}
	}

	return sccCount;

}

bool lessFirst (long int i,long int j) { return (i<j); }
bool lessLast (long int i,long int j) { return (i>j); }

void printMax5Vector(ofstream & myfile, vector<long int> & vt) {
	long int count = 0;
	sort(vt.begin(), vt.end(), lessLast );

	for (vector<long int>::iterator it = vt.begin(); it != vt.end(); it++) {
		cout << *it << "	";
		myfile<< *it << "	";
		count++; if (count == 5) break;
	}
}

int main() {

	map<long int,vector<long int> > adjacentList;
	map<long int, vector<long int> > rAdjacentList;
	vector<vector<long int> > graphFromFile;

	map<long int,bool>  visited;

	readAdjacentList("SCC3.txt",graphFromFile,visited);
	cout << "Finish Reading.	";
	cout << "No. Vertex: " << visited.size() << endl;
	
	bulidGraph(adjacentList,rAdjacentList,graphFromFile);
	cout << "Finish Building.	" << endl;
	//printAdjacentList(adjacentList);
	//printAdjacentList(rAdjacentList);
	
	map<long int, long int> tNode;
	resetVisited(visited);

	DFSMagicOrder(rAdjacentList, visited, tNode);
	cout << "Finish Ordering. " << endl << endl;
	//printMapTime(tNode);

	vector<long int> sccCount = DFSSCC(adjacentList,visited,tNode);
	cout << "Finish Counting. " << endl;
	
	ofstream  myfile;
	myfile.open ("result.txt");
	printMax5Vector(myfile, sccCount);
	myfile.close();
	cout << endl;
	

	return 0;
}
