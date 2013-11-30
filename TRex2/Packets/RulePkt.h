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

#ifndef RULEPKT_H_
#define RULEPKT_H_

#include "../Common/Consts.h"
#include "../Common/TimeMs.h"
#include "../Common/CompositeEventTemplate.h"
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"
#include "../../rdf3x-0.3.5/include/rts/operator/RDFQuery.hpp"
#include <set>
#include <map>
#include <string>

/**
 * A basic event predicate
 */
typedef struct EventPredicate {
	int eventType;						// Type of the event required by this predicate
	Constraint *constraints;	// Predicate constraints
	int constraintsNum;				// Number of constraints in the predicate
	int refersTo;							// Index of the reference predicate (-1 if root)
	TimeMs win;								// Detection time window
	CompKind kind;						// The kind of constraint
} Predicate;

/**
 * A basic KB predicate - Panigati
 */
typedef struct KnowledgeBasePredicate {
	int eventType;						// Type of the event required by this predicate
	Constraint *constraints;	// Predicate constraints
	int constraintsNum;				// Number of constraints in the predicate
	int refersTo;							// Index of the reference predicate (-1 if root)
	//TimeMs win;								// Detection time window
	//CompKind kind;						// The kind of constraint
	Resultset rs;					// The results of the KB query
} KBPredicate;

/**
 * A RulePkt contains the definition of a composite event. More in particular, it contains
 * the pattern that must be detected and the template of the composite event to generate.
 * The pattern is expressed through a set of predicates, a set of negations, a set of aggregates,
 * and a set of parameters.
 */
class RulePkt {
public:

	RulePkt(bool resetCount);

	virtual ~RulePkt();

	/**
	 * Adds the root predicate.
	 * Returns false if an error occurs
	 */
	bool addRootPredicate(int eventId, Constraint *constr, int constrLen);

	/**
	 * Adds a predicate; root predicate must be already defined.
	 * Returns false if an error occurs.
	 */
	bool addPredicate(int eventType, Constraint *constr, int constrLen, int refersTo, TimeMs &win, CompKind kind);


	/**
	 * Adds a KB predicate; root predicate must be already defined.
	 * Returns false if an error occurs.
	 */
	bool addKBPredicate(int eventType, Constraint *constr, int constrLen, int refersTo, std::string kb, std::string query);

	/**
	 * Adds a new time based negation.
	 * It asks no events of type eventType satisfying all constraints to occur within win from the state with id referenceId.
	 * Returns false if an error occurs.
	 */
	bool addTimeBasedNegation(int eventType, Constraint *constraints, int constrLen, int referenceId, TimeMs &win);

	/**
	 * Adds a negation between two states.
	 * It asks no events of type eventType satisfying all constraints to occur between the states with ids id1 and id2.
	 * Returns false if an error occurs.
	 */
	bool addNegationBetweenStates(int eventType, Constraint *constraints, int constrLen, int id1, int id2);

	/**
	 * Adds a new parameter between states.
	 * It asks the value of the attribute having name name1 in the event used for id id1 to be
	 * equal to the value of the attribute having name name2 in the event used for id id2.
	 */
	bool addParameterBetweenStates(int id1, char *name1, int id2, char *name2);

	/**
	 * Adds a new parameter between a state and a negation.
	 * It asks the value of the attribute having name name in the event used for id to be
	 * equal to the value of the attribute having name negName in the event used for negation negId.
	 */
	bool addParameterForNegation(int id, char *name, int negId, char *negName);

	/**
	 * Adds a new parameter between a state and an aggregate.
	 * It asks the value of the attribute having name name in the event used for id to be
	 * equal to the value of the attribute having name aggName in the event used for aggregate aggId.
	 */
	bool addParameterForAggregate(int id, char *name, int aggId, char *aggName);

	/**
	 * Adds a new time based aggregate to compute the given function fun using a set of values.
	 * In particular, it uses all the values of attributes having the given name coming from events having the
	 * given eventType and satisfying all constraints, occurring within win from the event used for the referenceId.
	 */
	bool addTimeBasedAggregate(int eventType, Constraint *constraints, int constrLen, int referenceId, TimeMs &win, char *name, AggregateFun fun);

	/**
	 * Adds a new aggregate to compute the given function fun using a set of values.
	 * In particular, it uses all the values of attributes having the given name coming from events having the
	 * given eventType and satisfying all constraints, occurring within the events used for the ids id1 and id2.
	 */
	bool addAggregateBetweenStates(int eventType, Constraint *constraints, int constrLen, int id1, int id2, char *name, AggregateFun fun);

	/**
	 * Adds a consuming clause for the given eventIndex.
	 * Returns false if an error occurs.
	 */
	bool addConsuming(int eventIndex);

	/**
	 * Sets the template for the composite event generated by the rule
	 */
	void setCompositeEventTemplate(CompositeEventTemplate *parCeTemplate) { ceTemplate = parCeTemplate; }

	/**
	 * Fills leaves with the set of indexes that are leaves in the ordering graph
	 */
	void getLeaves(std::set<int> &leaves);

	/**
	 * Fills joinPoints with the set of indexes that are shared among more than one sequence
	 */
	void getJoinPoints(std::set<int> &joinPoints);

	/**
	 * Returns the number of predicates in the rule
	 */
	int getPredicatesNum() { return predicates.size(); }

	/**
	 * Returns the number of parameters in the rule
	 */
	int getParametersNum() { return parameters.size(); }

	/**
	 * Returns the number of negations in the rule
	 */
	int getNegationsNum() { return negations.size(); }

	/**
	 * Returns the number of aggregates in the rule
	 */
	int getAggregatesNum() { return aggregates.size(); }

	/**
	 * Returns the predicate with the given index
	 */
	Predicate & getPredicate(int index) { return predicates[index]; }

	/**
	 * Returns the parameter with the given index
	 */
	Parameter & getParameter(int index) { return parameters[index]; }

	/**
	 * Returns the negation with the given index
	 */
	Negation & getNegation(int index) { return negations[index]; }

	/**
	 * Returns the aggregate with the given index
	 */
	Aggregate & getAggregate(int index) { return aggregates[index]; }

	/**
	 * Returns the set of consuming indexes
	 */
	std::set<int> getConsuming() { return consuming; }

	/**
	 * Returns the composite event template
	 */
	CompositeEventTemplate * getCompositeEventTemplate() { return ceTemplate; }

	/**
	 * Returns the rule id
	 */
	int getRuleId() { return ruleId; }

	/**
	 * Returns true if the subscription contains at least a predicate with the given eventType
	 */
	bool containsEventType(int eventType, bool includeNegations);

	/**
	 * Returns the set of all contained event types
	 */
	void getContainedEventTypes(std::set<int> &eventTypes);

	/**
	 * Get the maximum time window between the two events
	 * Requires lowerId < upperId
	 * && lowerId < getConstraintNum()
	 * && upperId < getConstraintNum()
	 */
	TimeMs getWinBetween(int lowerId, int upperId);

	/**
	 * Returns the maximum length of a sequence defined in the subscription
	 */
	TimeMs getMaxWin();

	/**
	 * Returns true if id1 is directly defined through id2, or viceversa
	 */
	bool isDirectlyConnected(int id1, int id2);

	/**
	 * Returns true if id1 is defined through id2, or viceversa
	 */
	bool isInTheSameSequence(int id1, int id2);

	/**
	 * Overloading
	 */
	bool operator<(const RulePkt &pkt) const;
	bool operator==(const RulePkt &pkt) const;
	bool operator!=(const RulePkt &pkt) const;

private:

	static int lastId;											// Last used rule identifier
	int ruleId;															// Identifier of the rule
	std::map<int, Predicate> predicates;		// Array of event predicates
	std::map<int, Parameter> parameters;		// Parameters between different predicates (map identifier -> data structure)
	std::map<int, Negation> negations;			// Negations defined for the rule (map identifier -> data structure)
	std::map<int, Aggregate> aggregates;		// Aggregates defined for the rule (map identifier -> data structure)
	std::set<int> consuming;								// Set of indexes of events that are consumed after detection
	CompositeEventTemplate *ceTemplate;			// Template of the composite event

	/**
	 * Adds a new negation negation.
	 * Predicates identified by lowerId and upperId must be already defined.
	 * If lowerId < 0, then the lowerTime bound is used.
	 * Returns false if an error occurs
	 */
	bool addNegation(int eventType, Constraint *constr, int constrLen, int lowerId, TimeMs &lowerTime, int upperId);

	/**
	 * Adds the parameter with the given attributes
	 * The type determines whether the second index refers to a normal state, to a negation, or to an aggregate.
	 */
	bool addParameter(int index1, char *name1, int index2, char *name2, StateType type);

	/**
	 * Adds a new aggregate.
	 * Predicates identified by lowerId and upperId must be already defined.
	 * If lowerId < 0, then the lowerTime bound is used.
	 * Returns false if an error occurs
	 */
	bool addAggregate(int eventType, Constraint *constr, int constrLen, int lowerId, TimeMs &lowerTime, int upperId, char *name, AggregateFun fun);

	/**
	 * Fills referenceCount with the number of times each index is referenced
	 */
	void getReferenceCount(std::map<int, int> &referenceCount);
};

#endif
