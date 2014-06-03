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

#ifndef CONSTS_H_
#define CONSTS_H_

#include "TimeMs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

/**
 * LOG defines whether debugging information have to be printed during exeucution or not
 * LOG = 1 prints debugging information
 * LOG = 0 does not print debugging information
 */
#define LOG 0

/**
 * MP_MODE defines how multithreading is implemented.
 * MP_MODE = MP_COPY makes the program copy packets to be processed before passing them to the different processing thread.
 * MP_MODE = MP_LOCK allows all thread to share a common copy of the packets to be processed. Locking is used for mutual exclusion.
 */

#define MP_COPY 0
#define MP_LOCK 1
#define MP_MODE MP_COPY

#define MAX_RULE_FIELDS 10
#define NAME "Name"
#define NAME_LEN 32

#define STRING_VAL_LEN 32

/**
 * Kinds of packets
 */
enum PktType {
	PUB_PKT=0,
	RULE_PKT=1,
	SUB_PKT=2,
	ADV_PKT=3,
	JOIN_PKT=4
};

/**
 * Kinds of types for the values of attributes and contraints
 */
enum ValType {
	INT=0,
	FLOAT=1,
	BOOL=2,
	STRING=3
};

/**
 * Kinds of compositions
 */
enum CompKind {
	EACH_WITHIN=0,
	FIRST_WITHIN=1,
	LAST_WITHIN=2,
	ALL_WITHIN=3
};

/**
 * Kinds of predicates
 */
//enum PredKind {
//	STD=0,
//	KB=1
//};

/**
 * Operations used in constraints
 */
enum Op {
	EQ=0,
	LT=1,
	GT=2,
	DF=3,
	IN=4
};

#define OP_NUM 5

/**
 * Aggregate functions defined
 */
enum AggregateFun {
	NONE=0,
	AVG=1,
	COUNT=2,
	MIN=3,
	MAX=4,
	SUM=5
};

/**
 * Type of the state
 */
enum StateType {
	STATE=0,
	NEG=1,
	AGG=2,
	KB=3
};

/**
 * A simple attribute
 */
typedef struct EventAttribute {
	char name[NAME_LEN];
	ValType type;
	int intVal;
	float floatVal;
	bool boolVal;
	char stringVal[STRING_VAL_LEN];
} Attribute;

/**
 * A simple constraint
 */
typedef struct EventConstraint {
	char name[NAME_LEN];
	Op op;
	ValType type;
	int intVal;
	float floatVal;
	bool boolVal;
	char stringVal[STRING_VAL_LEN];

	bool operator==(const EventConstraint &x) const {
		if (op!=x.op) return false;
		if (type!=x.type) return false;
		if (strcmp(name, x.name)!=0) return false;
		if (type==INT && intVal!=x.intVal) return false;
		if (type==FLOAT && floatVal!=x.floatVal) return false;
		if (type==BOOL && boolVal!=x.boolVal) return false;
		if (type==STRING && strcmp(stringVal, x.stringVal)!=0) return false;
		return true;
	}
} Constraint;

/**
 * Represents a parameter constraint.
 * It requires the value of name1 in predicate evIndex1
 * to be equal to the value of name2 in predicate evIndex2.
 */
typedef struct ParameterConstraints {
	int evIndex1;							// Index of the first state involved in the constraint
	char name1[NAME_LEN];			// Name of the first attribute involved in the constraint
	int evIndex2;							// Index of the second state involved in the constraint
	char name2[NAME_LEN];			// Name of the second attribute involved in the constraint
	StateType type;						// Decides whether the second index refers to a normal state, to an aggregate, to a negation or to a KB state
} Parameter;

/**
 * Represents a negation. It means: no event of type eventId between constraints identified by lowerId and upperId
 */
typedef struct Neg {
	int eventType;						// Type of the event
	Constraint *constraints;	// Constraints on event content
	int constraintsNum;				// Number of constraints
	int lowerId;							// Id of the lower bound constraint (-1 if the lower bound is time based)
	TimeMs lowerTime;					// Time of the lower bound
	int upperId;							// Id of the upper bound constraint
} Negation;

/**
 * Represents an aggregate.
 * It means: take all event of type eventId between constraints identified by lowerId and upperId
 * and compute the given function to the values of the attribute with the given name.
 */
typedef struct Agg {
	int eventType;						// Type of the event
	Constraint *constraints;	// Constraints on event content
	int constraintsNum;				// Number of constraints
	int lowerId;							// Id of the lower bound constraint (-1 if the lower bound is time based)
	TimeMs lowerTime;					// Time of the lower bound
	int upperId;							// Id of the upper bound constraint
	AggregateFun fun;					// Aggregate function
	char name[NAME_LEN];			// Name of the attribute to use for computation
} Aggregate;

/**
 * Represents a parameter constraint shared between more than one sequence PANIGATI
 */
typedef struct ExtParam {
	int seqId1;
	int evIndex1;
	char name1[NAME_LEN];
	int seqId2;
	int evIndex2;
	char name2[NAME_LEN];
} ExtParameter;


#endif
