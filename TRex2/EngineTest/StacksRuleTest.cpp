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

#include "StacksRuleTest.h"

using namespace std;

bool trex_testing::stacksRuleTest() {
	cout << "*** StacksRule ***" << endl;
	cout << "TestSequence\t";
	if (! singleSequence()) return false;
	cout << "OK" << endl << "SingleSequenceEach\t";
	if (! singleSequenceEach()) return false;
	cout << "OK" << endl << "SingleSequenceFirst\t";
	if (! singleSequenceFirst()) return false;
	cout << "OK" << endl << "SingleSequenceLast\t";
	if (! singleSequenceLast()) return false;
	cout << "OK" << endl << "SingleSequenceFirstEach\t";
	if (! singleSequenceFirstEach()) return false;
	cout << "OK" << endl << "SingleSequenceLastEach\t";
	if (! singleSequenceLastEach()) return false;
	cout << "OK" << endl << "TestEventGeneration\t";
	if (! testEventGeneration()) return false;
	cout << "OK" << endl << "TestBetweenAggregates\t";
	if (! testBetweenAggregates()) return false;
	cout << "OK" << endl << "TestTimingAggregates\t";
	if (! testTimingAggregates()) return false;
	cout << "OK" << endl << "TestAggregatesParameters\t";
	if (! testAggregatesParameters()) return false;
	cout << "OK" << endl << "TestJoins1\t";
	if (! testJoins1()) return false;
	cout << "OK" << endl << "TestJoins2\t";
	if (! testJoins2()) return false;
	cout << "OK" << endl << "TestJoins3\t";
	if (! testJoins3()) return false;
	cout << "OK" << endl << "TestJoins4\t";
	if (! testJoins4()) return false;
	cout << "OK" << endl << "TestExternalParams\t";
	if (! testExternalParams()) return false;
	cout << "OK" << endl << "TestConsumptionEach\t";
	if (! testConsumptionEach()) return false;
	cout << "OK" << endl << "TestConsumptionFirst\t";
	if (! testConsumptionFirst()) return false;
	cout << "OK" << endl << "TestConsumptionLast\t";
	if (! testConsumptionLast()) return false;
	cout << "OK" << endl << "TestConsumptionJoin\t";
	if (! testConsumptionJoin()) return false;
	cout << "OK" << endl << "TestConsumptionJoin2\t";
	if (! testConsumptionJoin2()) return false;
	cout << "OK" << endl << "TestConsumptionJoin3\t";
	if (! testConsumptionJoin3()) return false;
	cout << "OK" << endl << "TestAggregatesConsumption\t";
	cout << "\nAll StacksRule tests ran successfully!\n" << endl;
	cout << endl;
	return true;
}

bool trex_testing::singleSequence() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::singleSequenceEach() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	pkt2c->setTime(5);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(6);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(6);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=4) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=6) {
		cout << "Error: generating the wrong number of packets after processing packet 3b!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	return true;
}

bool trex_testing::singleSequenceFirst() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = FIRST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (! results.size()==0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results using before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3b!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	return true;
}

bool trex_testing::singleSequenceLast() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = LAST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (! results.size()==0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results using before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3b!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	return true;
}

bool trex_testing::singleSequenceFirstEach() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	CompKind kind = FIRST_WITHIN;
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	kind = EACH_WITHIN;
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	pkt2c->setTime(5);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(6);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(7);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (! results.size()==0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results using before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3b!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	return true;
}

bool trex_testing::singleSequenceLastEach() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	CompKind kind = EACH_WITHIN;
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	kind = LAST_WITHIN;
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	pkt2c->setTime(5);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(6);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(7);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (! results.size()==0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results using before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=3) {
		cout << "Error: generating the wrong number of packets after processing packet 3b!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	return true;
}

bool trex_testing::testEventGeneration() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	char name[2];
	name[0] = 'V';
	name[1] = '\0';
	RulePktValueReference *ref1 = new RulePktValueReference(0, name);
	RulePktValueReference *ref2 = new RulePktValueReference(1, name);
	RulePktValueReference *ref3 = new RulePktValueReference(2, name);
	OpTree *t1 = new OpTree(ref1, INT);
	OpTree *t2 = new OpTree(ref2, INT);
	OpTree *t3 = new OpTree(ref3, INT);
	OpTree *t12 = new OpTree(t1, t2, ADD, INT);
	OpTree *t123 = new OpTree(t12, t3, MUL, INT);
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	char ceName[2];
	ceName[0] = 'W';
	ceName[1] = '\0';
	ceTemplate->addAttribute(ceName, t123);

	buildConstraint(c1, 1);
	c1[0].op = GT;
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(2);
	att.intVal = 2;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(3);
	att.intVal = 3;
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	pkt4->setTime(4);
	att.intVal = 4;
	PubPkt *pkt5 = new PubPkt(1, &att, 1);
	pkt5->setTime(5);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;
	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	PubPkt *genPkt = *(results.begin());
	int genPktIndex;
	ValType genPktType;
	if (! genPkt->getAttributeIndexAndType(ceName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the required parameter, after processing packet 3!" << endl;
		return false;
	}
	if (genPkt->getIntAttributeVal(genPktIndex)!=12) {
		cout << "Error: wrong generated value for the W attribute, after processing packet 3!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	genPkt = *(results.begin());
	if (! genPkt->getAttributeIndexAndType(ceName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the required parameter, after processing packet 4!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=15) {
		cout << "Error: wrong generated value for the W attribute, after processing packet 4!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	genPkt = *(results.begin());
	if (! genPkt->getAttributeIndexAndType(ceName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the required parameter, after processing packet 5!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=18) {
		cout << "Error: wrong generated value for the W attribute, after processing packet 5!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testBetweenAggregates() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	char name[2];
	name[0] = 'V';
	name[1] = '\0';
	RulePktValueReference *maxRef = new RulePktValueReference(0);
	RulePktValueReference *avgRef = new RulePktValueReference(1);
	OpTree *maxTree = new OpTree(maxRef, INT);
	OpTree *avgTree = new OpTree(avgRef, INT);
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	char maxName[2];
	maxName[0] = 'M';
	maxName[1] = '\0';
	char avgName[2];
	avgName[0] = 'A';
	avgName[1] = '\0';
	ceTemplate->addAttribute(maxName, maxTree);
	ceTemplate->addAttribute(avgName, avgTree);

	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 1);
	c3[0].op = GT;
	AggregateFun fMax = MAX;
	AggregateFun fAvg = AVG;
	pkt->addAggregateBetweenStates(3, c3, 1, 1, 0, name, fMax);
	pkt->addAggregateBetweenStates(3, c3, 1, 1, 0, name, fAvg);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 2;
	PubPkt *pkt1 = new PubPkt(2, &att, 1);
	pkt1->setTime(1);
	att.intVal = 1;
	PubPkt *pkt2 = new PubPkt(1, &att, 1);
	pkt2->setTime(6);

	att.intVal = 10;
	PubPkt *agg1 = new PubPkt(3, &att, 1);
	agg1->setTime(2);
	att.intVal = 20;
	PubPkt *agg2 = new PubPkt(3, &att, 1);
	agg2->setTime(3);
	att.intVal = 30;
	PubPkt *agg3 = new PubPkt(3, &att, 1);
	agg3->setTime(4);
	att.intVal = 20;
	PubPkt *agg4 = new PubPkt(3, &att, 1);
	agg4->setTime(5);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;
	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg1, *mh);
	sr->processPkt(agg1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg2, *mh);
	sr->processPkt(agg2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg3, *mh);
	sr->processPkt(agg3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg4, *mh);
	sr->processPkt(agg4, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 4!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating more than one message when packet 2 arrives!" << endl;
		return false;
	}
	PubPkt *genPkt = *(results.begin());
	int genPktIndex;
	ValType genPktType;
	if (! genPkt->getAttributeIndexAndType(maxName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the M parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=30) {
		cout << "Error: wrong generated value for the M attribute: " << genPkt->getIntAttributeVal(genPktIndex) << "!" << endl;
		return false;
	}
	if (! genPkt->getAttributeIndexAndType(avgName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the A parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=(10+20+30+20)/4) {
		cout << "Error: wrong generated value for the A attribute!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (agg1->decRefCount()) delete agg1;
	if (agg2->decRefCount()) delete agg2;
	if (agg3->decRefCount()) delete agg3;
	if (agg4->decRefCount()) delete agg4;
	return true;
}

bool trex_testing::testTimingAggregates() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 100;
	CompKind kind = EACH_WITHIN;
	char name[2];
	name[0] = 'V';
	name[1] = '\0';
	RulePktValueReference *maxRef = new RulePktValueReference(0);
	RulePktValueReference *avgRef = new RulePktValueReference(1);
	OpTree *maxTree = new OpTree(maxRef, INT);
	OpTree *avgTree = new OpTree(avgRef, INT);
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	char maxName[2];
	maxName[0] = 'M';
	maxName[1] = '\0';
	char avgName[2];
	avgName[0] = 'A';
	avgName[1] = '\0';
	ceTemplate->addAttribute(maxName, maxTree);
	ceTemplate->addAttribute(avgName, avgTree);

	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 1);
	c3[0].op = GT;
	TimeMs aggWin = 50;
	AggregateFun fMax = MAX;
	AggregateFun fAvg = AVG;
	pkt->addTimeBasedAggregate(3, c3, 1, 0, aggWin, name, fMax);
	pkt->addTimeBasedAggregate(3, c3, 1, 0, aggWin, name, fAvg);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	TimeMs currentTime;

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 2;
	PubPkt *pkt1 = new PubPkt(2, &att, 1);
	pkt1->setTime(currentTime+10);
	att.intVal = 1;
	PubPkt *pkt2 = new PubPkt(1, &att, 1);
	pkt2->setTime(currentTime+100);

	att.intVal = 100;
	PubPkt *agg1 = new PubPkt(3, &att, 1);
	agg1->setTime(currentTime+30); // Should not be used!
	att.intVal = 10;
	PubPkt *agg2 = new PubPkt(3, &att, 1);
	agg2->setTime(currentTime+60);
	att.intVal = 20;
	PubPkt *agg3 = new PubPkt(3, &att, 1);
	agg3->setTime(currentTime+70);
	att.intVal = 30;
	PubPkt *agg4 = new PubPkt(3, &att, 1);
	agg4->setTime(currentTime+80);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;
	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg1, *mh);
	sr->processPkt(agg1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg2, *mh);
	sr->processPkt(agg2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg3, *mh);
	sr->processPkt(agg3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg4, *mh);
	sr->processPkt(agg4, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 4!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating more than one message when packet 2 arrives!" << endl;
		return false;
	}
	PubPkt *genPkt = *(results.begin());
	int genPktIndex;
	ValType genPktType;
	if (! genPkt->getAttributeIndexAndType(maxName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the M parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=30) {
		cout << "Error: wrong generated value for the M attribute!" << endl;
		return false;
	}
	if (! genPkt->getAttributeIndexAndType(avgName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the A parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=(10+20+30)/3) {
		cout << "Error: wrong generated value for the A attribute!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (agg1->decRefCount()) delete agg1;
	if (agg2->decRefCount()) delete agg2;
	if (agg3->decRefCount()) delete agg3;
	if (agg4->decRefCount()) delete agg4;
	return true;
}

bool trex_testing::testAggregatesParameters() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	char name[2];
	name[0] = 'V';
	name[1] = '\0';
	RulePktValueReference *maxRef = new RulePktValueReference(0);
	RulePktValueReference *avgRef = new RulePktValueReference(1);
	OpTree *maxTree = new OpTree(maxRef, INT);
	OpTree *avgTree = new OpTree(avgRef, INT);
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	char maxName[2];
	maxName[0] = 'M';
	maxName[1] = '\0';
	char avgName[2];
	avgName[0] = 'A';
	avgName[1] = '\0';
	ceTemplate->addAttribute(maxName, maxTree);
	ceTemplate->addAttribute(avgName, avgTree);

	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 1);
	c3[0].op = GT;
	AggregateFun fMax = MAX;
	AggregateFun fAvg = AVG;
	pkt->addAggregateBetweenStates(3, c3, 1, 1, 0, name, fMax);
	pkt->addAggregateBetweenStates(3, c3, 1, 1, 0, name, fAvg);
	char parName[2];
	parName[0] = 'P';
	parName[1] = '\0';
	pkt->addParameterForAggregate(0, parName, 1, parName);
	pkt->addParameterForAggregate(1, parName, 0, parName);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att[2];
	strcpy(att[0].name, "V");
	strcpy(att[1].name, "P");
	att[0].type = INT;
	att[1].type = INT;
	att[0].intVal = 2;
	att[1].intVal = 10;
	PubPkt *pkt1 = new PubPkt(2, att, 2);
	pkt1->setTime(1);
	att[0].intVal = 1;
	att[1].intVal = 20;
	PubPkt *pkt2 = new PubPkt(1, att, 2);
	pkt2->setTime(6);

	att[0].intVal = 10;
	att[1].intVal = 10;
	PubPkt *agg1 = new PubPkt(3, att, 2);
	agg1->setTime(2);
	att[0].intVal = 60;
	att[1].intVal = 20;
	PubPkt *agg2 = new PubPkt(3, att, 2);
	agg2->setTime(3);
	att[0].intVal = 45;
	att[1].intVal = 10;
	PubPkt *agg3 = new PubPkt(3, att, 2);
	agg3->setTime(4);
	att[0].intVal = 20;
	att[1].intVal = 20;
	PubPkt *agg4 = new PubPkt(3, att, 2);
	agg4->setTime(5);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;
	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg1, *mh);
	sr->processPkt(agg1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg2, *mh);
	sr->processPkt(agg2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg3, *mh);
	sr->processPkt(agg3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(agg4, *mh);
	sr->processPkt(agg4, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generated results after processing aggregate message 4!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating more than one message when packet 2 arrives!" << endl;
		return false;
	}
	PubPkt *genPkt = *(results.begin());
	int genPktIndex;
	ValType genPktType;
	if (! genPkt->getAttributeIndexAndType(maxName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the M parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=45) {
		cout << "Error: wrong generated value for the M attribute!" << endl;
		return false;
	}
	if (! genPkt->getAttributeIndexAndType(avgName, genPktIndex, genPktType)) {
		cout << "Error: the generated packet does not have the A parameter!" << endl;
		return false;
	}
	if (genPktType!=INT || genPkt->getIntAttributeVal(genPktIndex)!=40) {
		cout << "Error: wrong generated value for the A attribute!" << endl;
		return false;
	}
	if (genPkt->decRefCount()) delete genPkt;
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (agg1->decRefCount()) delete agg1;
	if (agg2->decRefCount()) delete agg2;
	if (agg3->decRefCount()) delete agg3;
	if (agg4->decRefCount()) delete agg4;
	return true;
}

bool trex_testing::testJoins1() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results with only one sequence completed, after processing packet 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testJoins2() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	Constraint c4[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	buildConstraint(c4, 4);
	pkt->addPredicate(4, c4, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 4;
	PubPkt *pkt1 = new PubPkt(4, &att, 1);
	att.intVal = 3;
	PubPkt *pkt2 = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt3 = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testJoins3() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	Constraint c4[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	buildConstraint(c4, 4);
	pkt->addPredicate(4, c4, 1, 1, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 4;
	PubPkt *pkt1 = new PubPkt(4, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(4, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 3;
	PubPkt *pkt2 = new PubPkt(3, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(3, &att, 1);
	pkt2b->setTime(4);
	att.intVal = 2;
	PubPkt *pkt3 = new PubPkt(2, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt3b = new PubPkt(2, &att, 1);
	pkt3b->setTime(6);
	att.intVal = 1;
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	pkt4->setTime(7);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);
	pkt5->setTime(8);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 3b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=8) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=8) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testJoins4() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	Constraint c4[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kind);
	buildConstraint(c4, 4);
	pkt->addPredicate(4, c4, 1, 0, win, kind);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 4;
	PubPkt *pkt1 = new PubPkt(4, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(4, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 3;
	PubPkt *pkt2 = new PubPkt(3, &att, 1);
	pkt2->setTime(3);
	att.intVal = 2;
	PubPkt *pkt3 = new PubPkt(2, &att, 1);
	pkt3->setTime(4);
	att.intVal = 1;
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	pkt4->setTime(5);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);
	pkt5->setTime(6);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 3!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testExternalParams() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kind);
	char paramName[2];
	paramName[0] = 'P';
	paramName[1] = '\0';
	pkt->addParameterBetweenStates(1, paramName, 2, paramName);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute attPkt1[2];
	attPkt1[0].type = INT;
	attPkt1[1].type = INT;
	strcpy(attPkt1[0].name, "V");
	attPkt1[0].intVal = 3;
	strcpy(attPkt1[1].name, "P");
	attPkt1[1].intVal = 10;
	PubPkt *pkt1 = new PubPkt(3, attPkt1, 2);
	pkt1->setTime(1);

	// Good for the external parameter
	Attribute attPkt2[2];
	attPkt2[0].type = INT;
	attPkt2[1].type = INT;
	strcpy(attPkt2[0].name, "V");
	attPkt2[0].intVal = 2;
	strcpy(attPkt2[1].name, "P");
	attPkt2[1].intVal = 10;
	PubPkt *pkt2 = new PubPkt(2, attPkt2, 2);
	pkt2->setTime(2);

	// Not good for the external parameter (wrong value for P)
	Attribute attPkt2b[2];
	attPkt2b[0].type = INT;
	attPkt2b[1].type = INT;
	strcpy(attPkt2b[0].name, "V");
	attPkt2b[0].intVal = 2;
	strcpy(attPkt2b[1].name, "P");
	attPkt2b[1].intVal = 20;
	PubPkt *pkt2b = new PubPkt(2, attPkt2b, 2);
	pkt2b->setTime(3);

	// Not good for the external parameter (no attribute for P)
	Attribute attPkt2c[1];
	attPkt1[0].type = INT;
	strcpy(attPkt2c[0].name, "V");
	attPkt2c[0].intVal = 2;
	PubPkt *pkt2c = new PubPkt(2, attPkt2c, 1);
	pkt2c->setTime(4);

	// Good for external parameters
	PubPkt *pkt2d = new PubPkt(2, attPkt2, 2);
	pkt2d->setTime(6);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt4 = new PubPkt(1, &att, 1);
	pkt4->setTime(7);
	PubPkt *pkt5 = new PubPkt(1, &att, 1);
	pkt5->setTime(8);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results with only one sequence completed, after processing packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2d, *mh);
	sr->processPkt(pkt2d, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2d!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt4, *mh);
	sr->processPkt(pkt4, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 4!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt5, *mh);
	sr->processPkt(pkt5, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 5!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt2d->decRefCount()) delete pkt2d;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt4->decRefCount()) delete pkt4;
	if (pkt5->decRefCount()) delete pkt5;
	return true;
}

bool trex_testing::testConsumptionEach() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = EACH_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->addConsuming(1);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(6);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);
	pkt3c->setTime(7);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating messages after processing packet 3b (seems to ignore the consuming clause for packet 2)!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	return true;
}

bool trex_testing::testConsumptionFirst() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = FIRST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->addConsuming(1);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating messages after processing packet 3b (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	return true;
}

bool trex_testing::testConsumptionLast() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kind = LAST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kind);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 1, win, kind);
	pkt->addConsuming(1);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	PubPkt *pkt2c = new PubPkt(2, &att, 1);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating messages after processing packet 3b (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2c, *mh);
	sr->processPkt(pkt2c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=1) {
		cout << "Error: generating the wrong number of packets after processing packet 3c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt2c->decRefCount()) delete pkt2c;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	return true;
}

bool trex_testing::testConsumptionJoin() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kindEach = EACH_WITHIN;
	CompKind kindLast = LAST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kindEach);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kindLast);
	pkt->addConsuming(2);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	strcpy(att.name, "V");
	att.type = INT;
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	PubPkt *pkt1c = new PubPkt(3, &att, 1);
	pkt1c->setTime(7);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(6);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);
	pkt3c->setTime(8);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating messages after processing packet 3b (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1c, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt1c->decRefCount()) delete pkt1c;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	return true;
}

bool trex_testing::testConsumptionJoin2() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kindEach = EACH_WITHIN;
	CompKind kindLast = LAST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kindEach);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kindLast);
	pkt->addConsuming(2);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 3;
	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	PubPkt *pkt1c = new PubPkt(3, &att, 1);
	pkt1c->setTime(6);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(7);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);
	pkt3c->setTime(8);
	PubPkt *pkt3d = new PubPkt(1, &att, 1);
	pkt3d->setTime(9);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1c, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating messages after processing packet 3b (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3c!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3d, *mh);
	sr->processPkt(pkt3d, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating messages after processing packet 3d!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt1c->decRefCount()) delete pkt1c;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	if (pkt3d->decRefCount()) delete pkt3d;
	return true;
}

bool trex_testing::testConsumptionJoin3() {
	RulePkt *pkt = new RulePkt(true);
	Constraint c1[1];
	Constraint c2[1];
	Constraint c3[1];
	TimeMs win = 10;
	CompKind kindEach = EACH_WITHIN;
	CompKind kindLast = LAST_WITHIN;
	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	buildConstraint(c1, 1);
	pkt->addRootPredicate(1, c1, 1);
	buildConstraint(c2, 2);
	pkt->addPredicate(2, c2, 1, 0, win, kindEach);
	buildConstraint(c3, 3);
	pkt->addPredicate(3, c3, 1, 0, win, kindLast);
	pkt->addConsuming(1);
	pkt->addConsuming(2);
	pkt->setCompositeEventTemplate(ceTemplate);

	StacksRule *sr = new StacksRule(pkt);
	IndexingTable *indexingTable = new IndexingTable();
	indexingTable->installRulePkt(pkt);

	Attribute att;
	att.type = INT;
	strcpy(att.name, "V");
	att.intVal = 3;

	PubPkt *pkt1 = new PubPkt(3, &att, 1);
	pkt1->setTime(1);
	PubPkt *pkt1b = new PubPkt(3, &att, 1);
	pkt1b->setTime(2);
	PubPkt *pkt1c = new PubPkt(3, &att, 1);
	pkt1c->setTime(6);
	att.intVal = 2;
	PubPkt *pkt2 = new PubPkt(2, &att, 1);
	pkt2->setTime(3);
	PubPkt *pkt2b = new PubPkt(2, &att, 1);
	pkt2b->setTime(4);
	att.intVal = 1;
	PubPkt *pkt3 = new PubPkt(1, &att, 1);
	pkt3->setTime(5);
	PubPkt *pkt3b = new PubPkt(1, &att, 1);
	pkt3b->setTime(7);
	PubPkt *pkt3c = new PubPkt(1, &att, 1);
	pkt3c->setTime(8);

	MatchingHandler *mh = new MatchingHandler();
	set<PubPkt *> results;

	indexingTable->processMessage(pkt1, *mh);
	sr->processPkt(pkt1, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1b, *mh);
	sr->processPkt(pkt1b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2, *mh);
	sr->processPkt(pkt2, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt2b, *mh);
	sr->processPkt(pkt2b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 2b!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3, *mh);
	sr->processPkt(pkt3, mh, results, 0);
	if (results.size()!=2) {
		cout << "Error: generating the wrong number of packets after processing packet 3!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt1c, *mh);
	sr->processPkt(pkt1c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating results before the end of the sequence, after processing of packet 1c!" << endl;
		return false;
	}

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3b, *mh);
	sr->processPkt(pkt3b, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating messages after processing packet 3b (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	mh = new MatchingHandler();
	indexingTable->processMessage(pkt3c, *mh);
	sr->processPkt(pkt3c, mh, results, 0);
	if (results.size()!=0) {
		cout << "Error: generating messages after processing packet 3c (error in the consuming clause for packet 2)!" << endl;
		return false;
	}
	for (set<PubPkt *>::iterator it=results.begin(); it!=results.end(); ++it) {
		PubPkt *pkt = *it;
		if (pkt->decRefCount()) delete pkt;
	}
	results.clear();

	delete mh;
	delete sr;
	delete indexingTable;
	delete pkt;
	if (pkt1->decRefCount()) delete pkt1;
	if (pkt1b->decRefCount()) delete pkt1b;
	if (pkt1c->decRefCount()) delete pkt1c;
	if (pkt2->decRefCount()) delete pkt2;
	if (pkt2b->decRefCount()) delete pkt2b;
	if (pkt3->decRefCount()) delete pkt3;
	if (pkt3b->decRefCount()) delete pkt3b;
	if (pkt3c->decRefCount()) delete pkt3c;
	return true;
}

void trex_testing::buildConstraint(Constraint *constraints, int val) {
	constraints[0].name[0] = 'V';
	constraints[0].name[1] = '\0';
	constraints[0].op = EQ;
	constraints[0].type = INT;
	constraints[0].intVal = val;
}
