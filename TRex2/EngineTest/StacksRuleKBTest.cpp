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

bool trex_testing::stacksRuleKBTest() {
	cout << "*** StacksRule ***" << endl;
	cout << "TestKBSequence\t";
	if (! testKBSequence()) return false;
	cout << "OK" << endl << endl;
	return true;
}

bool trex_testing::testKBSequence() {
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
