//
// This file is part of T-Rex, a Complex Event Processing Middleware.
// See http://home.dei.polimi.it/margara
//
// Authors: Alessandro Margara, Alberto Negrello
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

#ifndef STACKSRULE_H_
#define STACKSRULE_H_

#include "../Common/Funs.h"
#include "../Common/Consts.h"
#include "../Common/TimeMs.h"
#include "../Common/QueryItem.hpp"
#include "../Packets/PubPkt.h"
#include "../Packets/RulePkt.h"
#include "../Engine/IndexingTableCommon.h"
#include "CompositeEventGenerator.h"
#include "Stack.h"
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <vector>

#include <boost/lexical_cast.hpp>

/**
 * Represents a single detection sequence: it is represented by a set of stacks with optional negations and parameters.
 */
class StacksRule {
public:

	/**
	 * Constructor: sets the ruleID and build stacks from RulePkt passed as parameter
	 */
	StacksRule(RulePkt *pkt);

	/**
	 * Destructor
	 */
	virtual ~StacksRule();

	/**
	 * Returns the rule id
	 */
	int getRuleId() { return ruleId; }

	/**
	 * Adds the received packet to the aggregate with the given index
	 */
	void addToAggregateStack(PubPkt *pkt, int index);

	/**
	 * Adds the received packet to the negation with the given index
	 */
	void addToNegationStack(PubPkt *pkt, int index);

	/**
	 * Adds the packet to the given index.
	 * Index must be different from 0
	 */
	void addToStack(PubPkt *pkt, int index);

	/**
	 * Adds the given packet to stack 0 and starts the computation of composite events.
	 */
	void startComputation(PubPkt *pkt, std::set<PubPkt *> &results);

	/**
	 * Process pkt: used only for testing purpose
	 */
	void processPkt(PubPkt *pkt, MatchingHandler *mh, std::set<PubPkt *> &results, int index);

private:
	int ruleId;																										// The id of the rule

	std::map<int, Stack *> stacks;																// Stacks in the rule (stack id -> data structure)
	std::set<Parameter *> endStackParameters;											// Set of parameters to check at the end
	std::map<int, std::set<Parameter *> > branchStackParameters;	// Parameters in the rule to check in the meantime (stack id -> data structure)
	std::map<int, std::set<Parameter *> > negationParameters;			// Parameters in the rule to check in the meantime (negation id -> data structure)
	std::map<int, std::set<Parameter *> > aggregateParameters;		// Parameters in the rule to check in the meantime (aggregate id -> data structure)
	std::map<int, Aggregate *> aggregates;												// Aggregate in the rule (aggregate id -> data structure)
	std::map<int, Negation *> negations;													// Negations in the rule (negation id -> data structure)
	int stacksNum;																								// Number of stacks in the rule
	int aggrsNum;																									// Number of aggregates in the rule
	int negsNum;																									// Number of negations in the rule
	int kbNum;														// NUmber of KB predicates in the rule

	std::map<int, int> referenceState;														// Stack id -> state it refers to in the rule

	int stacksSize[MAX_RULE_FIELDS];															// Number of pkts stored for each stack in the rule
	int negsSize[MAX_RULE_FIELDS];																// Number of pkts stored for each negation in the rule
	int aggsSize[MAX_RULE_FIELDS];																// Number of pkts stored for each aggregate in the sequence

	std::map<int, std::vector<PubPkt *> > receivedAggs;						// Aggregate index -> set of all matching PubPkt
	std::map<int, std::vector<PubPkt *> > receivedPkts;						// Stack index -> set of all matching PubPkt
	std::map<int, std::vector<PubPkt *> > receivedNegs;						// Negation index -> set of all matching PubPkt

	std::set<int> consumingIndexes;																// Indexes of events in the consuming clause (set of stack ids)
	CompositeEventGenerator *eventGenerator;											// Used to generate composite event attributes (if any)
	int compositeEventId;																					// Used to generate a composite event id no attributes are defined

	std::map<int, QueryItem *> queryRegistry;					// Keeps the registry of the query to be executed

	/**
	 * Adds the packet to the given stack (can be a normal stack, or a stack for negations or aggregates)
	 */
	inline void parametricAddToStack(PubPkt *pkt, int &parStacksSize, std::vector<PubPkt *> &parReceived);

	/**
	 * Adds a new parameter constraint
	 */
	inline void addParameter(int index1, char *name1, int index2, char *name2, StateType type, RulePkt *pkt);

	/**
	 * Adds a new negation to negations map
	 */
	inline void addNegation(int eventType, Constraint *constraints, int constrLen, int lowIndex, TimeMs &lowTime, int highIndex);

	/**
	 * Adds a new aggregate to aggregates map
	 */
	inline void addAggregate(int eventType, Constraint *constraints, int constrLen, int lowIndex, TimeMs &lowTime, int highIndex, char *name, AggregateFun &fun);

	/**
	 * Returns the events that satisfy the stack window with the given from the given time stamp
	 */
	inline void getWinEvents(std::list<PartialEvent *> *partialEvents, int index, TimeMs tsUp, CompKind mode, PartialEvent *partialEvent);

	/**
	 * Return true for the presence of negation (according to parameters)
	 */
	inline bool checkNegation(int negIndex, PartialEvent *partialResult);

	/**
	 * Computes partial results and returns them as a list of PartialEvent.
	 */
	inline std::list<PartialEvent *> * getPartialResults(PubPkt *pkt);

	/**
	 * Computes complex events and adds them to the results set
	 */
	inline void createComplexEvents(std::list<PartialEvent *> *partialEvents, std::set<PubPkt *> &results);

	/**
	 * Removes events that have been consumed
	 */
	inline void removeConsumedEvent(std::list<PartialEvent *> *partialEvents);

	/**
	 * Deletes partial events
	 */
	inline void deletePartialEvents(std::list<PartialEvent *> *partialEvents);

	/**
	 * Returns true if the parameter is satisfied by the packet
	 */
	inline bool checkParameter(PubPkt *pkt, PartialEvent *partialEvent, Parameter *parameter);

	/**
	 * Returns true if all parameters are satisfied by the packet
	 */
	inline bool checkParameters(PubPkt *pkt, PartialEvent *partialEvent, std::set<Parameter *> &parameters);

	/**
	 * Removes partial events that do not match parameters
	 */
	inline void removePartialEventsNotMatchingParameters(std::list<PartialEvent *> *partialEvents, std::set<Parameter *> &parameters);

	/**
	 * Remove packets invalidated by timing constraints.
	 */
	inline void clearStacks();

	/**
	 * Removes all packets that are older than minTS from the given stack.
	 * The stack can be a normal stack, or a stack for negations or aggregates.
	 */
	inline void removeOldPacketsFromStack(TimeMs &minTS, int &parStacksSize, std::vector<PubPkt *> &parReceived);

};

#endif

