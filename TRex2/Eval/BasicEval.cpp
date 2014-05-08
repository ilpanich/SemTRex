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

#include "BasicEval.h"
#include <unistd.h>

using namespace std;

BasicEval::BasicEval(bool testType) {
	engine = new TRexEngine(2);
	resultListener = new EvalResultListener;

	evalType = testType;

	set<RulePkt *> rulePkts;
	if (!evalType)
		createParamRules(rulePkts);
	else
		createKbRules(rulePkts);
	for (set<RulePkt *>::iterator it=rulePkts.begin(); it!=rulePkts.end(); ++it) {
		RulePkt *pkt = *it;
		engine->processRulePkt(pkt);
	}

	engine->finalize();
	engine->addResultListener(resultListener);

}

BasicEval::~BasicEval() {

	delete engine;
}


void BasicEval::createKbRules(set<RulePkt *> &rules) {
	int id = rand() % 10;

	for (int i = 0; i < 100; i++) {
		RulePkt *pkt = new RulePkt(true);
		int q = rand() % 8;
		string names[] = {"Stanley Holloway","Jerry Springer","Will Self","Ernest Thesiger","Peter Ackroyd","Mary Wollstonecraft Shelley","Mary Shelley","Alan M. Turing","Virginia Woolf","Beniaminus Disraeli","Davidas Rikardas","Michael Moorcock","Gilbert Keith Chesterton","Alistair Darling","Horace Walpole","Harold Alexander","John Donne","William Blake","Christopher Ingold","Neil Ross","Pops Mensah-Bonsu","Peter Cheyney","Kathryn Beaumont","Kelenna Azubuike","Carlos Raúl Villanueva","Michael Woodruff","Sean Yazbeck","Layla El","Alfred James Shaughnessy","Richard Harvey","John Sebastian Helmcken","David Boadella","Terry Fox","Clara Hughes","Dufferin Roblin","Gary Doer","David Reimer","James Coyne","Andy Bathgate","Mike Keane","Alexander Steen","Raymond Henault","Steve Corino","Bill Masterton","Ted Irvine","Ted Harris","Shannon Rempel","Reg Abbott","Jonathan Toews","Paul Baxter","John Marks (hockey)","Bruno Zarrillo","Lonny Bohonos","Travis Zajac","Frank Mathers","Dustin Boyd","Jennifer Ellison","Alfred Lennon","Mal Evans","Stephen Baxter","Gulielmus Ewart Gladstone","William Gladstone","Clive Barker","John Horton Conway","John Conway","Felicia Hemans","Andy Burnham","James Bulger","Mumes Bulger","James Larkin","Frank Hornby","Cathy Tyson","Augustus Radcliffe Grote","Neil Buchanan","Stephen Molyneux","Julia Lennon","Alfred Cheetham","John Redwood","Edward Pellew"};
		string cities[] = {"London","Winnipeg","Dover","Liverpool","Cambridge"};
		int n = rand() % 79;
		int c = rand() % 5;

		string queries[] = {"select ?name where { ?p <isCalled> ?name }", "select ?city where { ?p <bornInLocation> ?city }",
				"select ?name ?city where { ?p <isCalled> ?name. ?p <bornInLocation> ?city }",
				"select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> &city }",
				"select ?city where { ?p <isCalled> &name. ?p <bornInLocation> ?city }",
				"select ?name ?city where { ?p <isCalled> &name. ?p <bornInLocation> &city }",
				"select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> \"" + cities[c] + "\" }",
				"select ?city where { ?p <isCalled> \"" + names[n] + "\". ?p <bornInLocation> ?city }"};

		if (q == 0 || q == 3 || q == 6) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			pkt->addKBRootPredicate(NULL, 0, "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db", queries[q]);
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

			if (q == 3) {
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
				strcpy(ep->name1, ext_param1name);
				strcpy(ep->name2, ext_param2name);
				ep->evIndex1 = 0;
				ep->evIndex2 = 0;
				ep->seqId1 = 0;
				ep->seqId2 = 0;

				pkt->addExtParamToKBPred(ep);
			}
		}
		if (q == 1 || q == 4 || q == 7) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			pkt->addKBRootPredicate(NULL, 0, "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db", queries[q]);
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

			if (q == 4) {
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
			}
		}

		if (q == 2 || q == 5) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			pkt->addKBRootPredicate(NULL, 0, "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db", queries[q]);
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

			if (q == 5) {
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

				ep = new ExtParameter();

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
			}
		}

		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);

	}
}

void BasicEval::createParamRules(set<RulePkt *> &rules) {
	int id = rand() % 10;

	for(int i = 0; i < 100; i++) {
		RulePkt *pkt = new RulePkt(true);
		int q = rand() % 6;

		if (q == 0) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = EACH_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);

			//			cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param: " << pkt->getParameter(0).evIndex1 << "." << string(pkt->getParameter(0).name1) << " = " << pkt->getParameter(0).evIndex2 << "." << string(pkt->getParameter(0).name2) << " (" << pkt->getParameter(0).type << ")" << endl;
		}
		if (q == 1) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = EACH_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			param1name[0] = 'c';
			param1name[1] = 'i';
			param1name[2] = 't';
			param1name[3] = 'y';
			param1name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);

			//			cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param: " << pkt->getParameter(0).evIndex1 << "." << string(pkt->getParameter(0).name1) << " = " << pkt->getParameter(0).evIndex2 << "." << string(pkt->getParameter(0).name2) << " (" << pkt->getParameter(0).type << ")" << endl;
		}
		if (q == 2) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = EACH_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			char param2name[5];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = '\0';

			param2name[0] = 'c';
			param2name[1] = 'i';
			param2name[2] = 't';
			param2name[3] = 'y';
			param2name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);
			pkt->addParameterBetweenStates(0, param2name, 1, param2name);

			//			cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param1: " << pkt->getParameter(0).evIndex1 << "." << pkt->getParameter(0).name1 << " = " << pkt->getParameter(0).evIndex2 << "." << pkt->getParameter(0).name2 << " (" << pkt->getParameter(0).type << ")" << " param2: " << pkt->getParameter(1).evIndex1 << "." << pkt->getParameter(1).name1 << " = " << pkt->getParameter(1).evIndex2 << "." << pkt->getParameter(1).name2 << " (" << pkt->getParameter(1).type << ")" << endl;
		}

		if (q == 3) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = LAST_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);

			//					cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param: " << pkt->getParameter(0).evIndex1 << "." << string(pkt->getParameter(0).name1) << " = " << pkt->getParameter(0).evIndex2 << "." << string(pkt->getParameter(0).name2) << " (" << pkt->getParameter(0).type << ")" << endl;
		}
		if (q == 4) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = LAST_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			param1name[0] = 'c';
			param1name[1] = 'i';
			param1name[2] = 't';
			param1name[3] = 'y';
			param1name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);

			//					cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param: " << pkt->getParameter(0).evIndex1 << "." << string(pkt->getParameter(0).name1) << " = " << pkt->getParameter(0).evIndex2 << "." << string(pkt->getParameter(0).name2) << " (" << pkt->getParameter(0).type << ")" << endl;
		}
		if (q == 5) {
			pkt->addRootPredicate(id*1000, NULL, 0);
			TimeMs win = 12*60*60*1000;
			CompKind kind = LAST_WITHIN;
			pkt->addPredicate(id*1000+1, NULL, 0, 0, win, kind);
			char param1name[5];
			char param2name[5];
			param1name[0] = 'n';
			param1name[1] = 'a';
			param1name[2] = 'm';
			param1name[3] = 'e';
			param1name[4] = '\0';

			param2name[0] = 'c';
			param2name[1] = 'i';
			param2name[2] = 't';
			param2name[3] = 'y';
			param2name[4] = '\0';

			pkt->addParameterBetweenStates(0, param1name, 1, param1name);
			pkt->addParameterBetweenStates(0, param2name, 1, param2name);

			//					cout << pkt->getPredicate(0).eventType << " -> " << pkt->getPredicate(1).eventType << " param1: " << pkt->getParameter(0).evIndex1 << "." << pkt->getParameter(0).name1 << " = " << pkt->getParameter(0).evIndex2 << "." << pkt->getParameter(0).name2 << " (" << pkt->getParameter(0).type << ")" << " param2: " << pkt->getParameter(1).evIndex1 << "." << pkt->getParameter(1).name1 << " = " << pkt->getParameter(1).evIndex2 << "." << pkt->getParameter(1).name2 << " (" << pkt->getParameter(1).type << ")" << endl;
		}

		CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
		pkt->setCompositeEventTemplate(ceTemplate);
		rules.insert(pkt);

	}
}

PubPkt * BasicEval::createParamPkt1() {
	int r = rand();
	int id = (r % 10) * 1000 +1;
	string names[] = {"Stanley Holloway","Jerry Springer","Will Self","Ernest Thesiger","Peter Ackroyd","Mary Wollstonecraft Shelley","Mary Shelley","Alan M. Turing","Virginia Woolf","Beniaminus Disraeli","Davidas Rikardas","Michael Moorcock","Gilbert Keith Chesterton","Alistair Darling","Horace Walpole","Harold Alexander","John Donne","William Blake","Christopher Ingold","Neil Ross","Pops Mensah-Bonsu","Peter Cheyney","Kathryn Beaumont","Kelenna Azubuike","Carlos Raúl Villanueva","Michael Woodruff","Sean Yazbeck","Layla El","Alfred James Shaughnessy","Richard Harvey","John Sebastian Helmcken","David Boadella","Terry Fox","Clara Hughes","Dufferin Roblin","Gary Doer","David Reimer","James Coyne","Andy Bathgate","Mike Keane","Alexander Steen","Raymond Henault","Steve Corino","Bill Masterton","Ted Irvine","Ted Harris","Shannon Rempel","Reg Abbott","Jonathan Toews","Paul Baxter","John Marks (hockey)","Bruno Zarrillo","Lonny Bohonos","Travis Zajac","Frank Mathers","Dustin Boyd","Jennifer Ellison","Alfred Lennon","Mal Evans","Stephen Baxter","Gulielmus Ewart Gladstone","William Gladstone","Clive Barker","John Horton Conway","John Conway","Felicia Hemans","Andy Burnham","James Bulger","Mumes Bulger","James Larkin","Frank Hornby","Cathy Tyson","Augustus Radcliffe Grote","Neil Buchanan","Stephen Molyneux","Julia Lennon","Alfred Cheetham","John Redwood","Edward Pellew"};
	string cities[] = {"London","Winnipeg","Dover","Liverpool","Cambridge"};
	int n = r % 79;
	int c = r % 5;
	PubPkt *pkt;
	Attribute attr[2];
	attr[0].name[0] = 'n';
	attr[0].name[1] = 'a';
	attr[0].name[2] = 'm';
	attr[0].name[3] = 'e';
	attr[0].name[4] = '\0';
	attr[0].type = STRING;
	strcpy(attr[0].stringVal, names[n].c_str());
	attr[1].name[0] = 'c';
	attr[1].name[1] = 'i';
	attr[1].name[2] = 't';
	attr[1].name[3] = 'y';
	attr[1].name[4] = '\0';
	attr[1].type = STRING;
	strcpy(attr[1].stringVal, cities[c].c_str());
	pkt = new PubPkt(id, attr, 2);

	return pkt;
}

PubPkt * BasicEval::createParamPkt2() {
	int id = (rand() % 10) * 1000;
	string names[] = {"Stanley Holloway","Jerry Springer","Will Self","Ernest Thesiger","Peter Ackroyd","Mary Wollstonecraft Shelley","Mary Shelley","Alan M. Turing","Virginia Woolf","Beniaminus Disraeli","Davidas Rikardas","Michael Moorcock","Gilbert Keith Chesterton","Alistair Darling","Horace Walpole","Harold Alexander","John Donne","William Blake","Christopher Ingold","Neil Ross","Pops Mensah-Bonsu","Peter Cheyney","Kathryn Beaumont","Kelenna Azubuike","Carlos Raúl Villanueva","Michael Woodruff","Sean Yazbeck","Layla El","Alfred James Shaughnessy","Richard Harvey","John Sebastian Helmcken","David Boadella","Terry Fox","Clara Hughes","Dufferin Roblin","Gary Doer","David Reimer","James Coyne","Andy Bathgate","Mike Keane","Alexander Steen","Raymond Henault","Steve Corino","Bill Masterton","Ted Irvine","Ted Harris","Shannon Rempel","Reg Abbott","Jonathan Toews","Paul Baxter","John Marks (hockey)","Bruno Zarrillo","Lonny Bohonos","Travis Zajac","Frank Mathers","Dustin Boyd","Jennifer Ellison","Alfred Lennon","Mal Evans","Stephen Baxter","Gulielmus Ewart Gladstone","William Gladstone","Clive Barker","John Horton Conway","John Conway","Felicia Hemans","Andy Burnham","James Bulger","Mumes Bulger","James Larkin","Frank Hornby","Cathy Tyson","Augustus Radcliffe Grote","Neil Buchanan","Stephen Molyneux","Julia Lennon","Alfred Cheetham","John Redwood","Edward Pellew"};
	string cities[] = {"London","Winnipeg","Dover","Liverpool","Cambridge"};
	int n = rand() % 79;
	int c = rand() % 5;
	PubPkt *pkt;
	Attribute attr[2];
	attr[0].name[0] = 'n';
	attr[0].name[1] = 'a';
	attr[0].name[2] = 'm';
	attr[0].name[3] = 'e';
	attr[0].name[4] = '\0';
	attr[0].type = STRING;
	strcpy(attr[0].stringVal, names[n].c_str());
	attr[1].name[0] = 'c';
	attr[1].name[1] = 'i';
	attr[1].name[2] = 't';
	attr[1].name[3] = 'y';
	attr[1].name[4] = '\0';
	attr[1].type = STRING;
	strcpy(attr[1].stringVal, cities[c].c_str());
	pkt = new PubPkt(id, attr, 2);

	return pkt;
}

int BasicEval::startBasicEval() {

	time_t bTime, eTime;
	double duration;

	cout << endl << "### Starting evaluation ###" << endl << endl;

	vector<PubPkt *> pubs;
	if (!evalType) {
		for(int i = 0; i < 1000; i++) {
			PubPkt * pack1 = createParamPkt1();
			pack1->setTime(i);
			pubs.push_back(pack1);
		}
		for(int i = 1000; i < 2000; i++) {
			PubPkt * pack2 = createParamPkt2();
			pack2->setTime(i);
			pubs.push_back(pack2);
		}
	} else {
		for(int i = 1000; i < 3500; i++) {
			PubPkt * pack2 = createParamPkt2();
			pack2->setTime(i);
			pubs.push_back(pack2);
		}
	}

	bTime = time(NULL);

	for (vector<PubPkt *>::iterator it = pubs.begin(); it != pubs.end(); it++) {
		PubPkt * pkt = *it;
		//		cout << "Type: " << pkt->getEventType() << "\tAttr1: " << pkt->getAttribute(0).stringVal << "\tAttr2: " << pkt->getAttribute(1).stringVal << endl;
		engine->processPubPkt(pkt);
	}

	eTime = time(NULL);

	duration = eTime - bTime;

	resultListener->printDetectedEvents(evalType,"./dectResults",1,1);
	resultListener->printMaxProcTime(evalType,"./maxTime",1,1);
	resultListener->printMinProcTime(evalType,"./minTime",1,1);
	resultListener->printMeanProcTime(evalType,"./meanTime",1,1);
	resultListener->printPercProcTime(evalType,"./PercTime",1,1);
	resultListener->printToFile(evalType,"./throughput",duration,1,1);

	cout << endl << endl << "### Evaluation finished ###" << endl << endl;
	return 0;
}

/*
void BasicEval::createParamRules(set<RulePkt *> &rules) {

	RulePkt *pkt = new RulePkt(true);

	pkt->addRootPredicate(1000, NULL, 0);

	TimeMs win = 12*60*60*1000;
	CompKind kind = EACH_WITHIN;

	pkt->addPredicate(1001, NULL, 0, 0, win, kind);

	CompositeEventTemplate *ceTemplate = new CompositeEventTemplate(10);
	pkt->setCompositeEventTemplate(ceTemplate);
	rules.insert(pkt);

}

PubPkt * BasicEval::createParamPkt1() {
	int id = 1000;
	PubPkt *pkt;
	pkt = new PubPkt(id, NULL, 0);

	return pkt;
}
PubPkt * BasicEval::createParamPkt2() {
	int id = 1001;
	PubPkt *pkt;
	pkt = new PubPkt(id, NULL, 0);

	return pkt;
}

int BasicEval::startBasicEval() {

	cout << endl << "### Starting evaluation ###" << endl << endl;

	vector<PubPkt *> pubs;
	for(int i = 0; i < 10; i++) {
		PubPkt * pack2 = createParamPkt2();
		pack2->setTime(i);
		pubs.push_back(pack2);
	}

	for(int i = 10; i < 20; i++) {
		PubPkt * pack1 = createParamPkt1();
		pack1->setTime(i);
		pubs.push_back(pack1);
	}

	for (vector<PubPkt *>::iterator it = pubs.begin(); it != pubs.end(); it++) {
		PubPkt * pkt = *it;
		cout << "Type: " << pkt->getEventType() << "\tAttr1: " << pkt->getAttribute(0).stringVal << "\tAttr2: " << pkt->getAttribute(1).stringVal << endl;
		engine->processPubPkt(pkt);
	}

	resultListener->printDetectedEvents(0,"./dectResults",1,1);
	resultListener->printMaxProcTime(0,"./maxTime",1,1);
	resultListener->printMinProcTime(0,"./minTime",1,1);
	resultListener->printMeanProcTime(0,"./meanTime",1,1);
	resultListener->printPercProcTime(0,"./PercTime",1,1);

	cout << endl << endl << "### Evaluation finished ###" << endl << endl;
	return 0;
}
 */
