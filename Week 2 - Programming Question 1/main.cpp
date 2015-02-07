#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;




void printArray(long int * arr, long int size) {
	for (long int i = 0; i < size; i++)
		cout << arr[i] << endl;
}


void swapArr(long int * a, long int  * b) {
	long int tmp = *a;
	*a = *b;
	*b = tmp;
}

// selection sort for 2 index
void selectionSort(long int * arr, long int prevIndex, long int nextIndex) {
	
	if (arr[prevIndex] > arr[nextIndex]) {
		swapArr(&arr[prevIndex], &arr[nextIndex]);
	}

}

// pivot is first element
long int paritionFirst(long int * arr, long startIdx, long endIdx) {
	
	if (endIdx - startIdx == 0) return startIdx; // only 1 element

	if (endIdx - startIdx == 1) {
		selectionSort(arr, startIdx,endIdx);
		if (arr[startIdx] > arr[endIdx]) return endIdx;
		else return startIdx;
	}

	long int i = startIdx + 1;
	long int j = startIdx + 1;

	long int pivot = arr[startIdx];

	while (j <= endIdx) {
		if (arr[j] < pivot) {
			swapArr(&arr[i++], &arr[j++]);
			//j++;
			//i++;
		} else {
			j++;
			
		}

	}
	swapArr(&arr[startIdx],&arr[i-1]);

	return i-1;
}

int getMiddleIndex(long int startIndex, long int endIndex) {
	return ( startIndex + (endIndex - startIndex) / 2);

}

int getMedianIndex(long int * arr, long int startIndex, long int endIndex) {
	long int middleIndex = getMiddleIndex(startIndex,endIndex);

	long int medianIndex = 0;

	long int arrSort[] = {arr[startIndex], arr[middleIndex], arr[endIndex]};
	for (long int i = 0; i < 3-1; i++) {
		for (long int j = i; j < 3; j++) {
			if (arrSort[i] < arrSort[j]) {
				swapArr(&arrSort[i], &arrSort[j]);
			}
		}
	}

	if (arr[middleIndex] == arrSort[1]) {
		medianIndex = middleIndex;
	} else if (arr[startIndex] == arrSort[1]) {
		medianIndex = startIndex;
	} else {
		medianIndex = endIndex;
	}

	return medianIndex;
}


void quickSortMedian(long int * arr, long int startIndex, long int endIndex, long int &count) {
	if (startIndex >= endIndex) return;
	
	count += (endIndex - startIndex);

	long int medianIndex = getMedianIndex(arr, startIndex, endIndex);

	swapArr(&arr[startIndex], &arr[medianIndex]);
	long int paritionByFirst = paritionFirst(arr,startIndex,endIndex);
	
	quickSortMedian(arr, startIndex, paritionByFirst - 1, count);
	
	quickSortMedian(arr, paritionByFirst + 1, endIndex, count);
}


void quickSortFirst(long int * arr, long int startIndex, long int endIndex, long int &count) {
	if (startIndex >= endIndex) return;
	
	count += (endIndex - startIndex);

	long int paritionByFirst = paritionFirst(arr,startIndex,endIndex);
	
	quickSortFirst(arr, startIndex, paritionByFirst - 1, count);
	
	quickSortFirst(arr, paritionByFirst + 1, endIndex, count);
}



void quickSortLast(long int * arr, long int startIndex, long int endIndex, long int &count) {
	if (startIndex >= endIndex) return;
	
	count += (endIndex - startIndex);

	swapArr(&arr[startIndex], &arr[endIndex]);
	long int paritionByFirst = paritionFirst(arr,startIndex,endIndex);
	
	quickSortLast(arr, startIndex, paritionByFirst - 1, count);
	
	quickSortLast(arr, paritionByFirst + 1, endIndex, count);
}



int main() {

	string line = "";
	long int value = INT_MIN; // 100,000

	long int size = 0;

	try {
		ifstream inputFile;

		inputFile.open("QuickSort.txt");
		if (inputFile.is_open() ) {
			while (!inputFile.eof()) {

				while(getline(inputFile,line)) {
					size++;
				}
			}
			inputFile.close();
		}

		long int *intputArr = (long int*) malloc(sizeof(long int) * size);

		inputFile.open("QuickSort.txt");
		long int atIndex = 0;

		if (inputFile.is_open() ) {
			while (!inputFile.eof()) {

				while(getline(inputFile,line)) {
					intputArr[atIndex] = stoi(line);
					atIndex++;
				}
			}
			inputFile.close();
		}

		
		//printArray(intputArr,size);
		
		long int countLast = 0;
		long int countFirst = 0;
		long int countMedian = 0;
		long int startIndex = 0;
		long int endIndex = size-1;		
		
		//int partitionAt = paritionFirst(intputArr,startIndex,endIndex);
		//printArray(intputArr,size);
		//cout << "--" << partitionAt << endl;

		//quickSortFirst(intputArr,startIndex,endIndex, countFirst);

		//quickSortLast(intputArr,startIndex,endIndex, countLast);
		
		quickSortMedian(intputArr,startIndex,endIndex, countMedian);
		
		//printArray(intputArr,size);
		
		cout << "count First: " << countFirst << endl;
		cout << "count Last: " << countLast << endl;
		cout << "count Median: " << countMedian << endl;

		//cout << intputArr[getMedianIndex(intputArr, 1, 5)] << endl;
	} catch(exception ex) {
		cout << ex.what() << endl;
	}


	return 0;
}