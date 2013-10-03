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

#ifndef COMPOSITEEVENTGENERATOR_H_
#define COMPOSITEEVENTGENERATOR_H_

#include "../Packets/PubPkt.h"
#include "../Common/CompositeEventTemplate.h"
#include "../Common/OpTree.h"
#include "../Common/Funs.h"
#include "../Packets/RulePktValueReference.h"
#include <vector>
#include <map>

/**
 * Partial sequence of events, as used during processing
 */
typedef struct PartialEventStruct {
	PubPkt *indexes[MAX_RULE_FIELDS];
} PartialEvent;

/**
 * A composite event generator is used to generate a new composite event
 * starting from its template and from an automaton (set of sequences)
 * satisfying all constraints in the detecting rule.
 */
class CompositeEventGenerator {
public:

	/**
	 * Constructor: defines the template to be used for composite event generation
	 */
	CompositeEventGenerator(CompositeEventTemplate *ceTemplate);

	/**
	 * Destructor
	 */
	virtual ~CompositeEventGenerator();

	/**
	 * Creates a new composite event starting from the stored template and
	 * from the set of events given as input parameter.
	 */
	PubPkt * generateCompositeEvent(PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters);

private:
	CompositeEventTemplate *ceTemplate; // Template for the composite event

	/**
	 * Computes the value of an attribute using the given sequences
	 * Requires the type to be INT
	 */
	inline int computeIntValue(PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters, OpTree *opTree);

	/**
	 * Computes the value of an attribute using the given sequences
	 * Requires the type to be FLOAT
	 */
	inline float computeFloatValue(PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters, OpTree *opTree);

	/**
	 * Computes the value of an attribute using the given sequences
	 * Requires the type to be BOOL
	 */
	inline bool computeBoolValue(PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters, OpTree *opTree);

	/**
	 * Computes the value of an attribute using the given sequences
	 * Requires the type to be STRING
	 */
	inline void computeStringValue(PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters, OpTree *opTree, char *result);

	/**
	 * Returns the value of the aggregate with the given index.
	 * Requires the index of one of the aggregates and a list packets.
	 * Returns always 0 in case no events have been stored for computing the aggregate.
	 */
	inline float computeAggregate(int index, PartialEvent *partialEvent, std::map<int, Aggregate *> &aggregates, int aggsSize[MAX_RULE_FIELDS], std::map<int, std::vector<PubPkt *> > &receivedPkts, std::map<int, std::vector<PubPkt *> > &receivedAggs, std::map<int, std::set<Parameter *> > &aggregateParameters);

	/**
	 * Returns true if the packet satisfies all parameters, and false otherwise
	 */
	inline bool checkParameters(PubPkt *pkt, PartialEvent *partialEvent, std::set<Parameter *> &parameters);

};

#endif /* COMPOSITEEVENTGENERATOR_H_ */
