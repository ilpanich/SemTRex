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

#include "FilteringTest.h"

using namespace std;

bool trex_testing::filteringTest() {
	cout << "*** Filtering ***" << endl;
	cout << "intFilteringTest\t";
	if (! intFilteringTest()) return false;
	cout << "OK" << endl << "floatFilteringTest\t";
	if (! floatFilteringTest()) return false;
	cout << "OK" << endl << "boolFilteringTest\t";
	if (! boolFilteringTest()) return false;
	cout << "OK" << endl << "stringFilteringTest\t";
	if (! stringFilteringTest()) return false;
	cout << "OK" << endl;
	cout << "\nAll filtering tests ran successfully!\n" << endl;
	cout << endl;
	return true;
}

bool trex_testing::intFilteringTest() {
	IndexingTable *t1 = new IndexingTable();
	IndexingTable *t2 = new IndexingTable();
	IndexingTable *t3 = new IndexingTable();
	IndexingTable *t4 = new IndexingTable();
	RulePkt *pkt1 = new RulePkt(true);
	RulePkt *pkt2 = new RulePkt(true);
	RulePkt *pkt3 = new RulePkt(true);
	RulePkt *pkt4 = new RulePkt(true);
	Constraint c1, c2, c3, c4;
	int eventType = 1;
	char name[2];
	name[0] = 'A';
	name[1] = '\0';
	c1.type = c2.type = c3.type = c4.type = INT;
	strcpy(c1.name, name);
	strcpy(c2.name, name);
	strcpy(c3.name, name);
	strcpy(c4.name, name);
	c1.op = EQ;
	c2.op = LT;
	c3.op = GT;
	c4.op = DF;
	c1.intVal = c2.intVal = c3.intVal = c4.intVal = 10;
	pkt1->addRootPredicate(eventType, &c1, 1);
	pkt1->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt2->addRootPredicate(eventType, &c2, 1);
	pkt2->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt3->addRootPredicate(eventType, &c3, 1);
	pkt3->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt4->addRootPredicate(eventType, &c4, 1);
	pkt4->setCompositeEventTemplate(new CompositeEventTemplate(1));
	t1->installRulePkt(pkt1);
	t2->installRulePkt(pkt2);
	t3->installRulePkt(pkt3);
	t4->installRulePkt(pkt4);
	Attribute att, wrongAtt;
	att.name[0] = 'A';
	att.name[1] = '\0';
	wrongAtt.name[0] = 'W';
	wrongAtt.name[1] = '\0';
	att.type = wrongAtt.type = INT;
	att.intVal = wrongAtt.intVal = 5;
	PubPkt *okPkt = new PubPkt(eventType, &att, 1);
	PubPkt *wrongPkt = new PubPkt(eventType, &wrongAtt, 1);

	MatchingHandler mh1, mh2, mh3, mh4;
	t1->processMessage(wrongPkt, mh1);
	t2->processMessage(wrongPkt, mh2);
	t3->processMessage(wrongPkt, mh3);
	t4->processMessage(wrongPkt, mh4);
	if (mh1.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t1!" << endl;
		return false;
	}
	if (mh2.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t2!" << endl;
		return false;
	}
	if (mh3.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t3!" << endl;
		return false;
	}
	if (mh4.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t4!" << endl;
		return false;
	}
	mh1.matchingStates.clear();
	mh2.matchingStates.clear();
	mh3.matchingStates.clear();
	mh4.matchingStates.clear();

	t1->processMessage(okPkt, mh1);
	t2->processMessage(okPkt, mh2);
	t3->processMessage(okPkt, mh3);
	t4->processMessage(okPkt, mh4);
	if (mh1.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t1!" << endl;
		return false;
	}
	if (mh2.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t2!" << endl;
		return false;
	}
	if (/* TODO mh2.matchingStates[0]->size()!=1*/ false) {
		cout << "Error: wrong number of sequences generated by table t2!" << endl;
		return false;
	}
	if (/* (*mh2.matchingStates[0])[0].size()!=1*/ false) {
		cout << "Error: wrong number of states generated by table t2!" << endl;
		return false;
	}
	if (mh3.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t3!" << endl;
		return false;
	}
	if (mh4.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t4!" << endl;
		return false;
	}
	if (/* mh4.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t4!" << endl;
		return false;
	}
	if (/* (*mh4.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t4!" << endl;
		return false;
	}

	if (okPkt->decRefCount()) delete okPkt;
	if (wrongPkt->decRefCount()) delete wrongPkt;
	delete pkt1;
	delete pkt2;
	delete pkt3;
	delete pkt4;

	return true;
}

bool trex_testing::floatFilteringTest() {
	IndexingTable *t1 = new IndexingTable();
	IndexingTable *t2 = new IndexingTable();
	IndexingTable *t3 = new IndexingTable();
	IndexingTable *t4 = new IndexingTable();
	RulePkt *pkt1 = new RulePkt(true);
	RulePkt *pkt2 = new RulePkt(true);
	RulePkt *pkt3 = new RulePkt(true);
	RulePkt *pkt4 = new RulePkt(true);
	Constraint c1, c2, c3, c4;
	int eventType = 1;
	char name[2];
	name[0] = 'A';
	name[1] = '\0';
	c1.type = c2.type = c3.type = c4.type = FLOAT;
	strcpy(c1.name, name);
	strcpy(c2.name, name);
	strcpy(c3.name, name);
	strcpy(c4.name, name);
	c1.op = EQ;
	c2.op = LT;
	c3.op = GT;
	c4.op = DF;
	c1.floatVal = c2.floatVal = c3.floatVal = c4.floatVal = 10.5;
	pkt1->addRootPredicate(eventType, &c1, 1);
	pkt1->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt2->addRootPredicate(eventType, &c2, 1);
	pkt2->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt3->addRootPredicate(eventType, &c3, 1);
	pkt3->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt4->addRootPredicate(eventType, &c4, 1);
	pkt4->setCompositeEventTemplate(new CompositeEventTemplate(1));
	t1->installRulePkt(pkt1);
	t2->installRulePkt(pkt2);
	t3->installRulePkt(pkt3);
	t4->installRulePkt(pkt4);
	Attribute att;
	att.name[0] = 'A';
	att.name[1] = '\0';
	att.type = FLOAT;
	att.floatVal = 5.5;
	PubPkt *okPkt = new PubPkt(eventType, &att, 1);

	MatchingHandler mh1, mh2, mh3, mh4;
	t1->processMessage(okPkt, mh1);
	t2->processMessage(okPkt, mh2);
	t3->processMessage(okPkt, mh3);
	t4->processMessage(okPkt, mh4);
	if (mh1.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t1!" << endl;
		return false;
	}
	if (mh2.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t2!" << endl;
		return false;
	}
	if (/* mh2.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t2!" << endl;
		return false;
	}
	if (/* (*mh2.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t2!" << endl;
		return false;
	}
	if (mh3.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t3!" << endl;
		return false;
	}
	if (mh4.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t4!" << endl;
		return false;
	}
	if (/* mh4.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t4!" << endl;
		return false;
	}
	if (/* (*mh4.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t4!" << endl;
		return false;
	}

	if (okPkt->decRefCount()) delete okPkt;
	delete pkt1;
	delete pkt2;
	delete pkt3;
	delete pkt4;

	return true;
}

bool trex_testing::boolFilteringTest() {
	IndexingTable *t1 = new IndexingTable();
	IndexingTable *t2 = new IndexingTable();
	RulePkt *pkt1 = new RulePkt(true);
	RulePkt *pkt2 = new RulePkt(true);
	Constraint c1, c2;
	int eventType = 1;
	char name[2];
	name[0] = 'A';
	name[1] = '\0';
	c1.type = c2.type = BOOL;
	strcpy(c1.name, name);
	strcpy(c2.name, name);
	c1.op = EQ;
	c2.op = DF;
	c1.boolVal = c2.boolVal = true;
	pkt1->addRootPredicate(eventType, &c1, 1);
	pkt1->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt2->addRootPredicate(eventType, &c2, 1);
	pkt2->setCompositeEventTemplate(new CompositeEventTemplate(1));
	t1->installRulePkt(pkt1);
	t2->installRulePkt(pkt2);

	Attribute att;
	att.name[0] = 'A';
	att.name[1] = '\0';
	att.type = BOOL;
	att.boolVal = false;
	PubPkt *okPkt = new PubPkt(eventType, &att, 1);

	MatchingHandler mh1, mh2;
	t1->processMessage(okPkt, mh1);
	t2->processMessage(okPkt, mh2);
	if (mh1.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t1!" << endl;
		return false;
	}
	if (mh2.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t2!" << endl;
		return false;
	}
	if (/* mh2.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t2!" << endl;
		return false;
	}
	if (/* (*mh2.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t2!" << endl;
		return false;
	}

	if (okPkt->decRefCount()) delete okPkt;
	delete pkt1;
	delete pkt2;

	return true;
}

bool trex_testing::stringFilteringTest() {
	IndexingTable *t1 = new IndexingTable();
	IndexingTable *t2 = new IndexingTable();
	IndexingTable *t3 = new IndexingTable();
	RulePkt *pkt1 = new RulePkt(true);
	RulePkt *pkt2 = new RulePkt(true);
	RulePkt *pkt3 = new RulePkt(true);
	Constraint c1, c2, c3;
	int eventType = 1;
	char name[2];
	name[0] = 'A';
	name[1] = '\0';
	c1.type = c2.type = c3.type = STRING;
	strcpy(c1.name, name);
	strcpy(c2.name, name);
	strcpy(c3.name, name);
	c1.op = EQ;
	c2.op = IN;
	c3.op = IN;
	strcpy(c1.stringVal, "V");
	strcpy(c2.stringVal, "V");
	strcpy(c3.stringVal, "X");
	pkt1->addRootPredicate(eventType, &c1, 1);
	pkt1->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt2->addRootPredicate(eventType, &c2, 1);
	pkt2->setCompositeEventTemplate(new CompositeEventTemplate(1));
	pkt3->addRootPredicate(eventType, &c3, 1);
	pkt3->setCompositeEventTemplate(new CompositeEventTemplate(1));
	t1->installRulePkt(pkt1);
	t2->installRulePkt(pkt2);
	t3->installRulePkt(pkt3);
	Attribute att1, att2;
	strcpy(att1.name, "A");
	strcpy(att1.stringVal, "V1");
	att1.type = STRING;
	strcpy(att2.name, "A");
	strcpy(att2.stringVal, "V");
	att2.type = STRING;
	PubPkt *okPkt = new PubPkt(eventType, &att1, 1);
	PubPkt *okPkt2 = new PubPkt(eventType, &att2, 1);

	MatchingHandler mh1, mh2, mh3;
	t1->processMessage(okPkt, mh1);
	t2->processMessage(okPkt, mh2);
	t3->processMessage(okPkt, mh3);
	if (mh1.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t1!" << endl;
		return false;
	}
	mh1.matchingStates.clear();
	t1->processMessage(okPkt2, mh1);
	if (mh1.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t1 for okPkt2!" << endl;
		return false;
	}
	if (/* mh1.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t1 for okPkt2!" << endl;
		return false;
	}
	if (/* (*mh1.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t1 for okPkt2!" << endl;
		return false;
	}
	if (mh2.matchingStates.size()!=1) {
		cout << "Error: wrong number of automata generated by table t2!" << endl;
		return false;
	}
	if (/* mh2.matchingStates[0]->size()!=1 TODO */ false) {
		cout << "Error: wrong number of sequences generated by table t2!" << endl;
		return false;
	}
	if (/* (*mh2.matchingStates[0])[0].size()!=1 TODO */ false) {
		cout << "Error: wrong number of states generated by table t2!" << endl;
		return false;
	}
	if (mh3.matchingStates.size()!=0) {
		cout << "Error: packet should not be used by the indexing table t3!" << endl;
		return false;
	}

	if (okPkt->decRefCount()) delete okPkt;
	if (okPkt2->decRefCount()) delete okPkt2;
	delete pkt1;
	delete pkt2;
	delete pkt3;
	return true;
}
