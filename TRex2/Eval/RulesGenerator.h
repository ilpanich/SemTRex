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

#ifndef RULESGENERATOR_H_
#define RULESGENERATOR_H_

#include "../Packets/RulePktValueReference.h"
#include "../Common/Consts.h"
#include "../Common/TimeMs.h"
#include "../Engine/TRexEngine.h"
#include "../Packets/RulePkt.h"
#include "ParamHandler.h"
#include <set>

/**
 * This class is used to generate rules that populate the T-Rex engine.
 */
class RulesGenerator {
public:

	/**
	 * Constructor: creates a rule generator using the parameters stored in the given ParamHandler
	 */
	RulesGenerator(ParamHandler *parParamHandler);

	/**
	 * Destructor
	 */
	virtual ~RulesGenerator();

	/**
	 * Fills the rules set with the number of new packets specified in the parameter handler
	 */
	void createRulePkts(std::set<RulePkt *> &rules);

private:

	ParamHandler *paramHandler;

	/**
	 * Adds the root predicate to the given pkt
	 */
	void addRootPredicate(RulePkt *pkt);

	/**
	 * Adds a new predicate to the given pkt.
	 * If canSplit==true, the predicate can generate a new sequence inside the rule.
	 * Returns true if it has generated a new sequence.
	 */
	bool addPredicate(RulePkt *pkt, bool canSplit);

	/**
	 * Adds a parameter constraint to the given pkt
	 */
	void addParameter(RulePkt *pkt);

	/**
	 * Adds a negation to the given pkt
	 */
	void addNegation(RulePkt *pkt);

	/**
	 * Returns a random event type
	 */
	int getRandomEventType();

	/**
	 * Initializes the given set of constraints
	 */
	void initConstraints(Constraint *constraints, int constraintsNum);

	/**
	 * Returns a valid time window for a constraint
	 */
	TimeMs getWindow();

	/**
	 * Returns a valid CompKind for a predicate
	 */
	CompKind getCompKind();

	/**
	 * Creates numRules rules for the "Fire" case study
	 */
	void createFireRules(std::set<RulePkt *> &rules);

	/**
	 * Creates numRules rules for the "Rain" case study
	 */
	void createRainRules(std::set<RulePkt *> &rules);

	/**
	 * Creates numRules rules for the "Length" case study
	 */
	void createLengthRules(std::set<RulePkt *> &rules);

	/**
	 * Creates numRules rules for the "Width" case study
	 */
	void createWidthRules(std::set<RulePkt *> &rules);

	/**
	 * Creates numRules rules for the "Selection" case study
	 */
	void createSelectionRules(std::set<RulePkt *> &rules);

	/**
	 * Creates numRules rules for the "Aggregate" case study
	 */
	void createAggregateRules(std::set<RulePkt *> &rules);

};

#endif
