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

#ifndef PARAMHANDLER_H_
#define PARAMHANDLER_H_

#include "../Common/TimeMs.h"

#define DEFAULT_STUDY 0
#define FIRE_STUDY 1
#define RAIN_STUDY 2
#define LENGTH_STUDY 3
#define WIDTH_STUDY 4
#define SELECTION_STUDY 5
#define AGGREGATE_STUDY 6

/**
 * This class handles parameters during evaluations
 */
class ParamHandler {
public:

	/**
	 * Empty constructor: initializes with the default parameters
	 */
	ParamHandler();

	/**
	 * Destructor
	 */
	virtual ~ParamHandler();

	/**
	 * Sets the default parameters
	 */
	void setDefaultParameters();

	/**
	 * Setter and getter methods
	 */
	int getNumEventTypes() { return numEventTypes; }
	void setNumEventTypes(int parNumEventTypes) { numEventTypes = parNumEventTypes; }

	int getNumNames() { return numNames; }
	void setNumNames(int parNumNames) { numNames = parNumNames; }

	int getNumValues() { return numValues; }
	void setNumValues(int parNumValues) { numValues = parNumValues; }

	int getNumAttributes() { return numAttributes; }
	void setNumAttributes(int parNumAttributes) { numAttributes = parNumAttributes; }

	int getNumRules() { return numRules; }
	void setNumRules(int parNumRules) { numRules = parNumRules; }

	int getNumRulePredicates() { return numRulePredicates; }
	void setNumRulePredicates(int parNumRulePredicates) { numRulePredicates = parNumRulePredicates; }

	int getNumRuleParameters() { return numRuleParameters; }
	void setNumRuleParameters(int parNumRuleParameters) { numRuleParameters = parNumRuleParameters; }

	int getNumRuleNegations() { return numRuleNegations; }
	void setNumRuleNegations(int parNumRuleNegations) { numRuleNegations = parNumRuleNegations; }

	int getNumConstraints() { return numConstraints; }
	void setNumConstraints(int parNumConstraints) { numConstraints = parNumConstraints; }

	int getMinWinSize() { return minWinSize; }
	void setMinWinSize(int parMinWinSize) { minWinSize = parMinWinSize; }

	int getMaxWinSize() { return maxWinSize; }
	void setMaxWinSize(int parMaxWinSize) { maxWinSize = parMaxWinSize; }

	int getMaxAutSeqs() { return maxAutSeqs; }
	void setMaxAutSeqs(int parMaxAutSeqs) { maxAutSeqs=parMaxAutSeqs; }

	int getEachPerc() { return eachPerc; }
	void setEachPerc(int parEachPerc) { eachPerc = parEachPerc; }

	int getFirstPerc() { return firstPerc; }
	void setFirstPerc(int parFirstPerc) { firstPerc = parFirstPerc; }

	int getLastPerc() { return lastPerc; }
	void setLastPerc(int parLastPerc) { lastPerc = parLastPerc; }

	int getPubNum() { return pubNum; }
	void setPubNum(int parPubNum) { pubNum = parPubNum; }

	int getQueueSize() { return queueSize; }
	void setQueueSize(int parQueueSize) { queueSize = parQueueSize; }

	int getSleepTime() { return sleepTime; }
	void setSleepTime(int parSleepTime) { sleepTime = parSleepTime; }

	int getNumProc() { return numProc; }
	void setNumProc(int parNumProc) { numProc = parNumProc; }

	int getCaseStudy() { return caseStudy; }
	void setCaseStudy(int parCaseStudy) { caseStudy = parCaseStudy; }

	int getSmokePerc() { return smokePerc; }
	void setSmokePerc(int parSmokePerc) { smokePerc = parSmokePerc; }

	int getNumDefinitions() { return numDefinitions; }
	void setNumDefinitions(int parNumDefinitions) { numDefinitions = parNumDefinitions; }

	int getRainPerc() { return rainPerc; }
	void setRainPerc(int parRainPerc) { rainPerc = parRainPerc; }

	TimeMs getRainNegationWin() { return rainNegationWin; }
	void setRainNegationWin(TimeMs parRainNegationWin) { rainNegationWin = parRainNegationWin; }

	bool getConsuming() { return useConsuming; }
	void setConsuming(bool parUseConsuming) { useConsuming = parUseConsuming; }

private:
	/**
	 * Content properties
	 */
	int numEventTypes;
	int numNames;
	int numValues;

	/**
	 * Publication properties
	 */
	int numAttributes;	// Must be lower than NUM_NAMES

	/**
	 * Rules properties
	 */
	int numRules;
	int numRulePredicates;
	int numRuleParameters;
	int numRuleNegations;
	int numConstraints; // Must be lower than NUM_NAMES
	int minWinSize;	// Milliseconds
	int maxWinSize;	// Milliseconds
	int maxAutSeqs;
	int eachPerc;		// Percentage of each-within
	int firstPerc;	// Percentage of first-within
	int lastPerc; 	// Percentage of last-within
	bool useConsuming;

	/**
	 * System properties
	 */
	int pubNum;
	int queueSize;
	int sleepTime; // Microseconds
	int numProc;

	/**
	 * Type of test
	 */
	int caseStudy;
	int numDefinitions;	// Number of distinct definition
	// Data for the fire case study
	int smokePerc;	// Percentage of smoke events
	// Data for the rain case study
	int rainPerc; // Percentage of rain events
	TimeMs rainNegationWin; // Negation window

};

#endif
