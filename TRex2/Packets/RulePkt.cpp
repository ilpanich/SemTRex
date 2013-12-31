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

#include "RulePkt.h"

using namespace std;

int RulePkt::lastId = 0;

RulePkt::RulePkt(bool resetCount) {
	if (resetCount) lastId = 0;
	ruleId = lastId++;
	ceTemplate = NULL;
}

RulePkt::~RulePkt() {
	for (map<int, Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it) {
		delete it->second.constraints;
	}
	for (map<int, Negation>::iterator it=negations.begin(); it!=negations.end(); ++it) {
		delete it->second.constraints;
	}
	for (map<int, Aggregate>::iterator it=aggregates.begin(); it!=aggregates.end(); ++it) {
		delete it->second.constraints;
	}
	if (ceTemplate!=NULL) delete ceTemplate;
}

bool RulePkt::addRootPredicate(int eventType, Constraint constr[], int constrLen) {
	if (predicates.size()>0) return false;
	Predicate p;
	p.eventType = eventType;
	p.refersTo = -1;
	p.win = 0;
	p.kind = EACH_WITHIN;
	p.constraintsNum = constrLen;
	p.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) p.constraints[i] = constr[i];
	predicates.insert(make_pair(predicates.size(), p));
	return true;
}

bool RulePkt::addPredicate(int eventType, Constraint constr[], int constrLen, int refersTo, TimeMs &win, CompKind kind) {
	int numPredicates = predicates.size();
	if (numPredicates<=0 || refersTo>=numPredicates) return false;
	Predicate p;
	p.eventType = eventType;
	p.refersTo = refersTo;
	p.win = win;
	p.kind = kind;
	p.constraintsNum = constrLen;
	p.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) p.constraints[i] = constr[i];
	predicates.insert(make_pair(predicates.size(), p));
	return true;
}

bool RulePkt::addKBRootPredicate(Constraint constr[], int constrLen, string kb, string q) {
	if (kbPredicates.size()>0) return false;
	if (predicates.size()<=0) return false;
	KBPredicate p;
	p.constraintsNum = constrLen;
	p.refPredType = STD;
	p.refersTo = predicates.size() - 1;
	p.db = kb;
	p.query = q;
	p.dbId = MD5((const unsigned char*) kb.c_str(), kb.length(), NULL);
	p.qId = MD5((const unsigned char*) q.c_str(), q.length(), NULL);
	p.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) p.constraints[i] = constr[i];
	kbPredicates.insert(make_pair(predicates.size(), p));
	return true;
}

bool RulePkt::addKBPredicate(Constraint constr[], int constrLen, int refersTo, string kb, string q) {
	int numKBPredicates = kbPredicates.size();
	if (numKBPredicates<=0 || refersTo>=numKBPredicates) return false;
	KBPredicate p;
	p.refPredType = KB;
	p.refersTo = refersTo;
	p.constraintsNum = constrLen;
	p.db = kb;
	p.query = q;
	p.dbId = MD5((const unsigned char*) kb.c_str(), kb.length(), NULL);
	p.qId = MD5((const unsigned char*) q.c_str(), q.length(), NULL);
	// The following line executes the query. Must be moved to the appropriate TESLA rule execution section
	//p.rs = RDFQuery::execQuery(kb*, query*, false);
	p.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) p.constraints[i] = constr[i];
	kbPredicates.insert(make_pair(predicates.size() + kbPredicates.size(), p));
	return true;
}

bool RulePkt::addExtParamToKBPred(ExtParameter *param) {
	int numPredicates = predicates.size();
	int nummKBPredicates = predicates.size() + kbPredicates.size();
	int kbPId = param->evIndex1;
	int pId = param->evIndex2;
	if (numPredicates<=0 || pId >=numPredicates ||
			nummKBPredicates <= numPredicates || kbPId >= nummKBPredicates)
		return false;
	kbPredicates.at(kbPId).param.push_back(*param);
	return true;
}

bool RulePkt::addTimeBasedNegation(int eventType, Constraint *constraints, int constrLen, int referenceId, TimeMs &win) {
	return addNegation(eventType, constraints, constrLen, -1, win, referenceId);
}

bool RulePkt::addNegationBetweenStates(int eventType, Constraint *constraints, int constrLen, int id1, int id2) {
	TimeMs unused;
	int lowerId, upperId;
	if (id1<id2) {
		upperId=id1;
		lowerId=id2;
	} else {
		upperId=id2;
		lowerId=id1;
	}
	return addNegation(eventType, constraints, constrLen, lowerId, unused, upperId);
}

bool RulePkt::addParameterBetweenStates(int id1, char *name1, int id2, char *name2) {
	int lowerId, upperId;
	char *lowerName;
	char *upperName;
	if (id1<id2) {
		upperId=id1;
		upperName=name1;
		lowerId=id2;
		lowerName=name2;
	} else {
		upperId=id2;
		upperName=name2;
		lowerId=id1;
		lowerName=name1;
	}
	return addParameter(lowerId, lowerName, upperId, upperName, STATE);
}

bool RulePkt::addParameterForNegation(int id, char *name, int negId, char *negName) {
	return addParameter(id, name, negId, negName, NEG);
}

bool RulePkt::addParameterForAggregate(int id, char *name, int aggId, char *aggName) {
	return addParameter(id, name, aggId, aggName, AGG);
}

bool RulePkt::addTimeBasedAggregate(int eventType, Constraint *constraints, int constrLen, int referenceId, TimeMs &win, char *name, AggregateFun fun) {
	return addAggregate(eventType, constraints, constrLen, -1, win, referenceId, name, fun);
}

bool RulePkt::addAggregateBetweenStates(int eventType, Constraint *constraints, int constrLen, int id1, int id2, char *name, AggregateFun fun) {
	TimeMs unused;
	int lowerId, upperId;
	if (id1<id2) {
		upperId=id1;
		lowerId=id2;
	} else {
		upperId=id2;
		lowerId=id1;
	}
	return addAggregate(eventType, constraints, constrLen, lowerId, unused, upperId, name, fun);
}

bool RulePkt::addConsuming(int eventIndex) {
	int numPredicates = predicates.size();
	if (eventIndex<0 || eventIndex>=numPredicates) return false;
	consuming.insert(eventIndex);
	return true;
}

void RulePkt::getLeaves(set<int> &leaves) {
	map<int, int> referenceCount;
	getReferenceCount(referenceCount);
	for (map<int, int>::iterator it=referenceCount.begin(); it!=referenceCount.end(); it++) {
		if (it->second == 0) leaves.insert(it->first);
	}
}

void RulePkt::getJoinPoints(set<int> &joinPoints) {
	map<int, int> referenceCount;
	getReferenceCount(referenceCount);
	for (map<int, int>::iterator it=referenceCount.begin(); it!=referenceCount.end(); it++)
		if (it->second > 0) joinPoints.insert(it->first);
}

bool RulePkt::containsEventType(int eventType, bool includeNegations) {
	for (map<int, Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it) {
		if (it->second.eventType==eventType) return true;
	}
	if (includeNegations) {
		for (map<int, Negation>::iterator it=negations.begin(); it!=negations.end(); ++it) {
			if (it->second.eventType==eventType) return true;
		}
	}
	return false;
}

void RulePkt::getContainedEventTypes(set<int> &evTypes) {
	for (map<int, Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it) {
		evTypes.insert(it->second.eventType);
	}
}

TimeMs RulePkt::getMaxWin() {
	set<int> leaves;
	getLeaves(leaves);
	TimeMs returnTime = 0;
	for (set<int>::iterator it=leaves.begin(); it!=leaves.end(); it++) {
		int leaf = *it;
		TimeMs currentTime = getWinBetween(0, leaf);
		if (currentTime > returnTime) returnTime = currentTime;
	}
	return returnTime;
}

TimeMs RulePkt::getWinBetween(int lowerId, int upperId) {
	int currentIndex = upperId;
	TimeMs timeBetween = 0;
	while (currentIndex!=lowerId) {
		timeBetween += predicates[currentIndex].win;
		currentIndex = predicates[currentIndex].refersTo;
	}
	return timeBetween;
}

bool RulePkt::isDirectlyConnected(int id1, int id2) {
	int numPredicates = predicates.size();
	if (id1==id2 || id1<0 || id2<0 || id1>numPredicates || id2>numPredicates) return false;
	if (predicates[id2].refersTo==id1 || predicates[id1].refersTo==id2) return true;
	return false;
}

bool RulePkt::isInTheSameSequence(int id1, int id2) {
	int numPredicates = predicates.size();
	if (id1==id2 || id1<0 || id2<0 || id1>numPredicates || id2>numPredicates) return false;
	int min, max;
	if (id1<id2) {
		min=id1;
		max=id2;
	} else {
		min=id2;
		max=id1;
	}
	int i=max;
	while(i>0) {
		i=predicates[i].refersTo;
		if (i==min) return true;
	}
	return false;
}

bool RulePkt::operator<(const RulePkt &pkt) const {
	return ruleId<pkt.ruleId;
}

bool RulePkt::operator==(const RulePkt &pkt) const {
	return ruleId==pkt.ruleId;
}

bool RulePkt::operator!=(const RulePkt &pkt) const {
	return ruleId!=pkt.ruleId;
}

bool RulePkt::addNegation(int eventType, Constraint *constr, int constrLen, int lowerId, TimeMs &lowerTime, int upperId) {
	int numPredicates = predicates.size();
	if (lowerId>numPredicates) return false;
	if (upperId<0 || upperId>numPredicates) return false;
	if (lowerId>=0 && lowerId<=upperId) return false;
	Negation n;
	n.eventType = eventType;
	n.lowerId = lowerId;
	if (lowerId<0) n.lowerTime = lowerTime;
	n.upperId = upperId;
	n.constraintsNum = constrLen;
	n.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) n.constraints[i] = constr[i];
	negations.insert(make_pair(negations.size(), n));
	return true;
}

bool RulePkt::addParameter(int index1, char *name1, int index2, char *name2, StateType type) {
	int numPredicates = predicates.size();
	int numKBPredicates = kbPredicates.size();
	int numAggregates = aggregates.size();
	int numNegations = negations.size();
	if (index1<0 || index1>numPredicates) return false;
	if (index2<0) return false;
	if (type==STATE && index2>numPredicates + numKBPredicates) return false;
	if (type==AGG && index2>numAggregates) return false;
	if (type==NEG && index2>numNegations) return false;
	Parameter p;
	p.evIndex1 = index1;
	p.evIndex2 = index2;
	if (type==STATE && index2 < numPredicates)
		p.type = STATE;
	else {
		if(type==STATE && index2 > numPredicates + numKBPredicates)) return false;
		else p.type = KB;
	}
	strcpy(p.name1, name1);
	strcpy(p.name2, name2);
	parameters.insert(make_pair(parameters.size(), p));
	return true;
}

bool RulePkt::addAggregate(int eventType, Constraint *constr, int constrLen, int lowerId, TimeMs &lowerTime, int upperId, char *name, AggregateFun fun) {
	int numPredicates = predicates.size();
	if (lowerId>numPredicates) return false;
	if (upperId<0 || upperId>numPredicates) return false;
	if (lowerId>=0 && lowerId<=upperId) return false;
	Aggregate a;
	a.eventType = eventType;
	a.lowerId = lowerId;
	if (lowerId<0) a.lowerTime = lowerTime;
	a.upperId = upperId;
	a.constraintsNum = constrLen;
	a.constraints = new Constraint[constrLen];
	for (int i=0; i<constrLen; i++) a.constraints[i] = constr[i];
	strcpy(a.name, name);
	a.fun = fun;
	aggregates.insert(make_pair(aggregates.size(), a));
	return true;
}

// PANIGATI Aggiungere predicato KB

void RulePkt::getReferenceCount(map<int, int> &referenceCount) {
	int numPredicates = predicates.size();
	for (int i=0; i<numPredicates; i++) referenceCount.insert(make_pair(i, 0));
	for (int i=1; i<numPredicates; i++) {
		int referredIndex = predicates[i].refersTo;
		map<int, int>::iterator it = referenceCount.find(referredIndex);
		int count = it->second + 1;
		referenceCount.erase(it);
		referenceCount.insert(make_pair(referredIndex, count));
	}
}
