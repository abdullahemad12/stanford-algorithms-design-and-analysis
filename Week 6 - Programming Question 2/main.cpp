#include <iostream>
#include <queue>
#include<string>
#include <fstream>
#include <algorithm> // reverse
#include <sstream>      // std::stringstream
#include <functional>


using namespace std;

void readFile(string filename, vector<long int>  & elements) {
	string strNewNumber;
	ifstream inputFile(filename);
	stringstream buffer;
	long int newNumber = 0;
	
	try {
		buffer << inputFile.rdbuf();
	
		while(buffer >> strNewNumber) {
			
			newNumber = stol(strNewNumber);
			elements.push_back(newNumber);

		}
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}

}


void printVector(vector<long int> &elements) {
	for (vector<long int>::iterator itE = elements.begin(); itE != elements.end(); itE++) {
		cout << *itE << " " << endl;
	}
	cout << endl;
}


void CalMedianOfStream(vector<long int> &elements, vector<long int> & median) {
	
	priority_queue<long int, vector<long int>, greater<long int> >  minHeap;
	priority_queue<long int, vector<long int>, less<long int> >  maxHeap;

	// be sure that all elements in minHeap > all elements in maxHeap

	for (vector<long int>::iterator itE = elements.begin(); itE != elements.end(); itE++) {
		if ( (minHeap.size() == 0) &&  (maxHeap.size() == 0) ) {
			minHeap.emplace(*itE);
			median.push_back(*itE);
			continue;
		}
		else if ( (minHeap.size() != 0) &&  (maxHeap.size() == 0) ) {
			
			long long int topMinHeap = minHeap.top();

			if (*itE < topMinHeap) {
				maxHeap.emplace(*itE);
			} else {
				minHeap.emplace(*itE);
			}
		}
		else {
			long int topMinHeap = minHeap.top();
			

			if  (*itE > topMinHeap)  {
				minHeap.emplace(*itE);
			} else if (*itE < topMinHeap) {
				maxHeap.emplace(*itE);
			}
		}
	
		if (abs(static_cast<long int>(minHeap.size()) - static_cast<long int> (maxHeap.size()) )  == 2) {
			if (minHeap.size() > maxHeap.size()) {
				long int exMinHeap = minHeap.top();
					
				minHeap.pop();
				maxHeap.emplace(exMinHeap);
			} else if (minHeap.size() < maxHeap.size() ) {
				long int exMaxHeap = maxHeap.top();

				maxHeap.pop();
				minHeap.emplace(exMaxHeap);
			}
		}

		long long int topMaxHeap = maxHeap.top();
		long long int topMinHeap = minHeap.top();
			
		if (minHeap.size() == maxHeap.size()) {
			if (topMaxHeap < topMinHeap) median.push_back(topMaxHeap);
			else median.push_back(topMinHeap);
			
		} else if (minHeap.size() < maxHeap.size()) {
			median.push_back(topMaxHeap);
		
		} else if (minHeap.size() > maxHeap.size()) {
			median.push_back(topMinHeap);
		}
		
	}

	//printVector(median);

}


long long int sumMedian(vector<long int> &elements) {
	long long int sum = 0;

	for (vector<long int>::iterator itE = elements.begin(); itE != elements.end(); itE++) {
		sum += *itE;
	}
	
	sum = sum % 10000;

	return sum;
}



int main() {
	vector<long int> elements;
	vector<long int> median;

	//readFile("Median.txt", elements);
	readFile("Median.txt", elements);
	CalMedianOfStream(elements,median);

	cout << median.size() << endl;

	long long int last4Digits = sumMedian(median);
	cout << last4Digits << endl;

	return 0;
}