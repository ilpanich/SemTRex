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

#ifndef EVALRESULTLISTENER_H_
#define EVALRESULTLISTENER_H_

#include "../Engine/ResultListener.h"
#include <fstream>
#include <iostream>
#include <set>
#include <list>
#include <math.h>
#include "../Packets/PubPkt.h"

/**
 * The ResultListener used for evaluation: it stores the number of received complex events
 * on a local variable, and provides methods to write it on a file.
 */
class EvalResultListener: public ResultListener {
public:

	EvalResultListener();

	virtual ~EvalResultListener();

	void handleResult(std::set<PubPkt *> &genPkts, double procTime);

	/**
	 * Reset the total amount of received events
	 */
	void reset() {
		times.clear();
		detectedEvents = 0;
		processingTime = 0;
		minProcessingTime = -1;
		maxProcessingTime = -1;
		numCalls = 0;
	}

	/**
	 * Prints collected results on the given file.
	 * The label is written on the left of the written result (for faster processing).
	 * If newLine, starts writing on a new line.
	 * If printLabel, prints label before writing.
	 */
	void printToFile(int label, char const *fileName, double duration, bool newLine, bool printLabel);

	/**
	 * Prints the mean processing time.
	 * The label is written on the left of the written result (for faster processing).
	 * If newLine, starts writing on a new line.
	 * If printLabel, prints label before writing.
	 */
	void printMeanProcTime(int label, char const *fileName, bool newLine, bool printLabel);

	/**
	 * Prints the minimum processing time.
	 * The label is written on the left of the written result (for faster processing).
	 * If newLine, starts writing on a new line.
	 * If printLabel, prints label before writing.
	 */
	void printMinProcTime(int label, char const *fileName, bool newLine, bool printLabel);

	/**
	 * Prints the maximum processing time.
	 * The label is written on the left of the written result (for faster processing).
	 * If newLine, starts writing on a new line.
	 * If printLabel, prints label before writing.
	 */
	void printMaxProcTime(int label, char const *fileName, bool newLine, bool printLabel);

	/**
	 * Prints the 99th percentile of the processing time.
	 * The label is written on the left of the written result (for faster processing).
	 * If newLine, starts writing on a new line.
	 * If printLabel, prints label before writing.
	 */
	void printPercProcTime(int label, char const *fileName, bool newLine, bool printLabel);

private:
	std::list<double> times;	// List of all processing times
	long detectedEvents;			// Events detected so far
	double processingTime;		// Sum of all processing times
	double minProcessingTime;	// Minimum processing time
	double maxProcessingTime;	// Maximum processing time
	long numCalls;						// Number of function calls

};

#endif
