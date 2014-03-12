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
	else if (caseStudy==KB_STUDY) createKbRules(rules);
	else if (caseStudy==PARAM_STUDY) createParamRules(rules);
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


// Edit the following function to generate kb rules, no matter if they are non-sense, it is just a test case
void RulesGenerator::createKbRules(set<RulePkt *> &rules) {
	int id = 1;
	int tempVal = 1;
	// This is the id of the smoke event (we compute the id of the temp event as smokeId+1000).
	// SmokeId varies in the range 1..numFireDefinitions
	int smokeId = 0;
	string queries[] = {"select ?name where { ?p <isCalled> ?name }", "select ?city where { ?p <bornInLocation> ?city }",
			"select ?name ?city where { ?p <isCalled> ?name. ?p <bornInLocation> ?city }",
			"select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> &city }",
			"select ?city where { ?p <isCalled> &name. ?p <bornInLocation> ?city }",
			"select ?name ?city where { ?p <isCalled> &name. ?p <bornInLocation> &city }"};
	string names[] = {"Stanley Holloway","Jerry Springer","Will Self","Ernest Thesiger","Peter Ackroyd","Mary Wollstonecraft Shelley","Mary Shelley","Alan M. Turing","Virginia Woolf","Beniaminus Disraeli","Davidas Rikardas","Michael Moorcock","Gilbert Keith Chesterton","Alistair Darling","Horace Walpole","Harold Alexander","John Donne","William Blake","Christopher Ingold","Neil Ross","Pops Mensah-Bonsu","Peter Cheyney","Kathryn Beaumont","Kelenna Azubuike","Carlos Raúl Villanueva","Michael Woodruff","Sean Yazbeck","Layla El","Alfred James Shaughnessy","Richard Harvey","John Sebastian Helmcken","David Boadella","Terry Fox","Clara Hughes","Dufferin Roblin","Gary Doer","David Reimer","James Coyne","Andy Bathgate","Mike Keane","Alexander Steen","Raymond Henault","Steve Corino","Bill Masterton","Ted Irvine","Ted Harris","Shannon Rempel","Reg Abbott","Jonathan Toews","Paul Baxter","John Marks (hockey)","Bruno Zarrillo","Lonny Bohonos","Travis Zajac","Frank Mathers","Dustin Boyd","Jennifer Ellison","Alfred Lennon","Mal Evans","Stephen Baxter","Gulielmus Ewart Gladstone","William Gladstone","Clive Barker","John Horton Conway","John Conway","Felicia Hemans","Andy Burnham","James Bulger","Mumes Bulger","James Larkin","Frank Hornby","Cathy Tyson","Augustus Radcliffe Grote","Neil Buchanan","Stephen Molyneux","Julia Lennon","Alfred Cheetham","John Redwood","Edward Pellew"};
	string cities[] = {"London","Winnipeg","Dover","Liverpool","Cambridge"};
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
		TimeMs win = getWindow();
		CompKind kind = getCompKind();
		pkt->addPredicate(id*1000+1, constraint, 1, 0, win, kind);
		pkt->addParameterBetweenStates(id*1000, "Z", id*1000+1, "Z");
		int q = rand() % 10;
		if (q == 0) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			char param1name[5];
			char param2name[6];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = '\0';

			param2name[0] = '?';
			param2name[1] = 'n';
			param2name[2] = 'a';
			param2name[3] = 'm';
			param2name[4] = 'e';
			param2name[5] = '\0';
			pkt->addParamerForQueryKB(0, param1name, 0, param2name);
		}
		if (q == 1) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			char param1name[5];
			char param2name[6];
			param1name[0] = 'c';
			param1name[1] = 'i';
			param1name[2] = 't';
			param1name[3] = 'y';
			param1name[4] = '\0';

			param2name[0] = '?';
			param2name[1] = 'c';
			param2name[2] = 'i';
			param2name[3] = 't';
			param2name[4] = 'y';
			param2name[5] = '\0';
			pkt->addParamerForQueryKB(0, param1name, 0, param2name);
		}
		if (q == 2) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			char param1name[10];
			char param2name[12];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = ',';
			param1name[5] = 'c';
			param1name[6] = 'i';
			param1name[7] = 't';
			param1name[8] = 'y';
			param1name[9] = '\0';

			param2name[0] = '?';
			param2name[1] = 'n';
			param2name[2] = 'a';
			param2name[3] = 'm';
			param2name[4] = 'e';
			param2name[5] = ',';
			param2name[6] = '?';
			param2name[7] = 'c';
			param2name[8] = 'i';
			param2name[9] = 't';
			param2name[10] = 'y';
			param2name[11] = '\0';

			pkt->addParamerForQueryKB(0, param1name, 0, param2name);
		}
		if(q == 3) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			ExtParameter * ep = new ExtParameter();
			char ext_param1name[5];
			char ext_param2name[6];
			ext_param1name[0] = 'c';
			ext_param1name[1] = 'i';
			ext_param1name[2] = 't';
			ext_param1name[3] = 'y';
			ext_param1name[4] = '\0';

			ext_param2name[0] = '&';
			ext_param2name[1] = 'c';
			ext_param2name[2] = 'i';
			ext_param2name[3] = 't';
			ext_param2name[4] = 'y';
			ext_param2name[5] = '\0';
			ep->evIndex1 = 0;
			ep->evIndex2 = 0;
			strcpy(ep->name1, ext_param1name);
			strcpy(ep->name2, ext_param2name);
			ep->seqId1 = 0;
			ep->seqId2 = 0;

			pkt->addExtParamToKBPred(ep);

			char param1name[5];
			char param2name[6];
			param1name[0] = 'c';
			param1name[1] = 'i';
			param1name[2] = 't';
			param1name[3] = 'y';
			param1name[4] = '\0';

			param2name[0] = '?';
			param2name[1] = 'c';
			param2name[2] = 'i';
			param2name[3] = 't';
			param2name[4] = 'y';
			param2name[5] = '\0';

			pkt->addParamerForQueryKB(0,param1name,0,param2name);

		}
		if(q == 4) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			ExtParameter * ep = new ExtParameter();
			char ext_param1name[5];
			char ext_param2name[6];
			ext_param1name[0] = 'n';
			ext_param1name[1] = 'a';
			ext_param1name[2] = 'm';
			ext_param1name[3] = 'e';
			ext_param1name[4] = '\0';

			ext_param2name[0] = '&';
			ext_param2name[1] = 'n';
			ext_param2name[2] = 'a';
			ext_param2name[3] = 'm';
			ext_param2name[4] = 'e';
			ext_param2name[5] = '\0';
			ep->evIndex1 = 0;
			ep->evIndex2 = 0;
			strcpy(ep->name1, ext_param1name);
			strcpy(ep->name2, ext_param2name);
			ep->seqId1 = 0;
			ep->seqId2 = 0;

			pkt->addExtParamToKBPred(ep);

			char param1name[5];
			char param2name[6];
			param1name[0] = 'c';
			param1name[1] = 'i';
			param1name[2] = 't';
			param1name[3] = 'y';
			param1name[4] = '\0';

			param2name[0] = '?';
			param2name[1] = 'c';
			param2name[2] = 'i';
			param2name[3] = 't';
			param2name[4] = 'y';
			param2name[5] = '\0';

			pkt->addParamerForQueryKB(0,param1name,0,param2name);

		}
		if(q == 5) {
			pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",queries[q]);
			ExtParameter * ep = new ExtParameter();
			char ext_param1name[5];
			char ext_param2name[6];
			ext_param1name[0] = 'c';
			ext_param1name[1] = 'i';
			ext_param1name[2] = 't';
			ext_param1name[3] = 'y';
			ext_param1name[4] = '\0';

			ext_param2name[0] = '&';
			ext_param2name[1] = 'c';
			ext_param2name[2] = 'i';
			ext_param2name[3] = 't';
			ext_param2name[4] = 'y';
			ext_param2name[5] = '\0';
			ep->evIndex1 = 0;
			ep->evIndex2 = 0;
			strcpy(ep->name1, ext_param1name);
			strcpy(ep->name2, ext_param2name);
			ep->seqId1 = 0;
			ep->seqId2 = 0;

			pkt->addExtParamToKBPred(ep);
			ext_param1name[0] = 'c';
			ext_param1name[1] = 'i';
			ext_param1name[2] = 't';
			ext_param1name[3] = 'y';
			ext_param1name[4] = '\0';

			ext_param2name[0] = '&';
			ext_param2name[1] = 'c';
			ext_param2name[2] = 'i';
			ext_param2name[3] = 't';
			ext_param2name[4] = 'y';
			ext_param2name[5] = '\0';
			ep->evIndex1 = 1;
			ep->evIndex2 = 0;
			strcpy(ep->name1, ext_param1name);
			strcpy(ep->name2, ext_param2name);
			ep->seqId1 = 0;
			ep->seqId2 = 0;

			pkt->addExtParamToKBPred(ep);

			char param1name[10];
			char param2name[12];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = ',';
			param1name[5] = 'c';
			param1name[6] = 'i';
			param1name[7] = 't';
			param1name[8] = 'y';
			param1name[9] = '\0';

			param2name[0] = '?';
			param2name[1] = 'n';
			param2name[2] = 'a';
			param2name[3] = 'm';
			param2name[4] = 'e';
			param2name[5] = ',';
			param2name[6] = '?';
			param2name[7] = 'c';
			param2name[8] = 'i';
			param2name[9] = 't';
			param2name[10] = 'y';
			param2name[11] = '\0';

			pkt->addParamerForQueryKB(0,param1name,0,param2name);

		}
		if (q > 5) {
			string query = "";
			int n = rand() % 79;
			int c = rand() % 5;
			if (q % 2 == 0) {
				query = "select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> <" + cities[c] +"> }";
				pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",query);
				char param1name[5];
				char param2name[6];
				param1name[0] = 'n';
				param1name[1] = 'a';
				param1name[2] = 'm';
				param1name[3] = 'e';
				param1name[4] = '\0';

				param2name[0] = '?';
				param2name[1] = 'n';
				param2name[2] = 'a';
				param2name[3] = 'm';
				param2name[4] = 'e';
				param2name[5] = '\0';
				pkt->addParamerForQueryKB(0, param1name, 0, param2name);
			} else {
				query = "select ?city where { ?p <isCalled> <" + names[n] + ". ?p <bornInLocation> ?city }";
				pkt->addKBRootPredicate(NULL,0,"/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db",query);
				char param1name[5];
				char param2name[6];
				param1name[0] = 'c';
				param1name[1] = 'i';
				param1name[2] = 't';
				param1name[3] = 'y';
				param1name[4] = '\0';

				param2name[0] = '?';
				param2name[1] = 'c';
				param2name[2] = 'i';
				param2name[3] = 't';
				param2name[4] = 'y';
				param2name[5] = '\0';
				pkt->addParamerForQueryKB(0, param1name, 0, param2name);
			}

		}
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);
	}
}

void RulesGenerator::createParamRules(set<RulePkt *> &rules) {
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
		TimeMs win = getWindow();
		CompKind kind = getCompKind();
		pkt->addPredicate(id*1000+1, constraint, 1, 0, win, kind);
		pkt->addParameterBetweenStates(id*1000, "Z", id*1000+1, "Z");
		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		if (paramHandler->getConsuming()) pkt->addConsuming(1);
		rules.insert(pkt);
	}
}
