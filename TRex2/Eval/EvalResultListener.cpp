//
// This file is part of T-Rex, a Complex Event Processing Middleware.
// See http://home.dei.polimi.it/margara
//
// Authors: Alessandro Margara
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "EvalResultListener.h"

using namespace std;

EvalResultListener::EvalResultListener() {
	detectedEvents = 0;
	processingTime = 0;
	numCalls = 0;
}

EvalResultListener::~EvalResultListener() { }

void EvalResultListener::handleResult(set<PubPkt *> &genPkts, double procTime) {
	detectedEvents += genPkts.size();
	processingTime += procTime;
	if (minProcessingTime<0) {
		minProcessingTime = procTime;
		maxProcessingTime = procTime;
	}
	if (procTime<minProcessingTime) minProcessingTime = procTime;
	if (procTime>maxProcessingTime) maxProcessingTime = procTime;
	times.push_back(procTime);
	numCalls++;
}

void EvalResultListener::printToFile(int label, char const *fileName, double duration, bool newLine, bool printLabel) {
	ofstream file;
	file.open(fileName, ios::app);
	double msgPerSec = duration==0 ? 0 : (detectedEvents/duration);
	if (newLine) file << "\n";
	if (printLabel) file << label << "\t";
	file << fixed << msgPerSec << "\t";
	file.close();
}

void EvalResultListener::printMeanProcTime(int label, char const *fileName, bool newLine, bool printLabel) {
	ofstream file;
	file.open(fileName, ios::app);
	double mean = numCalls==0 ? 0 : (processingTime/numCalls);
	if (newLine) file << "\n";
	if (printLabel) file << label << "\t";
	file << mean << "\t";
	file.close();
}

void EvalResultListener::printMinProcTime(int label, char const *fileName, bool newLine, bool printLabel) {
	ofstream file;
	file.open(fileName, ios::app);
	if (newLine) file << "\n";
	if (printLabel) file << label << "\t";
	file << minProcessingTime << "\t";
	file.close();
}

void EvalResultListener::printMaxProcTime(int label, char const *fileName, bool newLine, bool printLabel) {
	ofstream file;
	file.open(fileName, ios::app);
	if (newLine) file << "\n";
	if (printLabel) file << label << "\t";
	file << maxProcessingTime << "\t";
	file.close();
}

void EvalResultListener::printPercProcTime(int label, char const *fileName, bool newLine, bool printLabel) {
	times.sort();
	int rank = ceil(99*times.size()/100.0);
	double percentile = 0;
	int i = 0;
	for (list<double>::iterator it=times.begin(); it!=times.end(); ++it) {
		if (i==rank) {
			percentile = *it;
			break;
		} else {
			i++;
		}
	}
	ofstream file;
	file.open(fileName, ios::app);
	if (newLine) file << "\n";
	if (printLabel) file << label << "\t";
	file << percentile << "\t";
	file.close();
}
