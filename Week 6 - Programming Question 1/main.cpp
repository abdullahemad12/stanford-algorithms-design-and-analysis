#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_set>

using namespace std;


bool isItemInSet(unordered_set<long long int> & setChain, long long int  checkedNumber) {
	unordered_set<long long int>::iterator itSet = setChain.find(checkedNumber);

	if (itSet != setChain.end())
		return true;
	
	return false;

}

void printMap(unordered_map<long long int, unordered_set<long long int>> & hashTbl) {
	for (unordered_map<long long int, unordered_set<long long int>>::iterator itMap = hashTbl.begin(); itMap != hashTbl.end(); itMap++) {
		cout << itMap->first << "-->: ";
		
		for (unordered_set<long long int>::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); itSet++) {
			cout << *itSet << " " ;
		}
		cout << endl;
	}
}

void printMapToFile(unordered_map<long long int, unordered_set<long long int>> & hashTbl) {
	ofstream myfile;
	 myfile.open ("AfterBinning.csv");

	for (unordered_map<long long int, unordered_set<long long int>>::iterator itMap = hashTbl.begin(); itMap != hashTbl.end(); itMap++) {
		myfile << itMap->first << "-->: ";
		
		for (unordered_set<long long int>::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); itSet++) {
			myfile << *itSet << " " ;
		}
		myfile << endl;
	}
	myfile.close();
	return;
}

 
 
  


//void clearChain(map<long long int, set<long long int>> & hashTbl) {
//	for (map<long long int, set<long long int>>::iterator itMap = hashTbl.begin(); itMap != hashTbl.end(); itMap++) {
//		itMap->second.clear();
//	}
//}

void readFile(string filename, unordered_map<long long int,unordered_set<long long int > >  & hashTbl, unordered_set<long long int> & arrIn, int binValue) {
	string strNewNumber;
	ifstream inputFile(filename);
	stringstream buffer;
	long long int newNumber = 0;
	
	try {
		buffer << inputFile.rdbuf();
	
		while(buffer >> strNewNumber) {
			
			newNumber = stoll(strNewNumber);
			unordered_set<long long int>::iterator itSet = arrIn.find(newNumber);

			if (itSet == arrIn.end()) {
				arrIn.emplace(newNumber);
				
				unordered_map<long long int,unordered_set<long long int > >::iterator itMap = hashTbl.find(static_cast<long long int>(newNumber/binValue));
				if (itMap == hashTbl.end()) {
					unordered_set<long long int> aSet;
					aSet.emplace(newNumber);
					
					hashTbl.emplace(static_cast<long long int>(newNumber/binValue),aSet);
				} else {
					itMap->second.emplace(newNumber);
				}
				
			}
		}
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}

void insertIntoSet(unordered_set<long long int> & arrIn, long long int value, unordered_set<long long int> & arrOut, long long int halfBin) {
		for (unordered_set<long long int>::iterator itSet = arrIn.begin(); itSet != arrIn.end(); itSet++) {
			if ( (*itSet + value >= -halfBin) && (*itSet + value <= halfBin)) {
				arrOut.emplace(*itSet + value);
				//cout << *itSet << " " << value << endl;
			}
		}
}

void Count2Sum( unordered_map<long long int, unordered_set<long long int> > & hashTbl, unordered_set<long long int> & arrIn, unordered_set<long long int> & arrOut, long long int binValue) {
	
	long long int halfBin = static_cast<long long int> (binValue / 2);
	long long int count = 0;
	
	for (unordered_set<long long int>::iterator itSet = arrIn.begin(); itSet != arrIn.end(); itSet++) {
		count++;

		long long int firstBin = static_cast<long long int> ( (-*itSet - halfBin) / binValue);
		long long int lastBin = static_cast<long long int> ( (-*itSet + halfBin) / binValue);

		for (long long int bin = firstBin; bin <= lastBin; bin++) {
			unordered_map<long long int, unordered_set<long long int>> ::iterator itBin = hashTbl.find(bin);

			if (itBin != hashTbl.end()) {
				insertIntoSet(itBin->second,*itSet,arrOut,halfBin);
			}

		}
		
		if (count == 100000 != 0) cout << "First 100000 finished" << endl; 
	}

}
	
void doCal(int * arr, int size) {
	for (int i = 0; i < size; i ++) {
		cout << arr[i] << ":" << static_cast<int>(arr[i]/4) << ";";
		cout << static_cast<int>((-arr[i] - 2 ) /4)  << " " << static_cast<int> ((-arr[i] + 2) / 4);
		cout << endl;
	}

}

int main() {

	unordered_map<long long int, unordered_set<long long int> > hashTbl;
	unordered_set<long long int> arrIn;
	unordered_set<long long int> arrOut;

	//long long int binValue = 2 - (-2);
	long long int binValue = 10000 - (-10000);
	readFile("algo1-programming_prob-2sum.txt", hashTbl, arrIn, binValue);
	cout << "Finish reading file " << endl;

	//printMap(hashTbl);
	printMapToFile(hashTbl);
	
	//Count2Sum(hashTbl,arrIn, arrOut, binValue);

	//cout << arrOut.size() << endl;

	//cout << "Number of T: " << arrOut.size() << endl; 
	

	
	//int arr[] = {-10,-5,3,7,-4,2,1,6};
	//int size = sizeof(arr)/sizeof(int);
	//doCal(arr,size);
	
	return 0;
}