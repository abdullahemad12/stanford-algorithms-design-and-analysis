#include <iostream>
#include <set>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Job {
	long int jobWeight;
	long int jobLength;
	long int index;

	Job::Job(long int JobWeight, long int JobLength, long int Index) {
		jobWeight = JobWeight;
		jobLength = JobLength;
		index = Index;
	}
};

struct JobCompare {
	bool operator() (const Job& lhs, const Job& rhs) {
		long int cLHS = lhs.jobWeight - lhs.jobLength;
		long int cRHS = rhs.jobWeight - rhs.jobLength;

		if (cRHS < cLHS) return true;
		else if (cRHS > cLHS) return false;
		else if (cRHS == cLHS)
			if (lhs.jobWeight > rhs.jobWeight ) return true;
			else if (lhs.jobWeight < rhs.jobWeight) return false;
			else if (lhs.jobWeight == rhs.jobWeight) return (lhs.index > rhs.index);
	}
};


struct JobOptimalCompare {
	bool operator() (const Job& lhs, const Job& rhs) {
		double cLHS = static_cast<double>(lhs.jobWeight) / static_cast<double>(lhs.jobLength) ;
		double cRHS = static_cast<double>(rhs.jobWeight) / static_cast<double>(rhs.jobLength );

		if (cRHS - cLHS < 0) return true;
		else if (cRHS - cLHS > 0) return false;
		else if ( abs(cRHS - cLHS) < std::numeric_limits<double>::epsilon() )
			if (lhs.jobWeight > rhs.jobWeight ) return true;
			else if (lhs.jobWeight < rhs.jobWeight) return false;
			else if (lhs.jobWeight == rhs.jobWeight) return (lhs.index > rhs.index);
	}
};

void ReadFile(string filename, long int& n, set<Job, JobCompare> & scheduleJob, set <Job, JobOptimalCompare> & opScheduleJob) {
	ifstream inputFile(filename);
	stringstream buffer;
	
	string strnNumJob;
	string strJobWeight;
	string strJobLength;

	long int index = 0;

	try {
		buffer << inputFile.rdbuf();
		buffer >> strnNumJob;

		n = stol(strnNumJob);

		while (buffer >> strJobWeight >> strJobLength) {
			index++;

			long int jobWeight = stol(strJobWeight);
			long int jobLength = stol(strJobLength);

			//Job aNewJob 
			Job aNewJob = Job(jobWeight, jobLength, index);
			auto ret = scheduleJob.emplace(aNewJob);
			/*
			if (!ret.second) 
				cout << "jobweight: " << jobWeight << " joblength: " << jobLength << endl;
			*/

			opScheduleJob.emplace(aNewJob);
		}


	}catch(exception ex) {
		std::cout << ex.what() << endl;
	}
}


long long int GetWeightedCompleteTime1(set<Job, JobCompare> & scheduleJob) {
	long long int wcTime = 0;
	long long int completeTime = 0;

	for (auto it = scheduleJob.begin(); it != scheduleJob.end(); it++) {
		completeTime += it->jobLength;
		wcTime += it->jobWeight * completeTime;
	}

	return wcTime;
}

long long int GetWeightedCompleteTime2(set<Job, JobOptimalCompare> & scheduleJob) {
	long long int wcTime = 0;
	long long int completeTime = 0;

	for (auto it = scheduleJob.begin(); it != scheduleJob.end(); it++) {
		completeTime += it->jobLength;
		wcTime += it->jobWeight * completeTime;
	}

	return wcTime;
}


int main() {
	long int n; // number of jobs;

	set<Job, JobCompare> scheduleJobs;
	set<Job, JobOptimalCompare> opScheduleJobs;

	ReadFile("jobs.txt", n, scheduleJobs,opScheduleJobs);

	long long int wcTime = GetWeightedCompleteTime1(scheduleJobs);
	cout << wcTime << endl;

	long long int wcTime2 = GetWeightedCompleteTime2(opScheduleJobs);
	cout << wcTime2 << endl;

	return 0;
}