#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream> 
#include <stdlib.h>
#include <time.h>
#include <cmath>


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

struct FindClause {
	long itemValue;
	vector<long> vClause;

	FindClause(long ItemValue) {
		itemValue = ItemValue;
	}
};

void ReadFile(string filename,vector<Clause>& iClause, long& numberVariable) {
	string strNumPoint;
	string strX;
	string strY;

	ifstream inputFile(filename);
	stringstream buffer;

	long int vertexCount = 0;

	try {
		buffer << inputFile.rdbuf();
		buffer >> strNumPoint;

		numberVariable = stol(strNumPoint);
		
		while(buffer >> strX >> strY) {
			long X = stol(strX);
			long Y = stol(strY);
			bool active = true;

			Clause newClause = Clause(X,Y,active);
			iClause.emplace_back(newClause);
		}
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}


void ManipulateMap(long item, long vectorIndex, map<long, vector<FindClause> >& mElement) {
	long absItem = abs(item);

	auto itMapFind = mElement.find(absItem);

	if (itMapFind == mElement.end()) {
		vector<FindClause> vNewElement;
		
		FindClause newFC = FindClause(item);
		
		newFC.vClause.emplace_back(vectorIndex);

		vNewElement.emplace_back(newFC);
		mElement.emplace(absItem,vNewElement);
	} else {
		bool hasElement = false;
		long vElementIndex = 0;

		vector<FindClause>& vElement = itMapFind->second;		

		for (auto iVElement = vElement.begin(); iVElement != vElement.end(); iVElement++) {
			if (iVElement->itemValue  == item) {
			
				hasElement = true;

				//add ClauseIndex
				vElement[vElementIndex].vClause.emplace_back(vectorIndex);
			}
			vElementIndex++;
		}

		if (!hasElement) {
			FindClause newFC = FindClause(item);
			
			newFC.vClause.emplace_back(vectorIndex);

			vElement.emplace_back(newFC);
		}
	}
}
// if bigger than
void BuildMapElement(vector<Clause>& iClause, map<long,vector<FindClause> >& mElement) {

	long vectorIndex = 0;

	for (auto it = iClause.begin(); it != iClause.end(); it++) {
		if (it->active) {
			
			ManipulateMap(it->first, vectorIndex, mElement);
			ManipulateMap(it->second,vectorIndex, mElement);
		}
		vectorIndex++;
	}
}


void CopyClause(vector<Clause>& iClause, vector<Clause>& oClause) {
	 
	oClause.clear();
	for (auto itC = iClause.begin(); itC != iClause.end(); itC++) {
		if (itC->active) {
			Clause newClause = Clause(itC->first,itC->second, itC->active);
			oClause.emplace_back(newClause);
		}
	}
}


void PreProcessing(vector<Clause>& iClause, map<long,vector<FindClause> >& mElement, vector<Clause> &oClause) {
	
	long atLoop = 0;
	while (true) {
		bool isMoreReduce = false;
		
		mElement.clear();
		
		atLoop++;
		cout << "Build Map at Index " << atLoop << endl;

		BuildMapElement(iClause,mElement); // create new map
		
		for (auto mItem = mElement.begin(); mItem != mElement.end(); mItem++) {
			if (mItem->second.size() == 1) { // only have one size value
				isMoreReduce = true;
				
				long itemValue = mItem->second.begin()->itemValue;
				vector<long>& itemAtClause = mItem->second.begin()->vClause;

				for (auto itAtClause = itemAtClause.begin(); itAtClause != itemAtClause.end(); itAtClause++)
					iClause[*itAtClause].active = false;
				
			}
		}
		
		CopyClause(iClause,oClause);
		CopyClause(oClause,iClause);

		if (iClause.size() == 0) isMoreReduce = false;

		if (!isMoreReduce)
			break;
	}
	
	//for (auto itC = iClause.begin(); itC != iClause.end(); itC++) {
	//	if (itC->active) {
	//		Clause newClause(itC->first,itC->second,itC->active);
	//		oClause.emplace_back(newClause);
	//	}
	//}

}

// return -1 if all clauses got satisfied
// or return one not satisfy
int IsSatisfy2SAT(vector<Clause>& oClause,map<long,bool>& variableValue) { 
	int retValue = -1;
	int atClauseIndex = 0;

	vector<int> vUnsatisfy;

	for (auto itC = oClause.begin(); itC != oClause.end(); itC++) {
		auto itF = variableValue.find(itC->first);	
		auto itS = variableValue.find(itC->second);

		if (!(itF->second || itS->second)) {
			vUnsatisfy.emplace_back(atClauseIndex);
		}
		atClauseIndex++;
	}

	if (vUnsatisfy.size() == 0)
		return retValue;
	
	int retValueIndex = (rand() % vUnsatisfy.size());

	retValue = vUnsatisfy[retValueIndex];

	return retValue;
}


bool RandomBool() {
  return rand() % 2 == 1;
}


void FlipValue(vector<Clause>& oClause, int atIndex,map<long,bool>& variableValue) {
	bool chooseFirst = RandomBool();

	long valueToFlip = LONG_MAX;
	long nValueToFlip = LONG_MIN;

	if (chooseFirst) {
		valueToFlip = oClause[atIndex].first;
		nValueToFlip = 0 - valueToFlip;
	} else {
		valueToFlip = oClause[atIndex].second;
		nValueToFlip = 0 - valueToFlip;
	}

	auto itPos = variableValue.find(valueToFlip);
	auto itNPost = variableValue.find(nValueToFlip);

	itPos->second = !(itPos->second);
	itNPost->second = !(itNPost->second);

}

bool RunInnerPapaAlogorithm(vector<Clause>& oClause,  map<long,vector<FindClause> >& mElement,map<long,bool>& variableValue) {

	long long nLoop = 2 * mElement.size() * mElement.size() + 1;

	for (long i = 1; i <= nLoop; i++) {
		
		if (i % 1000 == 0)
			cout << "Papa innrer at x 1000 : " << i << endl;
		
		int unSatisClause = IsSatisfy2SAT(oClause,variableValue);
		
		if (unSatisClause == -1)
			return true;

		FlipValue(oClause,unSatisClause,variableValue);
	}

	return false;

}

bool RunPapaAlogorithm(vector<Clause>& oClause,  map<long,vector<FindClause> >& mElement) {

	bool satis = false;

	map<long,bool> variableValue;

	int nLoop = log(mElement.size()) / log(2);

	// init
	

	for (int i = 0; i < nLoop;i++) {
		variableValue.clear();
		for (auto mItem = mElement.begin(); mItem != mElement.end(); mItem++) {
			long varIndex = mItem->first;
			bool varValue = RandomBool();

			long nVarIndex = 0 - varIndex;
			bool nVarValue = !varValue;

			variableValue.emplace(varIndex,varValue);
			variableValue.emplace(nVarIndex,nVarValue);
		}
		
		cout << "Papa outer at: " << i << endl;
		satis = RunInnerPapaAlogorithm(oClause,mElement,variableValue);
		if (satis) return true;
	}

	return satis;
}

int main() {

	/* initialize random seed: */
	srand ( time(0) );

	long numberVariable;
	vector<Clause> iClause;
	map<long,vector<FindClause> > mElement;
	vector<Clause> oClause;

	ReadFile("2sat1.txt",iClause,numberVariable);
	PreProcessing(iClause,mElement,oClause);

	cout << "Number of clauses left: " << oClause.size() << endl;

	bool satis = RunPapaAlogorithm(oClause,mElement);
	cout << satis << endl;

	return 0;
}