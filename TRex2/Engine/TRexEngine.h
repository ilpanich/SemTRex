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

#ifndef TREXENGINE_H_
#define TREXENGINE_H_

#include "StacksRule.h"
#include "IndexingTable.h"
#include "ResultListener.h"
#include "../Packets/RulePkt.h"
#include "../Packets/PubPkt.h"
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"
#include <pthread.h>

typedef std::map<int, StacksRule *> StacksRules;

typedef struct CacheStruct {
	unsigned char * dbId;
	unsigned char * queryId;
	Resultset rs;
} Cache;

typedef struct SharedStruct {
	pthread_cond_t *processCond;
	pthread_cond_t *resultCond;
	pthread_mutex_t *processMutex;
	pthread_mutex_t *resultMutex;
	int *stillProcessing;
	bool finish;
	int lowerBound;
	int upperBound;
	std::set<PubPkt *> result;
	MatchingHandler *mh;
	StacksRules *stacksRule;
	PubPkt *pkt;
	Cache *qResCache;
} Shared;

/**
 * This is the engine of T-Rex. It creates new automata starting from received rule packets,
 * and processes incoming publications to detect complex events.
 */
class TRexEngine {
public:

	/**
	 * Constructor
	 */
	TRexEngine(int numProc);

	/**
	 * Destructor
	 */
	~TRexEngine();

	/**
	 * Creates data structures needed for processing
	 */
	void finalize();

	/**
	 * Processes the given rule pkt
	 */
	void processRulePkt(RulePkt *pkt);

	/**
	 * Processes the given publication pkt
	 */
	void processPubPkt(PubPkt *pkt);

	/**
	 * Adds a new ResultHandler
	 */
	void addResultListener(ResultListener *resultListener) { resultListeners.insert(resultListener); }

	/**
	 *  Removes the given ResultHandler
	 */
	void removeResultListener(ResultListener *resultListener) { resultListeners.erase(resultListener); }

private:
	StacksRules *stacksRules;						 				// Stored stacks rules, sorted per rule id
	IndexingTable indexingTable; 								// The indexing table to speed up matching
	std::set<ResultListener *> resultListeners;	// Result listeners associated with the processing engine
	pthread_t *threads;													// Array of threads to use
	Shared *shared;															// Shared Memory
	Cache *qResCache;				// Query Result Cache
	int numProc;																// Number of processors to use

};

#endif
