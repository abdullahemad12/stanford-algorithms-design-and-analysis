#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Clause {
	long first;
	long second;
	bool active;

	Clause(long First, long Second, bool Active) {
		first = First;
		second = Second;
		active = Active;
	}
};


void ManipulateMap(long item, map<long, vector<long> >& mElement) {
	auto itMapFind = mElement.find(item);

	long absItem = abs(item);

	if (itMapFind == mElement.end()) {
		vector<long> vNewElement;
		vNewElement.emplace_back(item);

		mElement.emplace(absItem,vNewElement);
	} else {
		bool hasElement = false;
		vector<long> vElement = itMapFind->second;		

		for (auto iVElement = vElement.begin(); iVElement != vElement.end(); iVElement++) {
			if (*iVElement  == item)
				hasElement = true;
		}

		if (!hasElement)
			vElement.emplace_back(item);
	}
}
// if bigger than
void BuildMapElement(vector<Clause>& iClause, map<long,vector<long> >& mElement) {

	for (auto it = iClause.begin(); it != iClause.end(); it++) {
		if (it->active) {
			ManipulateMap(it->first, mElement);
			ManipulateMap(it->second,mElement);
		}
  
	}
}

void PreProcessing(vector<Clause>& iClause, map<long,vector<long> >& mElement, vector<Clause> &oClause) {
	

	while (true) {
		bool isMoreReduce = false;
		
		mElement.clear();
		BuildMapElement(iClause,mElement); // create new map
		
		for (auto mItem = mElement.begin(); mItem != mElement.end(); mItem++) {
			if (mItem->second.size() == 1) { // only have one size value
				isMoreReduce = true;
				long itemValue = *mItem->second.begin();

				// loop all clause to remove
				for (auto itC = iClause.begin(); itC != iClause.end(); itC++) {
					if (itemValue == itC->first || itemValue == itC->second)
						itC->active = false;
				}
			}
		}
		
		if (!isMoreReduce)
			break;
	}
	
	for (auto itC = iClause.begin(); itC != iClause.end(); itC++) {
		if (itC->active) {
			Clause newClause(itC->first,itC->second,itC->active);
			oClause.emplace_back(newClause);
		}
	}

}

int main() {

	return 0;
}