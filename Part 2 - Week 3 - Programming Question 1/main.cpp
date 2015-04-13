#include <iostream>
#include <vector>
#include <fstream>
#include <sstream> 
#include <vector>
#include <string>

using namespace std;

struct Item {
	long long int value;
	long long int weight;

	Item(long int Value, long int Weight) {
		value = Value;
		weight = Weight;
	}
};


void ReadFile(string filename, long long int& numberItems, long long int& knapsackSize,  vector<Item>& vItem) {
	string strKnapsackSize;

	string strNumberItems;

	ifstream inputFile(filename);
	stringstream buffer;
	
	string strItemValue;
	long long int itemValue;

	string strItemWeight;
	long long int itemWeight;

	// item 0
	Item anItem0 = Item(0,0);
	vItem.emplace_back(anItem0);

	try {
		buffer << inputFile.rdbuf();
		
		buffer >> strKnapsackSize >> strNumberItems;
		knapsackSize = stoll(strKnapsackSize);
		numberItems = stoll(strNumberItems);
		
		while (buffer >> strItemValue >> strItemWeight) {
			itemValue = stol(strItemValue);
			itemWeight = stol(strItemWeight);

			Item anItem = Item(itemValue,itemWeight);
			vItem.emplace_back(anItem);
		}

		
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}


long int RunKnapsackAlgo(const long int numberItems, const long int knapsackSize, const vector<Item>& vItem,vector<long long int>& dArr) {
	long long int optimalValue = 0;
	vector<long long int> dArrTmp(dArr.begin(), dArr.end());

	long int count = 0;
	
	for(long int i = 1; i <= numberItems;i++) {
		
		count ++; 
		if ( (count % 200) == 0 ) cout << "Finish " << count << " items" << endl;
		
		for (long long int x = 0; x <= knapsackSize;x++) {
			const Item& anItem = vItem[i];

			if ( (x >= anItem.weight)  && ( (dArrTmp[x - anItem.weight] + anItem.value) > dArrTmp[x]) ) {
				dArr[x] = dArrTmp[x - anItem.weight] + anItem.value;
			}
		}
	
		for (long int i = 0; i < dArr.size(); i++) {
			dArrTmp[i] = dArr[i];
		}
	}
	
	optimalValue = dArr[knapsackSize];

	return optimalValue;
}


int main() {

	vector<Item> vItem;
	long long int numberItems;
	long long int knapsackSize;

	
	ReadFile("knapsack1.txt",numberItems,knapsackSize,vItem);
	//ReadFile("knapsack_big.txt",numberItems,knapsackSize,vItem);
	cout << "Finish Reading File ..." << endl;

	vector<long long int> dArr (knapsackSize + 1,0); // number of items and 0 value
	
	long int optimalValue = RunKnapsackAlgo(numberItems,knapsackSize,vItem,dArr);
	cout << optimalValue << endl;


	return 0;
}