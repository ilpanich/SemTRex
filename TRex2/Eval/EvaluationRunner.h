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

#ifndef EVALUATIONRUNNER_H_
#define EVALUATIONRUNNER_H_

#include <queue>
#include <pthread.h>
#include <iostream>
#include "Producer.h"
#include "../Packets/PubPkt.h"
#include "../Packets/RulePkt.h"
#include "../Engine/TRexEngine.h"
#include "ParamHandler.h"
#include "RulesGenerator.h"
#include "../Engine/ResultListener.h"
#include "../Common/Timer.h"

/**
 * Memory shared between producer and consumer of packets
 */
typedef struct ContextStruct {
	pthread_cond_t *cond;					// Condition variable
	pthread_mutex_t *mutex;				// Mutex
	Producer *p;									// Producer
	TRexEngine *engine;						// Engine
	ParamHandler *paramHandler;		// ParameterHandler
	std::queue<PubPkt *> queue;		// Queue of packets
	bool finish;									// True if the producer finished its messages
	int dropped;									// Total number of packet dropped
} Context;

/**
 * This class creates a consumer and a producer to evaluate engine performance with
 * asynchronous message passing through a queue.
 */
class EvaluationRunner {
public:

	/**
	 * Constructor
	 */
	EvaluationRunner(ParamHandler *parParamHandler, ResultListener *parResultListener);

	/**
	 * Destructor
	 */
	virtual ~EvaluationRunner();

	/**
	 * Performs an evaluation test: returns the total number of packets dropped by the producer
	 */
	int startEval();

private:
	Context context;			// Shared memory between producer and consumer

};

#endif
