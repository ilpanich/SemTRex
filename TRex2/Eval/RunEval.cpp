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

#include <string>
#include <sstream>
#include "RunEval.h"
#include "RunEvalCommon.h"
#include "EvaluationRunner.h"
#include "EvalResultListener.h"
#include "ParamHandler.h"

using namespace std;
using namespace trex_testing;

void runCoreLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runCoreEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runFireFirst(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runFireLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runFireEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runRain(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runLengthLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runLengthEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runLengthFirst(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runLastConsuming(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runEachConsuming(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runNoConsumingEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runWidthLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runWidthEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runWinLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runWinEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runNumRulesLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runNumRulesEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runSelectivityLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runSelectivityEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runNumProcLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runNumProcEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runSelection(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);
void runAggregate(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener);

void trex_testing::runEval() {
	// How many runs?
	int numRuns = 10;

	ParamHandler *paramHandler = new ParamHandler();
	EvalResultListener *resultListener = new EvalResultListener;

	// What to run?
	for (int seed=1; seed<=numRuns; ++seed) {
		runCoreLast(seed, paramHandler, resultListener);
		runCoreEach(seed, paramHandler, resultListener);
		runFireFirst(seed, paramHandler, resultListener);
		runFireLast(seed, paramHandler, resultListener);
		runFireEach(seed, paramHandler, resultListener);
		runRain(seed, paramHandler, resultListener);
		runLengthLast(seed, paramHandler, resultListener);
		runLengthEach(seed, paramHandler, resultListener);
		runLengthFirst(seed, paramHandler, resultListener);
		runLastConsuming(seed, paramHandler, resultListener);
		runEachConsuming(seed, paramHandler, resultListener);
		runNoConsumingEach(seed, paramHandler, resultListener);
		runWidthLast(seed, paramHandler, resultListener);
		runWidthEach(seed, paramHandler, resultListener);
		runWinLast(seed, paramHandler, resultListener);
		runWinEach(seed, paramHandler, resultListener);
		runNumRulesLast(seed, paramHandler, resultListener);
		runNumRulesEach(seed, paramHandler, resultListener);
		runSelectivityLast(seed, paramHandler, resultListener);
		runSelectivityEach(seed, paramHandler, resultListener);
		runNumProcLast(seed, paramHandler, resultListener);
		runNumProcEach(seed, paramHandler, resultListener);
		runSelection(seed, paramHandler, resultListener);
		runAggregate(seed, paramHandler, resultListener);
	}

	delete resultListener;
	delete paramHandler;
}

// Core Test Case (Last)
void runCoreLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	for (int nc=1; nc<=5; nc++) {
		string name = "TRex2_CoreLast";
		stringstream stream;
		stream << nc;
		string stringNc = stream.str();
		name+=stringNc;
		string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
		getThroughputFile(throughputFile, name, seed);
		getDroppedFile(droppedFile, name, seed);
		getMinTimeFile(minTimeFile, name, seed);
		getMaxTimeFile(minTimeFile, name, seed);
		getMeanTimeFile(minTimeFile, name, seed);
		getPercTimeFile(minTimeFile, name, seed);
		paramHandler->setDefaultParameters();
		paramHandler->setCaseStudy(LENGTH_STUDY);
		paramHandler->setNumRules(1000);
		paramHandler->setPubNum(150000);
		paramHandler->setNumRulePredicates(2);

		paramHandler->setEachPerc(0);
		paramHandler->setFirstPerc(0);
		paramHandler->setLastPerc(100);

		paramHandler->setNumDefinitions(10);
		paramHandler->setNumProc(nc);

		int maxMessagesPerSecond = 10000;
		int minMessagesPerSecond = 1000;
		int tick = 1000;

		for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
			paramHandler->setSleepTime(1000000/s);
			srand(seed);
			cout << endl << "* Msg/s -> " << s << endl;
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (s!=minMessagesPerSecond), true);
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printMinProcTime(s, minTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printPercProcTime(s, percTimeFile.data(), (s!=minMessagesPerSecond), true);
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << endl;
			file.close();
		}
	}
}

// Core Test Case (Each)
void runCoreEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	for (int nc=1; nc<=5; nc++) {
		string name = "TRex2_CoreEach";
		stringstream stream;
		stream << nc;
		string stringNc = stream.str();
		name+=stringNc;
		string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
		getThroughputFile(throughputFile, name, seed);
		getDroppedFile(droppedFile, name, seed);
		getMinTimeFile(minTimeFile, name, seed);
		getMaxTimeFile(minTimeFile, name, seed);
		getMeanTimeFile(minTimeFile, name, seed);
		getPercTimeFile(minTimeFile, name, seed);
		paramHandler->setDefaultParameters();
		paramHandler->setCaseStudy(LENGTH_STUDY);
		paramHandler->setNumRules(1000);
		paramHandler->setPubNum(10000);
		paramHandler->setNumRulePredicates(2);

		paramHandler->setEachPerc(100);
		paramHandler->setFirstPerc(0);
		paramHandler->setLastPerc(0);

		paramHandler->setNumDefinitions(10);
		paramHandler->setNumProc(nc);

		int maxMessagesPerSecond = 1000;
		int minMessagesPerSecond = 100;
		int tick = 100;

		for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
			paramHandler->setSleepTime(1000000/s);
			srand(seed);
			cout << endl << "* Msg/s -> " << s << endl;
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (s!=minMessagesPerSecond), true);
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printMinProcTime(s, minTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (s!=minMessagesPerSecond), true);
			resultListener->printPercProcTime(s, percTimeFile.data(), (s!=minMessagesPerSecond), true);
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << endl;
			file.close();
		}
	}
}

// Fire Test Case (First)
void runFireFirst(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_First";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(FIRE_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(100);
	paramHandler->setLastPerc(0);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 1000;
	int tick = 1000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=10; i<=90; i+=40) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Smoke percentage -> " << i << endl;
			paramHandler->setSmokePerc(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==10) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<90) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Fire Test Case (Last)
void runFireLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_Last";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(FIRE_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 1000;
	int tick = 1000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=10; i<=90; i+=40) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Smoke percentage -> " << i << endl;
			paramHandler->setSmokePerc(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==10) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<90) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Fire Test Case (Each)
void runFireEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_Each";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(FIRE_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(5000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumProc(5);

	int maxSleepTime = 10000;
	int minSleepTime = 1000;
	int tick = 1000;

	for (int s=maxSleepTime; s>=minSleepTime; s-=tick) {
		paramHandler->setSleepTime(s);
		for (int i=10; i<=90; i+=40) {
			srand(seed);
			cout << endl << "* Msg/s -> " << 1000000/s << " | Smoke percentage -> " << i << endl;
			paramHandler->setSmokePerc(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(1000000/s, throughputFile.data(), duration, (i==10 && s!=maxSleepTime), (i==10));
			resultListener->printMeanProcTime(1000000/s, meanTimeFile.data(), (i==10 && s!=maxSleepTime), (i==10));
			resultListener->printMinProcTime(1000000/s, minTimeFile.data(), (i==10 && s!=maxSleepTime), (i==10));
			resultListener->printMaxProcTime(1000000/s, maxTimeFile.data(), (i==10 && s!=maxSleepTime), (i==10));
			resultListener->printPercProcTime(1000000/s, percTimeFile.data(), (i==10 && s!=maxSleepTime), (i==10));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==10) {
				file << 1000000/s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<90) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Rain Test Case
void runRain(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_Rain";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(RAIN_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100); // TODO

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 30000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=1; i<=5; i+=2) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Rain percentage -> " << i << endl;
			paramHandler->setRainPerc(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==1) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Length Test Case (Last)
void runLengthLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_LengthLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=5; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Length Test Case (Each)
void runLengthEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_LengthEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 3400;
	int minMessagesPerSecond = 100;
	int tick = 300;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=5; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Length Test Case (First)
void runLengthFirst(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_LengthFirst";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(100);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=5; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Consuming Test Case (Each)
void runEachConsuming(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_EachConsuming";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(20000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	paramHandler->setConsuming(true);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 500;
	int tick = 500;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=2; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Consuming Test Case (Last)
void runLastConsuming(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_LastConsuming";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	paramHandler->setConsuming(true);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=2; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// NoConsuming Test Case (Each)
void runNoConsumingEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_NoConsumingEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(20000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	paramHandler->setConsuming(false);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 500;
	int tick = 500;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=2; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Width Test Case (Last)
void runWidthLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_WidthLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(WIDTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=5; i++) {
			srand(seed+1);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Width Test Case (Each)
void runWidthEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_WidthEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(WIDTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 3400;
	int minMessagesPerSecond = 100;
	int tick = 300;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=2; i<=5; i++) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of states -> " << i << endl;
			paramHandler->setNumRulePredicates(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==2 && s!=minMessagesPerSecond), (i==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<5) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Win Test Case (Last)
void runWinLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_WinLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);
	paramHandler->setNumRulePredicates(2);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int w=2; w<=10; w+=4) {
			srand(seed+1);
			cout << endl << "* Msg/s -> " << s << " | Average win size -> " << w << endl;
			paramHandler->setMinWinSize((w-1)*1000);
			paramHandler->setMaxWinSize((w+1)*1000);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (w==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (w<10) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Win Test Case (Each)
void runWinEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_WinEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(20000);
	paramHandler->setNumRulePredicates(2);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 5200;
	int minMessagesPerSecond = 100;
	int tick = 300;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int w=2; w<=10; w+=4) {
			srand(seed+1);
			cout << endl << "* Msg/s -> " << s << " | Average win size -> " << w << endl;
			paramHandler->setMinWinSize((w-1)*1000);
			paramHandler->setMaxWinSize((w+1)*1000);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMinProcTime(s, minTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			resultListener->printPercProcTime(s, percTimeFile.data(), (w==2 && s!=minMessagesPerSecond), (w==2));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (w==2) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (w<10) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// NumRules Test Case (Last)
void runNumRulesLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_NumRulesLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumDefinitions(100);
	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=1; i<=9; i+=2) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of rules -> " << i << endl;
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumRules(i*1000);
			paramHandler->setNumDefinitions(i*100); // To keep the same selectivity
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==1) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<9) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// NumRules Test Case (Each)
void runNumRulesEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_NumRulesEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(20000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 7000;
	int minMessagesPerSecond = 100;
	int tick = 300;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=1; i<=9; i+=2) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of rules -> " << i << endl;
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumRules(i*1000);
			paramHandler->setNumDefinitions(i*100); // To keep the same selectivity
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==1 && s!=minMessagesPerSecond), (i==1));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==1) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<9) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Selectivity Test Case (Last)
void runSelectivityLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_SelectivityLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(150000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 20000;
	int minMessagesPerSecond = 2000;
	int tick = 2000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=20; i<=100; i+=20) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of definition -> " << i << endl;
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumDefinitions(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==20) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<100) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Selectivity Test Case (Each)
void runSelectivityEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_SelectivityEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 3400;
	int minMessagesPerSecond = 100;
	int tick = 300;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=20; i<=100; i+=20) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Number of definitions -> " << i << endl;
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumDefinitions(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==20 && s!=minMessagesPerSecond), (i==20));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==20) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<100) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// NumProc Test Case (Last)
void runNumProcLast(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_NumProcLast";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);
	paramHandler->setSleepTime(40000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumProc(5);

	int minNumRules = 1000;
	int maxNumRules = 10000;
	int tick = 1000;

	for (int s=minNumRules; s<=maxNumRules; s+=tick) {
		for (int i=1; i<=8; i+=1) {
			srand(seed);
			cout << endl << "* Number of Rules -> " << s << " | Number of Processors -> " << i << endl;
			paramHandler->setNumProc(i);
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumRules(s);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==1 && s!=minNumRules), (i==1));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==1) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<8) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// NumProc Test Case (Each)
void runNumProcEach(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_NumProcEach";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(LENGTH_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(2000);
	paramHandler->setSleepTime(2000);

	paramHandler->setEachPerc(100);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(0);

	paramHandler->setNumDefinitions(100);

	int minNumRules = 100;
	int maxNumRules = 1000;
	int tick = 100;

	for (int s=minNumRules; s<=maxNumRules; s+=tick) {
		for (int i=1; i<=8; i+=1) {
			srand(seed);
			cout << endl << "* Number of Rules -> " << s << " | Number of Processors -> " << i << endl;
			paramHandler->setNumProc(i);
			paramHandler->setNumRulePredicates(2);
			paramHandler->setNumRules(s);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==1 && s!=minNumRules), (i==1));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==1 && s!=minNumRules), (i==1));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==1) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<8) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}

// Selection Test Case
void runSelection(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_Selection";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(SELECTION_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setNumProc(1);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 1000;
	int tick = 1000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		srand(seed);
		cout << endl << "* Msg/s -> " << s << endl;
		EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
		resultListener->reset();
		int dropped = runner.startEval();
		double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
		resultListener->printToFile(s, throughputFile.data(), duration, s!=minMessagesPerSecond, true);
		resultListener->printMeanProcTime(s, meanTimeFile.data(), s!=minMessagesPerSecond, true);
		resultListener->printMinProcTime(s, minTimeFile.data(), s!=minMessagesPerSecond, true);
		resultListener->printMaxProcTime(s, maxTimeFile.data(), s!=minMessagesPerSecond, true);
		resultListener->printPercProcTime(s, percTimeFile.data(), s!=minMessagesPerSecond, true);
		ofstream file;
		file.open(droppedFile.data(), ios::app);
		file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << endl;
		file.close();
	}
}

// Aggregate Test Case
void runAggregate(int seed, ParamHandler *paramHandler, EvalResultListener *resultListener) {
	string name = "TRex2_Aggregate";
	string throughputFile, droppedFile, minTimeFile, maxTimeFile, meanTimeFile, percTimeFile;
	getThroughputFile(throughputFile, name, seed);
	getDroppedFile(droppedFile, name, seed);
	getMinTimeFile(minTimeFile, name, seed);
	getMaxTimeFile(minTimeFile, name, seed);
	getMeanTimeFile(minTimeFile, name, seed);
	getPercTimeFile(minTimeFile, name, seed);
	paramHandler->setDefaultParameters();
	paramHandler->setCaseStudy(AGGREGATE_STUDY);
	paramHandler->setNumRules(1000);
	paramHandler->setPubNum(10000);

	paramHandler->setEachPerc(0);
	paramHandler->setFirstPerc(0);
	paramHandler->setLastPerc(100);

	paramHandler->setNumProc(5);

	int maxMessagesPerSecond = 10000;
	int minMessagesPerSecond = 1000;
	int tick = 1000;

	for (int s=minMessagesPerSecond; s<=maxMessagesPerSecond; s+=tick) {
		paramHandler->setSleepTime(1000000/s);
		for (int i=10; i<=90; i+=40) {
			srand(seed);
			cout << endl << "* Msg/s -> " << s << " | Smoke percentage -> " << i << endl;
			paramHandler->setSmokePerc(i);
			EvaluationRunner runner = EvaluationRunner(paramHandler, resultListener);
			resultListener->reset();
			int dropped = runner.startEval();
			double duration = ((double) paramHandler->getPubNum()*paramHandler->getSleepTime())/1000000.00;
			resultListener->printToFile(s, throughputFile.data(), duration, (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMeanProcTime(s, meanTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMinProcTime(s, minTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printMaxProcTime(s, maxTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			resultListener->printPercProcTime(s, percTimeFile.data(), (i==10 && s!=minMessagesPerSecond), (i==10));
			ofstream file;
			file.open(droppedFile.data(), ios::app);
			if (i==10) {
				file << s << "\t" << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else if (i<90) {
				file << (dropped*100/paramHandler->getPubNum()) << "\t";
			}
			else {
				file << (dropped*100/paramHandler->getPubNum()) << "\n";
			}
			file.close();
		}
	}
}
