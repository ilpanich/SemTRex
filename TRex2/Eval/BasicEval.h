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

#ifndef BASICEVAL_H_
#define BASICEVAL_H_

#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include "../Packets/PubPkt.h"
#include "../Packets/RulePkt.h"
#include "../Engine/TRexEngine.h"
#include "../Engine/ResultListener.h"
#include "EvalResultListener.h"
#include "../Common/Timer.h"

/**
 * Basic test cases
 */
class BasicEval {
public:

	/**
	 * Constructor
	 */
	BasicEval(bool testType);

	/**
	 * Destructor
	 */
	virtual ~BasicEval();

	/**
	 * Performs an evaluation test: returns the total number of packets dropped by the producer
	 */
	int startBasicEval();

private:
	TRexEngine *engine;						// Engine

	EvalResultListener *resultListener;		// ResultListener

	bool evalType;								// TRUE if KB eval, otherwise FALSE

	void createParamRules(std::set<RulePkt *> &rules);

	void createKbRules(std::set<RulePkt *> &rules);

	PubPkt * createParamPkt1();

	PubPkt * createParamPkt2();

};

#endif
