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

#include "EvaluationRunner.h"

using namespace std;

EvaluationRunner::EvaluationRunner(ParamHandler *parParamHandler, ResultListener *parResultListener) {
	context.p = new Producer(parParamHandler);
	context.engine = new TRexEngine(parParamHandler->getNumProc());
	context.paramHandler = parParamHandler;

	RulesGenerator rg = RulesGenerator(parParamHandler);
	set<RulePkt *> rulePkts;
	rg.createRulePkts(rulePkts);
	for (set<RulePkt *>::iterator it=rulePkts.begin(); it!=rulePkts.end(); ++it) {
		RulePkt *pkt = *it;
		context.engine->processRulePkt(pkt);
	}

	context.engine->finalize();
	context.engine->addResultListener(parResultListener);
	context.finish = false;
}

EvaluationRunner::~EvaluationRunner() {
	delete context.p;
	delete context.engine;
}

void * producer(void *parContext) {
	Context *c = (Context *) parContext;
	ParamHandler *paramHandler = c->paramHandler;
	int queueLimit = paramHandler->getQueueSize();
	// Creates and stores publication packets
	queue<PubPkt *> pubs;
	for (int i=0; i<paramHandler->getPubNum(); i++) {
		PubPkt *pkt = c->p->createPubPkt();
		pubs.push(pkt);
	}
	// Sends packets
	int numDropped = 0;
	Timer t;
	double elapsedTime = 0;
	int extraMessagesToSend = 0;
	int sleepTime = paramHandler->getSleepTime();
	while (! pubs.empty()) {
		pthread_mutex_lock(c->mutex);
		PubPkt *pkt = pubs.front();
		pubs.pop();
		if ((int) c->queue.size()>=queueLimit) {
			delete pkt;
			numDropped++;
		} else {
			pkt->setCurrentTime();
			c->queue.push(pkt);
		}
		if (pubs.empty()) {
			c->finish = true;
			c->dropped = numDropped;
		}
		pthread_cond_signal(c->cond);
		pthread_mutex_unlock(c->mutex);
		// Needs to send messages to compensate increased sleeping time
		if (extraMessagesToSend>0) {
			extraMessagesToSend--;
			continue;
		}
		// Sleeps and computes actually sleeping time (extraMessagesToSend is used to compensate rate loss due to thread scheduling)
		t.start();
		usleep(sleepTime);
		t.stop();
		elapsedTime = t.getElapsedTimeInMicroSec();
		if (elapsedTime > sleepTime+500) {
			// TODO: remove comment to generate extraMessages
			//extraMessagesToSend = elapsedTime/sleepTime;
		}
	}
	return NULL;
}

void * consumer(void *parContext) {
	Context *c = (Context *) parContext;
	while(true) {
		pthread_mutex_lock(c->mutex);
		if (c->queue.empty()) {
			if (c->finish) {
				pthread_mutex_unlock(c->mutex);
				break;
			}
			pthread_cond_wait(c->cond, c->mutex);
		}
		PubPkt *pkt = c->queue.front();
		c->queue.pop();
		pthread_mutex_unlock(c->mutex);
		c->engine->processPubPkt(pkt);
	}
	return NULL;
}

int EvaluationRunner::startEval() {
	context.cond = new pthread_cond_t;
	context.mutex = new pthread_mutex_t;

	pthread_cond_init(context.cond, NULL);
	pthread_mutex_init(context.mutex, NULL);

	pthread_t prod;
	pthread_t cons;

	cout << endl << "### Starting evaluation ###" << endl << endl;

	if (pthread_create(&prod, NULL, producer, (void *) &context)!=0) {
		cout << "Error creating producer" << endl;
		pthread_mutex_destroy(context.mutex);
		pthread_cond_destroy(context.cond);
	}

	if (pthread_create(&cons, NULL, consumer, (void *) &context)!=0) {
		cout << "Error creating consumer" << endl;
		pthread_mutex_lock(context.mutex);
		pthread_mutex_unlock(context.mutex);
		pthread_cond_signal(context.cond);
		pthread_join(prod, NULL);
		pthread_mutex_destroy(context.mutex);
		pthread_cond_destroy(context.cond);;
		return 0;
	}

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	pthread_mutex_destroy(context.mutex);
	pthread_cond_destroy(context.cond);
	delete context.cond;
	delete context.mutex;

	cout << endl << endl << "### Evaluation finished ###" << endl << endl;
	return context.dropped;
}
