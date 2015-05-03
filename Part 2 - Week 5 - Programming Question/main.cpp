#include <iostream> 
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <set>

using namespace std;


struct Point {
	float x;
	float y;

	Point (float X, float Y): x(X), y(Y) {}
};


void GenerateBitMask(long int& bitMask, int numPoint) {
	while(numPoint--) 
		bitMask = (bitMask << 1) | 0x1;
}

void ReadFile(string filename, map<long int, Point>& vPoint, long int& bitMask, int& problemSize) {
	string strNumPoint;
	string strX;
	string strY;

	int numPoint = 0;

	ifstream inputFile(filename);
	stringstream buffer;

	long int vertexCount = 0;

	try {
		buffer << inputFile.rdbuf();
		buffer >> strNumPoint;

		numPoint = stoi(strNumPoint);
		problemSize = numPoint - 1;

		GenerateBitMask(bitMask,problemSize);

		while(buffer >> strX >> strY) {
			float X = stod(strX);
			float Y = stod(strY);

			Point newPoint = Point(X,Y);
			if (vPoint.size() >= 1)
				vPoint.emplace(0x01<<(vertexCount -1), newPoint); // encode vertex
			else 
				vPoint.emplace(0,newPoint);
			
			vertexCount++;
		}
	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}


int CountBitSet(long int number) {
	int count = 0;

	while(number) {
		count++;
		long int prevNum = number - 1;
		
		if (prevNum & number) 
			number = prevNum & number;
		else break;
	}

	return count;
}


float GetDistance(Point& p1, Point& p2) {
	return sqrt(pow(p1.x - p2.x,2) + pow(p1.y- p2.y,2) );
}

void GetAllPairDistance(map<long int,Point>& vPoint, map<long int, map<long int, float> >& distanceCache)  {
	
	for (auto itI = vPoint.begin(); itI != vPoint.end(); itI++) {
		
		for (auto itJ = vPoint.begin(); itJ != vPoint.end(); itJ++) {
			if (itI != itJ) {
				auto itFind = distanceCache.find(itI->first);
				float distance = GetDistance(itI->second, itJ->second);

				if (itFind != distanceCache.end()) {
					itFind->second.emplace(itJ->first, distance);

				} else {				
					map<long int,float> mapDesVertex;
					mapDesVertex.emplace(itJ->first, distance);

					distanceCache.emplace(itI->first,mapDesVertex);
				}

			}
		}
	}
}


void GenerateAllSet(map<int, vector<long int> >& allGenSet, const long int& bitMask) {
	
	// init first level
	vector<long int> lvlItem;
	lvlItem.emplace_back(0); 
	allGenSet.emplace(0,lvlItem);
	
	for (long int i = 1; i <= bitMask; i++)  {
		int numBit = CountBitSet(i);

		auto itFind = allGenSet.find(numBit);

		if (itFind != allGenSet.end() ) {
			itFind->second.emplace_back(i);
		} else {
			lvlItem.clear();
			lvlItem.emplace_back(i);

			allGenSet.emplace(numBit,lvlItem);
		}
	}
}

// matrix: column 1 means 0x01 << 1
// matrix: set S and endVertex
// vPoint: include point 0
void TSPInit(long int& bitMask, map<long int,Point>&  vPoint, vector<vector<float> >& matrix) {
	
	int numPoint = vPoint.size();

	matrix.resize(bitMask + 1);
	for (long int k = 0; k <= bitMask;k++) {
		matrix[k].resize(numPoint);
	}

	// row run	
	for (long int i = 0; i <= bitMask; i++) {
		for (long int j = 0; j < numPoint;j++) { // column run
			matrix[i][j]  = FLT_MAX;
		}			
	}
	
	matrix[0][0] = 0.0; // all zero row means only have vertex with index 0. Notice that from vertex 2 (index 1), start at 0001
}


long int GetSWithoutJ(long int& setS, long int& j) {
	return (setS ^ j);
}


// get distance from k->j
float GetVertexDistance(long int k, long int j, map<long int, map<long int, float> >& distanceCache) { 
	
	auto findK = distanceCache.find(k);
	auto findJ = findK->second.find(j);

	return findJ->second;
}

int GetColumnK(long int k) {
	int count = 0;
	while(k) {
		k = k >> 1;
		count++;
	}
	return count;
}

// bitMask all 1. Number of 1 equals to number of vertexes
float TSPDynamic(map<long int,Point> vPoint,long int& bitMask ) {

	vector<long int> setK;

	for(auto it = vPoint.begin(); it != vPoint.end(); it++) {
		setK.emplace_back(it->first);
	}
	
	vector<long int> setJ;

	for(auto it = vPoint.begin(); it != vPoint.end(); it++) {
		if (it->first != 0)
			setJ.emplace_back(it->first);
	}


	map<int, vector<long int> > allGenSet; // size -> all elements in size
	map<long int, map<long int, float> > distanceCache; // vertex --> distance to all other vertexes 

	// 7 = 111
	// 15 = 1111
	// 16777215 = 111111111111111111111111
	GenerateAllSet(allGenSet, bitMask); 
	GetAllPairDistance(vPoint,distanceCache);

	vector<vector<float> > matrix;
	TSPInit(bitMask,vPoint,matrix);
	
	allGenSet.erase(0);

	for (auto m = allGenSet.begin(); m != allGenSet.end(); m++) { //size (level)
		for (auto eachSetS = m->second.begin(); eachSetS != m->second.end(); eachSetS++) { // each set in level
			
			for (auto eachJ = setJ.begin(); eachJ != setJ.end(); eachJ++) {

				if (*eachJ & *eachSetS) { // j belongs to S and j of course != first vertex
					float minLocalK = FLT_MAX;

					for (auto eachK = setK.begin(); eachK != setK.end(); eachK++) {
						
						long int sWoutJ = GetSWithoutJ(*eachSetS,*eachJ);
						
						if (*eachK == 0) { // first vertex always belongs to S. Keep in mind that eachJ always > 0
							float distanceKJ = GetVertexDistance(*eachK,*eachJ,distanceCache);

							if (matrix[sWoutJ][0] + distanceKJ < minLocalK)
								minLocalK = matrix[sWoutJ][0] + distanceKJ;

						} else { // not first vertex
							if ( (*eachK & *eachSetS) && (*eachK != *eachJ) ) { // each k > 0 and k belongs S and k != j
								float distanceKJ = GetVertexDistance(*eachK,*eachJ,distanceCache);
								
								int columK = GetColumnK(*eachK);
								if (matrix[sWoutJ][columK] + distanceKJ < minLocalK)
									minLocalK = matrix[sWoutJ][columK] + distanceKJ;
							}
						}
					} // end of localK
					int columJ = GetColumnK(*eachJ);
					matrix[*eachSetS][columJ] = minLocalK;
				}
			}
		}

		cout << "Finish Problem Size = " << m->first << endl;
	} // problem size

	int numPoint = vPoint.size();
	float minValue = FLT_MAX;

	for (int i = 1; i < numPoint; i++) {
		if (matrix[bitMask][i] != FLT_MAX)  {
			float cij = GetVertexDistance(0x1<<(i-1),0,distanceCache);
			
			if ( (matrix[bitMask][i] + cij < minValue) )
				minValue = matrix[bitMask][i] + cij;
		}
	}
	return minValue;
}



int main() {




	long int mask = 0;
	int problemSize = 0;

	//GenNextLevel(currLevel,nextLevel,mask);

	map<long int,Point>  vPoint;
	ReadFile("tsp.txt",vPoint,mask, problemSize);

 	float minValue = TSPDynamic(vPoint,mask);
	cout << minValue << endl;

	return 0;
}