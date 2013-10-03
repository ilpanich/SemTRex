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

#include "ParamHandler.h"

ParamHandler::ParamHandler() {
	setDefaultParameters();
}

ParamHandler::~ParamHandler() {
	// Nothing to do
}

void ParamHandler::setDefaultParameters() {
	numEventTypes = 10;
	numNames = 8; // Default 8
	numValues = 100;

	/** Publication properties */
	numAttributes = 3;	// Must be lower than NUM_NAMES

	/** Rules properties */
	numRules = 5000;
	numRulePredicates = 5;
	numRuleParameters = 0;
	numRuleNegations = 0;
	numConstraints = 2; // Must be lower than NUM_NAMES
	minWinSize = 14*1000;	// Milliseconds
	maxWinSize = 16*1000;	// Milliseconds
	maxAutSeqs = 2;		// 2
	eachPerc = 34;		// 34
	firstPerc = 33;		// 33
	lastPerc = 33;		// 33
	useConsuming = false;

	/** System properties */
	pubNum = 10000;			// 10000
	queueSize = 100;		// 100
	sleepTime = 10*1000; // Microseconds (default 15*1000)
	numProc = 1;

	/** Case studies */
	caseStudy = FIRE_STUDY;
	// Fire case study
	smokePerc = 10;
	numDefinitions = 10;
	// Rain case study
	rainPerc = 1;
	rainNegationWin = 1*1000;
}
