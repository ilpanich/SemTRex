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

#include "Unmarshaller.h"

using namespace std;

RulePkt * Unmarshaller::decodeRulePkt(char *source) {
	int index = 0;
	return decodeRulePkt(source, index);
}

PubPkt * Unmarshaller::decodePubPkt(char *source) {
	int index = 0;
	return decodePubPkt(source, index);
}

SubPkt * Unmarshaller::decodeSubPkt(char *source) {
	int index = 0;
	return decodeSubPkt(source, index);
}

AdvPkt * Unmarshaller::decodeAdvPkt(char *source) {
	int index = 0;
	return decodeAdvPkt(source, index);
}

JoinPkt * Unmarshaller::decodeJoinPkt(char *source) {
	int index = 0;
	return decodeJoinPkt(source, index);
}

int Unmarshaller::decodeInt(char *source) {
	int index = 0;
	return decodeInt(source, index);
}

PktType Unmarshaller::decodePktType(char *source) {
	int index = 0;
	return decodePktType(source, index);
}

RulePkt * Unmarshaller::decodeRulePkt(char *source, int &index) {
	int predicatesNum = decodeInt(source, index);
	map<int, Predicate> predicates;
	for (int i=0; i<predicatesNum; i++) {
		int key = decodeInt(source, index);
		Predicate value = decodeEventPredicate(source, index);
		predicates.insert(make_pair(key, value));
	}
	int parametersNum = decodeInt(source, index);
	map<int, Parameter> parameters;
	for (int i=0; i<parametersNum; i++) {
		int key = decodeInt(source, index);
		Parameter value = decodeParameter(source, index);
		parameters.insert(make_pair(key, value));
	}
	int aggregatesNum = decodeInt(source, index);
	map<int, Aggregate> aggregates;
	for (int i=0; i<aggregatesNum; i++) {
		int key = decodeInt(source, index);
		Aggregate value = decodeAggregate(source, index);
		aggregates.insert(make_pair(key, value));
	}
	int negationsNum = decodeInt(source, index);
	map<int, Negation> negations;
	for (int i=0; i<negationsNum; i++) {
		int key = decodeInt(source, index);
		Negation value = decodeNegation(source, index);
		negations.insert(make_pair(key, value));
	}
	CompositeEventTemplate *eventTemplate = decodeEventTemplate(source, index);
	int numConsuming = decodeInt(source, index);
	set<int> consuming;
	for (int i=0; i<numConsuming; i++) {
		consuming.insert(decodeInt(source, index));
	}
	RulePkt *pkt = new RulePkt(false);
	for (map<int, Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it) {
		int index = it->first;
		Predicate pred = it->second;
		if (index==0) {
			pkt->addRootPredicate(pred.eventType, pred.constraints, pred.constraintsNum);
		} else {
			pkt->addPredicate(pred.eventType, pred.constraints, pred.constraintsNum, pred.refersTo, pred.win, pred.kind);
		}
		delete pred.constraints;
	}
	for (map<int, Aggregate>::iterator it=aggregates.begin(); it!=aggregates.end(); ++it) {
		Aggregate agg = it->second;
		if (agg.lowerId<0) {
			pkt->addTimeBasedAggregate(agg.eventType, agg.constraints, agg.constraintsNum, agg.upperId, agg.lowerTime, agg.name, agg.fun);
		} else {
			pkt->addAggregateBetweenStates(agg.eventType, agg.constraints, agg.constraintsNum, agg.lowerId, agg.upperId, agg.name, agg.fun);
		}
		delete agg.constraints;
	}
	for (map<int, Negation>::iterator it=negations.begin(); it!=negations.end(); ++it) {
		Negation neg = it->second;
		if (neg.lowerId<0) {
			pkt->addTimeBasedNegation(neg.eventType, neg.constraints, neg.constraintsNum, neg.upperId, neg.lowerTime);
		} else {
			pkt->addNegationBetweenStates(neg.eventType, neg.constraints, neg.constraintsNum, neg.lowerId, neg.upperId);
		}
		delete neg.constraints;
	}
	for (map<int, Parameter>::iterator it=parameters.begin(); it!=parameters.end(); ++it) {
		Parameter par = it->second;
		if (par.type==STATE) {
			pkt->addParameterBetweenStates(par.evIndex1, par.name1, par.evIndex2, par.name2);
		} else if (par.type==AGG) {
			pkt->addParameterForAggregate(par.evIndex1, par.name1, par.evIndex2, par.name2);
		} else {
			pkt->addParameterForNegation(par.evIndex1, par.name1, par.evIndex2, par.name2);
		}
	}
	pkt->setCompositeEventTemplate(eventTemplate);
	for (set<int>::iterator it=consuming.begin(); it!=consuming.end(); ++it) {
		int consum = *it;
		pkt->addConsuming(consum);
	}
	return pkt;
}

PubPkt * Unmarshaller::decodePubPkt(char *source, int &index) {
	int eventType = decodeInt(source, index);
	TimeMs timeStamp = decodeInt(source, index);
	int numAttributes = decodeInt(source, index);
	Attribute attributes[numAttributes];
	for (int i=0; i<numAttributes; i++) {
		attributes[i] = decodeAttribute(source, index);
	}
	PubPkt *pkt = new PubPkt(eventType, attributes, numAttributes);
	pkt->setTime(timeStamp);
	return pkt;
}

SubPkt * Unmarshaller::decodeSubPkt(char *source, int &index) {
	int eventType = decodeInt(source, index);
	int numConstraints = decodeInt(source, index);
	if (numConstraints==0) return new SubPkt(eventType);
	Constraint constraints[numConstraints];
	for (int i=0; i<numConstraints; i++) {
		constraints[i] = decodeConstraint(source, index);
	}
	SubPkt *pkt = new SubPkt(eventType, constraints, numConstraints);
	return pkt;
}

AdvPkt * Unmarshaller::decodeAdvPkt(char *source, int &index) {
	set<int> advertisements = decodeIntSet(source, index);
	return new AdvPkt(advertisements);
}

JoinPkt * Unmarshaller::decodeJoinPkt(char *source, int &index) {
	long address = decodeLong(source, index);
	int port = decodeInt(source, index);
	return new JoinPkt(address, port);
}

Constraint Unmarshaller::decodeConstraint(char *source, int &index) {
	Constraint c;
	char *name = decodeString(source, index);
	strcpy(c.name, name);
	delete name;
	c.op = decodeConstraintOp(source, index);
	c.type = decodeValType(source, index);
	if (c.type==INT) c.intVal = decodeInt(source, index);
	else if (c.type==FLOAT) c.floatVal = decodeFloat(source, index);
	else if (c.type==BOOL) c.boolVal = decodeBoolean(source, index);
	else if (c.type==STRING) {
		char *stringVal = decodeString(source, index);
		strcpy(c.stringVal, stringVal);
		delete stringVal;
	}
	return c;
}

Attribute Unmarshaller::decodeAttribute(char *source, int &index) {
	Attribute att;
	char *name = decodeString(source, index);
	strcpy(att.name, name);
	delete name;
	att.type = decodeValType(source, index);
	if (att.type==INT) att.intVal = decodeInt(source, index);
	else if (att.type==FLOAT) att.floatVal = decodeFloat(source, index);
	else if (att.type==BOOL) att.boolVal = decodeBoolean(source, index);
	else if (att.type==STRING) {
		char *stringVal = decodeString(source, index);
		strcpy(att.stringVal, stringVal);
		delete stringVal;
	}
	return att;
}

Predicate Unmarshaller::decodeEventPredicate(char *source, int &index) {
	Predicate pred;
	pred.eventType = decodeInt(source, index);
	pred.constraintsNum = decodeInt(source, index);
	pred.constraints = new Constraint[pred.constraintsNum];
	for (int i=0; i<pred.constraintsNum; i++) {
		pred.constraints[i] = decodeConstraint(source, index);
	}
	pred.refersTo = decodeInt(source, index);
	pred.win = decodeInt(source, index);
	pred.kind = decodeCompKind(source, index);
	return pred;
}

Parameter Unmarshaller::decodeParameter(char *source, int &index) {
	Parameter par;
	par.evIndex1 = decodeInt(source, index);
	char *name1 = decodeString(source, index);
	strcpy(par.name1, name1);
	delete name1;
	par.evIndex2 = decodeInt(source, index);
	char *name2 = decodeString(source, index);
	strcpy(par.name2, name2);
	delete name2;
	par.type = decodeStateType(source, index);
	return par;
}

Negation Unmarshaller::decodeNegation(char *source, int &index) {
	Negation neg;
	neg.eventType = decodeInt(source, index);
	neg.constraintsNum = decodeInt(source, index);
	neg.constraints = new Constraint[neg.constraintsNum];
	for (int i=0; i<neg.constraintsNum; i++) {
		neg.constraints[i] = decodeConstraint(source, index);
	}
	neg.lowerId = decodeInt(source, index);
	neg.lowerTime = decodeInt(source, index);
	neg.upperId = decodeInt(source, index);
	return neg;
}

Aggregate Unmarshaller::decodeAggregate(char *source, int &index) {
	Aggregate agg;
	agg.eventType = decodeInt(source, index);
	agg.constraintsNum = decodeInt(source, index);
	agg.constraints = new Constraint[agg.constraintsNum];
	for (int i=0; i<agg.constraintsNum; i++) {
		agg.constraints[i] = decodeConstraint(source, index);
	}
	agg.lowerId = decodeInt(source, index);
	agg.lowerTime = decodeInt(source, index);
	agg.upperId = decodeInt(source, index);
	agg.fun = decodeAggregateFun(source, index);
	char *name = decodeString(source, index);
	strcpy(agg.name, name);
	delete name;
	return agg;
}

CompositeEventTemplate * Unmarshaller::decodeEventTemplate(char *source, int &index) {
	int eventType = decodeInt(source, index);
	int attrNum = decodeInt(source, index);
	CompositeEventTemplate *eventTemplate = new CompositeEventTemplate(eventType);
	for (int i=0; i<attrNum; i++) {
		char *name = decodeString(source, index);
		OpTree *tree = decodeOpTree(source, index);
		eventTemplate->addAttribute(name, tree);
		delete name;
	}
	return eventTemplate;
}

OpTree * Unmarshaller::decodeOpTree(char *source, int &index) {
	OpTreeType type = decodeOpTreeType(source, index);
	ValType valType = decodeValType(source, index);
	if (type == LEAF) {
		RulePktValueReference * val = decodeValueReference(source, index);
		return new OpTree(val, valType);
	} else {
		OpTree *leftTree = decodeOpTree(source, index);
		OpTree *rightTree = decodeOpTree(source, index);
		OpTreeOperation op = decodeOpTreeOperation(source, index);
		return new OpTree(leftTree, rightTree, op, valType);
	}
}

RulePktValueReference * Unmarshaller::decodeValueReference(char *source, int &index) {
	int idx = decodeInt(source, index);
	bool isAgg = decodeBoolean(source, index);
	char *name = decodeString(source, index);
	if (isAgg) {
		delete name;
		return new RulePktValueReference(idx);
	}
	else {
		RulePktValueReference *ref = new RulePktValueReference(idx, name);
		delete name;
		return ref;
	}
}

set<int> Unmarshaller::decodeIntSet(char *source, int &index) {
	int size = decodeInt(source, index);
	set<int> returnSet;
	for (int i=0; i<size; i++) {
		int element = decodeInt(source, index);
		returnSet.insert(element);
	}
	return returnSet;
}

bool Unmarshaller::decodeBoolean(char *source, int &index) {
	bool returnValue = (source[index++] == 1);
	return returnValue;
}

int Unmarshaller::decodeInt(char *source, int &index) {
	int returnValue = (0xff & source[index]) << 24 |
			(0xff & source[index+1]) << 16 |
			(0xff & source[index+2]) << 8  |
			(0xff & source[index+3]) << 0;
	index+=4;
	return returnValue;
}

float Unmarshaller::decodeFloat(char *source, int &index) {
	float returnValue = 0;
	int *ptr = (int *) &returnValue;
	*ptr = (0xff & source[index]) << 24 |
			(0xff & source[index+1]) << 16 |
			(0xff & source[index+2]) << 8  |
			(0xff & source[index+3]) << 0;
	index+=4;
	return returnValue;
}

long Unmarshaller::decodeLong(char *source, int &index) {
	long returnValue = (0xff & (long)source[index]) << 56 |
			(0xff & (long)source[index+1]) << 48 |
			(0xff & (long)source[index+2]) << 40 |
			(0xff & (long)source[index+3]) << 32 |
			(0xff & (long)source[index+4]) << 24 |
			(0xff & (long)source[index+5]) << 16 |
			(0xff & (long)source[index+6]) << 8	|
			(0xff & (long)source[index+7]) << 0;
	index+=8;
	return returnValue;
}

char * Unmarshaller::decodeString(char *source, int &index) {
	int length = decodeInt(source, index);
	char *charArray = new char[length+1];
	for (int i=0; i<length; i++) {
		charArray[i] = (char) source[index+i];
	}
	charArray[length] = '\0';
	index += length;
	return charArray;
}

CompKind Unmarshaller::decodeCompKind(char *source, int &index) {
	CompKind result;
	if (source[index]==0) result = EACH_WITHIN;
	else if (source[index]==1) result = FIRST_WITHIN;
	else if (source[index]==2) result = LAST_WITHIN;
	else result = ALL_WITHIN;
	index++;
	return result;
}

Op Unmarshaller::decodeConstraintOp(char *source, int &index) {
	Op result = EQ;
	if (source[index]==0) result = EQ;
	else if (source[index]==1) result = LT;
	else if (source[index]==2) result = GT;
	else if (source[index]==3) result = DF;
	else if (source[index]==4) result = IN;
	index++;
	return result;
}

StateType Unmarshaller::decodeStateType(char *source, int &index) {
	StateType result;
	if (source[index]==0) result = STATE;
	else if (source[index]==1) result = NEG;
	else result = AGG;
	index++;
	return result;
}

ValType Unmarshaller::decodeValType(char *source, int &index) {
	ValType result;
	if (source[index]==0) result = INT;
	else if (source[index]==1) result = FLOAT;
	else if (source[index]==2) result = BOOL;
	else result = STRING;
	index++;
	return result;
}

AggregateFun Unmarshaller::decodeAggregateFun(char *source, int &index) {
	AggregateFun result;
	if (source[index]==0) result = NONE;
	else if (source[index]==1) result = AVG;
	else if (source[index]==2) result = COUNT;
	else if (source[index]==3) result = MIN;
	else if (source[index]==4) result = MAX;
	else result = SUM;
	index++;
	return result;
}

OpTreeType Unmarshaller::decodeOpTreeType(char *source, int &index) {
	if (source[index++]==0) return LEAF;
	else return INNER;
}

OpTreeOperation Unmarshaller::decodeOpTreeOperation(char *source, int &index) {
	OpTreeOperation result;
	if (source[index]==0) result = ADD;
	else if (source[index]==1) result = SUB;
	else if (source[index]==2) result = MUL;
	else if (source[index]==3) result = DIV;
	else if (source[index]==4) result = AND;
	else result = OR;
	index++;
	return result;
}

PktType Unmarshaller::decodePktType(char *source, int &index) {
	PktType result;
	if (source[index]==0) result = PUB_PKT;
	else if (source[index]==1) result = RULE_PKT;
	else if (source[index]==2) result = SUB_PKT;
	else if (source[index]==3) result = ADV_PKT;
	else result = JOIN_PKT;
	index++;
	return result;
}
