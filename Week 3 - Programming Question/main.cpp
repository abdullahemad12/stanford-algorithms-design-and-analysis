#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
#include <stdint.h> // INT MAX
#include <limits>

using namespace std;

struct AdjacentListItem {
	vector<int> contractVertex;
	vector<int> adjacentVertex;

	vector<int> & getContractVertex() { return contractVertex; }
	vector<int> & getAdjacentVertex() { return adjacentVertex; }
};

void printContractVertex(vector<int>& vect) {
	std::cout << "(";
	for (vector<int>::iterator it = vect.begin(); it != vect.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << ") ";
}


void printAdjacentVertex(vector<int>& lst) {
	
	for (vector<int>::iterator it = lst.begin(); it != lst.end(); it++) {
		std::cout << *it << " ";
	}
	
}

void printAdjacentList(vector<AdjacentListItem>& adjacentList) {
	for (vector<AdjacentListItem>::iterator it = adjacentList.begin(); it != adjacentList.end();  it++) {
		printContractVertex(it->contractVertex);
		std::cout << "	";
		printAdjacentVertex(it->adjacentVertex);
		std::cout << endl;
	}
	std::cout << endl;
}

void printAdjacentList(vector<AdjacentListItem>& adjacentList, bool * isValid) {
	int atValid = 0;

	for (vector<AdjacentListItem>::iterator it = adjacentList.begin(); it != adjacentList.end();  it++) {
		if (isValid[atValid]) {
			printContractVertex(it->contractVertex);
			std::cout << "	";
			printAdjacentVertex(it->adjacentVertex);
			std::cout << endl;
		}
		atValid++;
	}
	std::cout << endl;
}

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


void readAdjacentList(string filename, vector<AdjacentListItem> & adjacentList) {

	string line = "";
	
	//insert dump value at position 0
	AdjacentListItem item0;
	adjacentList.emplace_back(item0);
	
	try {
		ifstream inputFile;

		inputFile.open(filename);
		if (inputFile.is_open() ) {
			while (!inputFile.eof()) {
				
				vector<string> lineItem;
				while(getline(inputFile,line)) {
					lineItem = splitString(line,'\t');

					AdjacentListItem newListItem;
					newListItem.contractVertex.push_back(stoi(*lineItem.begin()) );
					if (lineItem.size() > 1) {
						for (vector<string>::iterator it = lineItem.begin() + 1; it != lineItem.end(); it++) {
							if (!it->empty())
								newListItem.adjacentVertex.push_back(stoi(*it));
						}
					}
					adjacentList.push_back(newListItem);
				}
			}
			inputFile.close();
		}
	} catch(exception ex) {
		std::cout << ex.what() << endl;
	}
	
}




bool isValueInVector(vector<int> & vec, int value) {
	for (vector<int>::iterator itV = vec.begin(); itV != vec.end(); itV++) {
		if (*itV == value)
			return true;
	}
	return false;
}

void  copyVectorItem(vector<int> & source, vector<int> & des) {
	for (vector<int>::iterator it = source.begin(); it != source.end(); it++) {
		des.emplace_back(*it);
	}
}

void doContractVertex(vector<AdjacentListItem> & adjacentList, int i, int j) {
	vector<int> rIContractVertex;
	copyVectorItem(adjacentList.at(i).contractVertex,rIContractVertex);

	vector<int> rJContractVertex;
	copyVectorItem(adjacentList.at(j).contractVertex,rJContractVertex);

	vector<int> rIAdjacentVertex;
	copyVectorItem(adjacentList.at(i).adjacentVertex,rIAdjacentVertex);

	vector<int> rJAdjacentVertex;
	copyVectorItem(adjacentList.at(j).adjacentVertex,rJAdjacentVertex);

	//
	adjacentList[i].adjacentVertex.clear();
	adjacentList[j].adjacentVertex.clear();

	// merge contract vertex
	for (vector<int>::iterator it = rJContractVertex.begin(); it != rJContractVertex.end(); it++) {
		adjacentList[i].contractVertex.push_back(*it);
	}

	// merge adjcent vertex
	for (vector<int>::iterator it = rIAdjacentVertex.begin(); it != rIAdjacentVertex.end(); it++) {
		if (!isValueInVector(adjacentList[i].contractVertex, *it) )
			adjacentList[i].adjacentVertex.emplace_back(*it);
	}

	for (vector<int>::iterator it = rJAdjacentVertex.begin(); it != rJAdjacentVertex.end(); it++) {
		if (!isValueInVector(adjacentList[i].contractVertex, *it) )
			adjacentList[i].adjacentVertex.emplace_back(*it);
	}

	//for (vector<int>::iterator it = rJAdjacentVertex.begin(); it != rIAdjacentVertex.end(); it++) {
	//	if (!isValueInVector(adjacentList[j].contractVertex, *it) )
	//		adjacentList[j].adjacentVertex.push_back(*it);
	//}

	//for (vector<int>::iterator it = rIAdjacentVertex.begin(); it != rIAdjacentVertex.end(); it++) {
	//	if (!isValueInVector(adjacentList[j].contractVertex, *it) )
	//		adjacentList[j].adjacentVertex.push_back(*it);
	//}

	//delete rItemI;
	//delete rItemJ;
	// contract vertex
	/*for (vector<int>::iterator it = adjacentList[i].contractVertex.begin(); it != adjacentList[i].contractVertex.end(); it++) {
		newItem.contractVertex.push_back(*it);
	}

	for (vector<int>::iterator it = adjacentList[j].contractVertex.begin(); it != adjacentList[j].contractVertex.end(); it++) {
		newItem.contractVertex.push_back(*it);
	}

	for (list<int>::iterator it = adjacentList[i].adjacentVertex.begin(); it != adjacentList[i].adjacentVertex.end(); it++) {
		if (!isValueInVector(newItem.contractVertex, *it) )
			newItem.adjacentVertex.push_back(*it);
	}

	for (list<int>::iterator it = adjacentList[j].adjacentVertex.begin(); it != adjacentList[j].adjacentVertex.end(); it++) {
		if (!isValueInVector(newItem.contractVertex, *it) )
			newItem.adjacentVertex.push_back(*it);
	}*/

	/*vector<AdjacentListItem>::iterator atI = adjacentList.begin() + i;
	vector<AdjacentListItem>::iterator atJ = adjacentList.begin() + j;

	if (i > j) {
		adjacentList.erase(atI);
		adjacentList.erase(atJ);
	} else {
		adjacentList.erase(atJ);
		adjacentList.erase(atI);
	
	}*/

	//adjacentList.emplace_back(newItem);
}

int countMinCut(vector<AdjacentListItem> &adjacentList, bool* availablePickup) {
	for (int i = 0; i < adjacentList.size(); i++) {
		if (availablePickup[i])
			return adjacentList[i].adjacentVertex.size();
	}

}

int getIndex(bool * avalablePickup, int size, int k) {
	int i = 0;
	int j = 0;

	while(j < size) {
		if (avalablePickup[j] == true) {
			i++;
			if (i == k) break;
		}
		j++;
	}
	return j;
}

int sanityCheck(bool * availablePickup, int size) {
	int count = 0;

	int i = 0;

	while (i < size) {
		if (availablePickup[i] ) 
			count++;
		i++;
	}
	return count;
}


int getMinCut(vector<AdjacentListItem>& newAdjacentListItem, bool * avalablePickup) {
	
	int totalVertexToContract = newAdjacentListItem.size();
	//totalVertexToContract--;

	while(totalVertexToContract > 3) {
		
		//int firstVertex = rand() % newAdjacentListItem.size(); 
		int firstPos = rand() % totalVertexToContract;
		if (firstPos == 0) continue;
		int firstVertex = getIndex(avalablePickup,newAdjacentListItem.size(), firstPos);

		if (avalablePickup[firstVertex])  {
			
			int secondNum = rand() % newAdjacentListItem[firstVertex].adjacentVertex.size();
			int secondVertex = newAdjacentListItem[firstVertex].adjacentVertex[secondNum];

			if (avalablePickup[secondVertex]) {
				//std::cout << firstVertex << " " << secondVertex << " " << totalVertexToContract << endl;

				doContractVertex(newAdjacentListItem,firstVertex,secondVertex);
				
				totalVertexToContract--;
				
				avalablePickup[firstVertex] = true;
				avalablePickup[secondVertex] = false;

				//printAdjacentList(newAdjacentListItem, avalablePickup);
			}
			
		}
		//std::cout << "loop?  "<< firstVertex <<  endl;
	}

	if (sanityCheck(avalablePickup, newAdjacentListItem.size()) != 2) cout << "Not pass sanity check" << endl;

	return countMinCut(newAdjacentListItem,avalablePickup);
}




int getMinAllGetMinCut(vector<AdjacentListItem> &adjacentListItem) {
	vector<int> result;
	int min = INT_MAX;

	long int N = adjacentListItem.size() * adjacentListItem.size();// * (int)log(adjacentListItem.size());
	
	std::cout << " total loop: "<< N << endl;

	for (long int loop = 1; loop <= N; loop ++) {
		vector<AdjacentListItem> newAdjacentListItem(adjacentListItem);	
		bool *  availableToContract = (bool*)malloc(sizeof(bool) * newAdjacentListItem.size() );;

		availableToContract[0] = false;
		for (int i = 1; i < newAdjacentListItem.size(); i++) {
			availableToContract[i] = true;
		}

		int minCut = getMinCut(newAdjacentListItem,availableToContract);
		if (min > minCut)
			min = minCut;
		
		std::cout << " at loop: " <<  loop << " min cut value: " << minCut << " and until min: " << min << endl;

		//result.emplace_back(minCut);
		free(availableToContract);
	}

	return min;
}

int getMinVector(vector<int>& result) {
	int min = INT_MAX;
	
	for (vector<int>::iterator it = result.begin(); it != result.end(); it++) {
		if (min > *it)
			min = *it;
	}
	
	return min;
}


void printVector(vector<int>& vect) {
	
	for (vector<int>::iterator it = vect.begin(); it != vect.end(); it++) {
		std::cout << *it << " ";
	}
	
}


int main() {
	srand (time(NULL));
	string line = "";

	vector<AdjacentListItem> adjacentList;
	readAdjacentList("kargerMinCut.txt",adjacentList);

	//printAdjacentList(adjacentList);
		//getMinCut(adjacentList);
		
	int minCut = getMinAllGetMinCut(adjacentList);
		
	std::cout << minCut << endl;


	
	
	return 0;
}
	
