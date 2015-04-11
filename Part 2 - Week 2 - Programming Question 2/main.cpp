#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>      // std::stringstream
#include <unordered_set>


using namespace std;

long int ConvertBitToInt(const string& bitArr) {
	long int value = 0;

	for (auto it = bitArr.rbegin(); it != bitArr.rend(); it++) {
		value += pow(2,it-bitArr.rbegin()) * (*it - '0');

	}

	return value;
}


void ReadFile(string filename, unordered_set<long int> nodes) {
	string strNumberOfNodes;
	long int numberOfNodes;

	string strNumberBitsPerNode;
	long int numberBitsPerNode;

	ifstream inputFile(filename);
	stringstream buffer;
	
	string firstBit;
	string nextBit;
	string arrBit;

	try {
		buffer << inputFile.rdbuf();
		
		buffer >> strNumberOfNodes >> strNumberBitsPerNode;
		numberOfNodes = stol(strNumberOfNodes);
		numberBitsPerNode = stol(strNumberBitsPerNode);
		
		while (buffer >> firstBit) { // see if first bit is available
			arrBit.append(nextBit);

			for ( int i = 1; i <= numberOfNodes - 1; i++) {
				buffer >> nextBit;
				arrBit.append(nextBit);
			}


		}

		
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}

int main() {

	string bitArray = "111000001101001111001111";
	long int value = ConvertBitToInt(bitArray);
	cout << value << endl;

	return 0;
}