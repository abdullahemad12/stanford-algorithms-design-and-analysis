#include <iostream> 
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const void printVector (vector<unsigned long int>& vec) {
	
	for (vector<unsigned long int>::iterator it = vec.begin(); it != vec.end(); it++)  {
		cout << (*it) << endl;
	}
}

void copyVector(vector<unsigned long int>& src, unsigned long int startIndex, unsigned long int endIndex, vector<unsigned long int> &dest) {
	for (unsigned long int i = startIndex; i <= endIndex; i++) {
		dest.emplace_back(src.at(i));
	}
}

unsigned long int countInversionHelper(vector<unsigned long int>& vec, unsigned long int startLeft, unsigned long int endLeft, unsigned long int startRight, unsigned long int endRight) {
	
	// create 2 temp vector
	vector<unsigned long int> left;
	vector<unsigned long int> right;
	vector<unsigned long int> merge;

	copyVector(vec,startLeft, endLeft,left);
	copyVector(vec,startRight,endRight,right);
	
	unsigned long int count = 0;
	
	/*
	printVector(left);
	cout << "-------------";
	printVector(right);
	*/
	
	unsigned long int i = 0;
	unsigned long int j = 0;
	unsigned long int endI = (endLeft - startLeft);
	unsigned long int endJ = (endRight - startRight);

	while ( (i <= endI) && (j <= endJ) ) {
		unsigned long int leftValue = left.at(i);
		unsigned long int rightValue = right.at(j);

		if (leftValue < rightValue) {
			merge.emplace_back(leftValue);
			i++;

		} else if (rightValue < leftValue) {
			merge.emplace_back(rightValue);
			j++;
			
			count += endI - i + 1;
		}
	}
	
	while (i <= endI) {
		merge.emplace_back(left.at(i));
		i++;
	} 

	while (j <= endJ) {
		merge.emplace_back(right.at(j));
		j++;
	}


	//printVector(merge);
	//cout << "-------------";
	//cout << count << endl;
	
	i = 0;
	
	for (unsigned long int k = startLeft; k <= endLeft; k++) {
		vec[k] = merge[i];
		i++;
	}
	for (unsigned long int k = startRight; k <= endRight; k++) {
		vec[k] = merge[i];
		i++;
	}

	return count;
	
}

void countInversion(vector<unsigned long int>& vec, unsigned long int startIndex, unsigned long int endIndex, unsigned long int& count) {
	if (startIndex >= endIndex) return;

	unsigned long int middleIndex = (startIndex + (endIndex - startIndex) /2);
	
	countInversion(vec, startIndex, middleIndex, count);
	countInversion(vec,middleIndex + 1, endIndex, count);
	
	count += countInversionHelper(vec,startIndex,middleIndex,middleIndex + 1, endIndex);
}

int main() {
	string line = "";
	unsigned long int intValue = 0;

	vector<unsigned long int> inputArr;

	try {
		ifstream inputFile;
		inputFile.open("IntegerArray.txt");

		if (inputFile.is_open() ) {
			while (!inputFile.eof()) {

				while(getline(inputFile,line)) {
					intValue = stoi(line);
					inputArr.emplace_back(intValue);
				}
			}
			inputFile.close();
		}
		
		// printVector(inputArr);
		// cout << inputArr.size() << endl;
		// int middle = (inputArr.size() - 1) / 2;
		// countInversionHelper(inputArr,0,middle, middle+ 1, inputArr.size() - 1);
		// printVector(inputArr); 

		unsigned long int count = 0;
		unsigned long int startIndex = 0;
		unsigned long int endIndex = inputArr.size() - 1;
		
		countInversion(inputArr,startIndex, endIndex, count);

		cout << count << endl;

	} catch(exception ex) {
		cout << ex.what() << endl;
	}

}