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

#include "RulesGenerator.h"

using namespace std;

RulesGenerator::RulesGenerator(ParamHandler *parParamHandler) {
	paramHandler = parParamHandler;
}

RulesGenerator::~RulesGenerator() {
	// Nothing to do
}

void RulesGenerator::createRulePkts(set<RulePkt *> &rules) {
	int numRules = paramHandler->getNumRules();
	int caseStudy = paramHandler->getCaseStudy();
	// Rules are about fire
	if (caseStudy==FIRE_STUDY) createFireRules(rules);
	else if (caseStudy==RAIN_STUDY) createRainRules(rules);
	else if (caseStudy==LENGTH_STUDY) createLengthRules(rules);
	else if (caseStudy==WIDTH_STUDY) createWidthRules(rules);
	else if (caseStudy==SELECTION_STUDY) createSelectionRules(rules);
	else if (caseStudy==AGGREGATE_STUDY) createAggregateRules(rules);
	else {
		// Rules are randomly generated (synthetic workload)
		for (int i=0; i<numRules; i++) {
			// Creates a new packet; resets count only if it is the first iteration in the cycle
			RulePkt *pkt = new RulePkt(i==0);
			addRootPredicate(pkt);
			int numSeqs = 1;
			for (int i=1; i<paramHandler->getNumRulePredicates(); i++) {
				if (addPredicate(pkt, (numSeqs<paramHandler->getMaxAutSeqs()))) numSeqs++;
			}
			for (int i=0; i<paramHandler->getNumRuleNegations(); i++) {
				addNegation(pkt);
			}
			for (int i=0; i<paramHandler->getNumRuleParameters(); i++) {
				addParameter(pkt);
			}
			rules.insert(pkt);
		}
	}
}

void RulesGenerator::addRootPredicate(RulePkt *pkt) {
	int eventType = getRandomEventType();
	Constraint c[paramHandler->getNumConstraints()];
	initConstraints(c, paramHandler->getNumConstraints());
	TimeMs t = getWindow();
	pkt->addRootPredicate(eventType, c, paramHandler->getNumConstraints());
}

bool RulesGenerator::addPredicate(RulePkt *pkt, bool canSplit) {
	bool returnValue = false;
	int eventType = getRandomEventType();
	Constraint c[paramHandler->getNumConstraints()];
	initConstraints(c, paramHandler->getNumConstraints());
	int refersTo;
	if (canSplit) {
		refersTo = (rand()%pkt->getPredicatesNum());
		if (refersTo!=pkt->getPredicatesNum()-1) returnValue = true;
	} else {
		refersTo = pkt->getPredicatesNum()-1;
	}
	TimeMs t = getWindow();
	CompKind kind = getCompKind();
	pkt->addPredicate(eventType, c, paramHandler->getNumConstraints(), refersTo, t, kind);
	return returnValue;
}

void RulesGenerator::addParameter(RulePkt *pkt) {
	bool neg = false;
	// Creates a parameter involving a negation (if at least a negation is defined)
	if ((rand()%100>50) && pkt->getNegationsNum()>0) neg = true;
	int index1, index2, constraintIndex1, constraintIndex2;
	char name1[2], name2[2];
	if (neg) {
		index1 = rand()%pkt->getPredicatesNum();
		index2 = rand()%pkt->getNegationsNum();
		constraintIndex2 = rand()%(pkt->getNegation(index2).constraintsNum);
		name2[0] = pkt->getNegation(index2).constraints[constraintIndex2].name[0];
	} else {
		index1 = (rand()%(pkt->getPredicatesNum()-1))+1;
		index2 = (rand()%index1);
		constraintIndex2 = rand()%(pkt->getPredicate(index2).constraintsNum);
		name2[0] = pkt->getPredicate(index2).constraints[constraintIndex2].name[0];
	}
	constraintIndex1 = rand()%(pkt->getPredicate(index1).constraintsNum);
	name1[0] = pkt->getPredicate(index1).constraints[constraintIndex1].name[0];
	name1[1] = '\0';
	name2[1] = '\0';
	if (neg) pkt->addParameterForNegation(index1, name1, index2, name2);
	else pkt->addParameterBetweenStates(index1, name1, index2, name2);
}

void RulesGenerator::addNegation(RulePkt *pkt) {
	int eventType = getRandomEventType();
	Constraint c[paramHandler->getNumConstraints()];
	initConstraints(c, paramHandler->getNumConstraints());
	// Creates a negation between two states
	if ((rand()%100)<50) {
		int index1 = (rand()%(pkt->getPredicatesNum()-1))+1;
		int index2 = (rand()%index1);
		pkt->addNegationBetweenStates(eventType, c, paramHandler->getNumConstraints(), index1, index2);
	}
	// Creates a time based negation
	else {
		int index2 = (rand()%pkt->getPredicatesNum());
		TimeMs win = getWindow();
		pkt->addTimeBasedNegation(eventType, c, paramHandler->getNumConstraints(), index2, win);
	}
}

int RulesGenerator::getRandomEventType() {
	return (rand()%paramHandler->getNumEventTypes())+1;
}

void RulesGenerator::initConstraints(Constraint *constraints, int constraintsNum) {
	set<int> alreadyUsed;
	int num = (rand()%paramHandler->getNumNames());
	alreadyUsed.insert(num);
	constraints[0].name[0] = 'A' + num;
	constraints[0].name[1] = '\0';
	constraints[0].type = INT;
	constraints[0].intVal = (rand()%paramHandler->getNumValues())+1;
	int op = (rand()%OP_NUM);
	if (op==0) constraints[0].op = EQ;
	else if (op==1) constraints[0].op = LT;
	else if (op==2) constraints[0].op = GT;
	else if (op==3) constraints[0].op = DF;
	for (int i=1; i<constraintsNum; ) {
		num = (rand()%paramHandler->getNumNames());
		if (alreadyUsed.find(num)!=alreadyUsed.end()) continue;
		alreadyUsed.insert(num);
		constraints[i].name[0] = 'A' + num;
		constraints[i].name[1] = '\0';
		constraints[i].type = INT;
		constraints[i].intVal = (rand()%paramHandler->getNumValues())+1;
		op = (rand()%OP_NUM);
		if (op==0) constraints[i].op = EQ;
		else if (op==1) constraints[i].op = LT;
		else if (op==2) constraints[i].op = GT;
		else if (op==3) constraints[i].op = DF;
		i++;
	}
}

TimeMs RulesGenerator::getWindow() {
	int winSize = (rand()%(paramHandler->getMaxWinSize()-paramHandler->getMinWinSize()))+paramHandler->getMinWinSize();
	TimeMs t = winSize;
	return t;
}

CompKind RulesGenerator::getCompKind() {
	int r = (rand()%100)+1;
	if (r<=paramHandler->getEachPerc()) return EACH_WITHIN;
	else if (r<=paramHandler->getEachPerc()+paramHandler->getFirstPerc()) return FIRST_WITHIN;
	else return LAST_WITHIN;
}

void RulesGenerator::createFireRules(set<RulePkt *> &rules) {
	int tempVal = 1;
	// This is the id of the smoke event (we compute the id of the temp event as smokeId+1000).
	// SmokeId varies in the range 1..numFireDefinitions
	int smokeId = 0;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		if (i%(paramHandler->getNumDefinitions())==0) smokeId=1;
		else smokeId++;
		// 100 different temperature (between 1 and 100)
		if (tempVal++>=100) tempVal = 1;
		Constraint tempConst[1];
		tempConst[0].name[0] = 'T';
		tempConst[0].name[1] = '\0';
		tempConst[0].type = INT;
		tempConst[0].intVal = tempVal;
		tempConst[0].op = GT;
		Constraint smokeConst[1];
		smokeConst[0].name[0] = 'S';
		smokeConst[0].name[1] = '\0';
		smokeConst[0].type = INT;
		smokeConst[0].intVal = 1;
		smokeConst[0].op = EQ;
		TimeMs win = getWindow();
		// Packet Temp -> Smoke
		RulePkt *pkt = new RulePkt(i==1);
		pkt->addRootPredicate(smokeId, smokeConst, 1);
		pkt->addPredicate(smokeId+1000, tempConst, 1, 0, win, getCompKind());
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}

void RulesGenerator::createRainRules(set<RulePkt *> &rules) {
	int tempVal = 1;
	// This is the id of the rain event (we compute the id of the temp event as rainId+1000).
	// SmokeId varies in the range 1..numFireDefinitions
	int rainId = 0;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		if (i%(paramHandler->getNumDefinitions())==0) rainId=1;
		else rainId++;
		// 100 different temperature (between 1 and 100)
		if (tempVal++>=100) tempVal = 1;
		Constraint tempConst[1];
		tempConst[0].name[0] = 'T';
		tempConst[0].name[1] = '\0';
		tempConst[0].type = INT;
		tempConst[0].intVal = tempVal;
		tempConst[0].op = GT;
		Constraint rainConst[1];
		rainConst[0].name[0] = 'A';
		rainConst[0].name[1] = '\0';
		rainConst[0].type = INT;
		rainConst[0].intVal = 1;
		rainConst[0].op = EQ;
		TimeMs win = paramHandler->getRainNegationWin();
		// Packet Temp -> Smoke
		RulePkt *pkt = new RulePkt(i==1);
		pkt->addRootPredicate(rainId+1000, tempConst, 1);
		pkt->addTimeBasedNegation(rainId, rainConst, 1, 0, win);
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}

void RulesGenerator::createLengthRules(set<RulePkt *> &rules) {
	// This is the first part of the id of the event: it is multiplied by 1000 and added to the state identifier
	int id = 1;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		if (i%(paramHandler->getNumDefinitions())==0) id=1;
		else id++;
		RulePkt *pkt = new RulePkt(i==1);
		Constraint constraint[1];
		constraint[0].name[0] = 'V';
		constraint[0].name[1] = '\0';
		constraint[0].type = INT;
		constraint[0].intVal = 1;
		constraint[0].op = EQ;
		pkt->addRootPredicate(id*1000, constraint, 1);
		for (int j=1; j<paramHandler->getNumRulePredicates(); j++) {
			TimeMs win = getWindow();
			CompKind kind = getCompKind();
			pkt->addPredicate(id*1000+j, constraint, 1, j-1, win, kind);
		}
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		if (paramHandler->getConsuming()) pkt->addConsuming(1);
		rules.insert(pkt);
	}
}

void RulesGenerator::createWidthRules(set<RulePkt *> &rules) {
	// This is the first part of the id of the event: it is multiplied by 1000 and added to the state identifier
	int id = 1;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		if (i%(paramHandler->getNumDefinitions())==0) id=1;
		else id++;
		RulePkt *pkt = new RulePkt(i==1);
		Constraint constraint[1];
		constraint[0].name[0] = 'V';
		constraint[0].name[1] = '\0';
		constraint[0].type = INT;
		constraint[0].intVal = 1;
		constraint[0].op = EQ;
		pkt->addRootPredicate(id*1000, constraint, 1);
		for (int j=1; j<paramHandler->getNumRulePredicates(); j++) {
			TimeMs win = getWindow();
			CompKind kind = getCompKind();
			pkt->addPredicate(id*1000+j, constraint, 1, 0, win, kind);
		}
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}

void RulesGenerator::createSelectionRules(set<RulePkt *> &rules) {
	// This is the id used in every rule
	int id = 1;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		RulePkt *pkt = new RulePkt(i==1);
		Constraint constraint[1];
		constraint[0].name[0] = 'V';
		constraint[0].name[1] = '\0';
		constraint[0].type = INT;
		constraint[0].intVal = i;
		constraint[0].op = EQ;
		pkt->addRootPredicate(id, constraint, 1);
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}

void RulesGenerator::createAggregateRules(set<RulePkt *> &rules) {
	int tempVal = 1;
	// This is the id of the smoke event (we compute the id of the temp event as smokeId+1000).
	// SmokeId varies in the range 1..numFireDefinitions
	int smokeId = 0;
	for (int i=1; i<=paramHandler->getNumRules(); i++) {
		if (i%(paramHandler->getNumDefinitions())==0) smokeId=1;
		else smokeId++;
		// 100 different temperature (between 1 and 100)
		if (tempVal++>=100) tempVal = 1;
		Constraint tempConst[1];
		tempConst[0].name[0] = 'T';
		tempConst[0].name[1] = '\0';
		tempConst[0].type = INT;
		tempConst[0].intVal = tempVal;
		tempConst[0].op = GT;
		Constraint smokeConst[1];
		smokeConst[0].name[0] = 'S';
		smokeConst[0].name[1] = '\0';
		smokeConst[0].type = INT;
		smokeConst[0].intVal = 1;
		smokeConst[0].op = EQ;
		TimeMs win = getWindow();
		// Packet Temp -> Smoke
		RulePkt *pkt = new RulePkt(i==1);
		pkt->addRootPredicate(smokeId, smokeConst, 1);
		char attrName[3];
		attrName[0] = 'T';
		attrName[1] = '\0';
		pkt->addTimeBasedAggregate(smokeId+1000, tempConst, 1, 0, win, attrName, AVG);
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		OpTree *opTree = new OpTree(new RulePktValueReference(0), INT);
		ceTemplate->addAttribute(attrName, opTree);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}
