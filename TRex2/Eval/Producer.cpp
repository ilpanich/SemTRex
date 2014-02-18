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

#include "Producer.h"

using namespace std;

Producer::Producer(ParamHandler *parParamHandler) {
	paramHandler = parParamHandler;
}

Producer::~Producer() {
	// Nothing to do
}

PubPkt * Producer::createPubPkt() {
	int caseStudy = paramHandler->getCaseStudy();
	if (caseStudy==FIRE_STUDY || caseStudy==AGGREGATE_STUDY) return createFirePkt();
	if (caseStudy==RAIN_STUDY) return createRainPkt();
	if (caseStudy==LENGTH_STUDY || caseStudy==WIDTH_STUDY) return createLengthPkt();
	if (caseStudy==SELECTION_STUDY) return createSelectionPkt();
	if (caseStudy==KB_STUDY) return createKbPkt();
	int eventType = getRandomEventType();
	Attribute attributes[paramHandler->getNumAttributes()];
	initAttributes(attributes, paramHandler->getNumAttributes());
	PubPkt *pkt = new PubPkt(eventType, attributes, paramHandler->getNumAttributes());
	return pkt;
}

int Producer::getRandomEventType() {
	return (rand()%paramHandler->getNumEventTypes())+1;
}

void Producer::initAttributes(Attribute *attributes, int attributesNum) {
	set<int> alreadyUsed;
	int num = (rand()%paramHandler->getNumNames());
	alreadyUsed.insert(num);
	attributes[0].name[0] = 'A' + num;
	attributes[0].name[1] = '\0';
	attributes[0].type = INT;
	attributes[0].intVal = (rand()%paramHandler->getNumValues())+1;
	for (int i=1; i<attributesNum; ) {
		num = (rand()%paramHandler->getNumNames());
		if (alreadyUsed.find(num)!=alreadyUsed.end()) continue;
		alreadyUsed.insert(num);
		attributes[i].name[0] = 'A' + num;
		attributes[i].name[1] = '\0';
		attributes[i].type = INT;
		attributes[i].intVal = (rand()%paramHandler->getNumValues())+1;
		i++;
	}
}

PubPkt * Producer::createFirePkt() {
	int smokeType = (rand()%paramHandler->getNumDefinitions())+1;
	int r = (rand()%100)+1;
	PubPkt *pkt;
	if (r<=paramHandler->getSmokePerc()) {
		Attribute attr[1];
		attr[0].name[0] = 'S';
		attr[0].name[1] = '\0';
		attr[0].type = INT;
		attr[0].intVal = 1;
		pkt = new PubPkt(smokeType, attr, 1);
	} else {
		Attribute attr[1];
		attr[0].name[0] = 'T';
		attr[0].name[1] = '\0';
		attr[0].type = INT;
		attr[0].intVal = (rand()%100)+1;
		pkt = new PubPkt(smokeType+1000, attr, 1);
	}
	return pkt;
}

PubPkt * Producer::createRainPkt() {
	int rainType = (rand()%paramHandler->getNumDefinitions())+1;
	int r = (rand()%100)+1;
	PubPkt *pkt;
	if (r<=paramHandler->getRainPerc()) {
		// Area attribute
		Attribute attr[1];
		attr[0].name[0] = 'A';
		attr[0].name[1] = '\0';
		attr[0].type = INT;
		attr[0].intVal = 1;
		pkt = new PubPkt(rainType, attr, 1);
	} else {
		Attribute attr[2];
		// Temp attribute
		attr[0].name[0] = 'T';
		attr[0].name[1] = '\0';
		attr[0].type = INT;
		attr[0].intVal = (rand()%100)+1;
		// Area attribute
		attr[1].name[0] = 'A';
		attr[1].name[1] = '\0';
		attr[1].type = INT;
		attr[1].intVal = 1;
		pkt = new PubPkt(rainType+1000, attr, 2);
	}
	return pkt;
}

PubPkt * Producer::createLengthPkt() {
	int type = (rand()%paramHandler->getNumDefinitions())+1;
	int state = rand()%paramHandler->getNumRulePredicates();
	Attribute attr[1];
	attr[0].name[0] = 'V';
	attr[0].name[1] = '\0';
	attr[0].type = INT;
	attr[0].intVal = 1;
	PubPkt *pkt = new PubPkt(type*1000+state, attr, 1);
	return pkt;
}

PubPkt * Producer::createSelectionPkt() {
	int type = 1;
	Attribute attr[1];
	attr[0].name[0] = 'V';
	attr[0].name[1] = '\0';
	attr[0].type = INT;
	attr[0].intVal = (rand()%paramHandler->getNumRules())+1;
	PubPkt *pkt = new PubPkt(type, attr, 1);
	return pkt;
}

PubPkt * Producer::createKbPkt() {
	int smokeType = (rand()%paramHandler->getNumDefinitions())+1;
	int r = (rand()%100)+1;
	string names[] = {"Stanley Holloway","Jerry Springer","Will Self","Ernest Thesiger","Peter Ackroyd","Mary Wollstonecraft Shelley","Mary Shelley","Alan M. Turing","Virginia Woolf","Beniaminus Disraeli","Davidas Rikardas","Michael Moorcock","Gilbert Keith Chesterton","Alistair Darling","Horace Walpole","Harold Alexander","John Donne","William Blake","Christopher Ingold","Neil Ross","Pops Mensah-Bonsu","Peter Cheyney","Kathryn Beaumont","Kelenna Azubuike","Carlos Raúl Villanueva","Michael Woodruff","Sean Yazbeck","Layla El","Alfred James Shaughnessy","Richard Harvey","John Sebastian Helmcken","David Boadella","Terry Fox","Clara Hughes","Dufferin Roblin","Gary Doer","David Reimer","James Coyne","Andy Bathgate","Mike Keane","Alexander Steen","Raymond Henault","Steve Corino","Bill Masterton","Ted Irvine","Ted Harris","Shannon Rempel","Reg Abbott","Jonathan Toews","Paul Baxter","John Marks (hockey)","Bruno Zarrillo","Lonny Bohonos","Travis Zajac","Frank Mathers","Dustin Boyd","Jennifer Ellison","Alfred Lennon","Mal Evans","Stephen Baxter","Gulielmus Ewart Gladstone","William Gladstone","Clive Barker","John Horton Conway","John Conway","Felicia Hemans","Andy Burnham","James Bulger","Mumes Bulger","James Larkin","Frank Hornby","Cathy Tyson","Augustus Radcliffe Grote","Neil Buchanan","Stephen Molyneux","Julia Lennon","Alfred Cheetham","John Redwood","Edward Pellew"};
	string cities[] = {"London","Winnipeg","Dover","Liverpool","Cambridge"};
	int n = rand() % 79;
	int c = rand() % 5;
	PubPkt *pkt;
	Attribute attr[4];
	attr[0].name[0] = 'S';
	attr[0].name[1] = '\0';
	attr[0].type = INT;
	attr[0].intVal = 1;
	attr[1].name[0] = 'n';
	attr[1].name[1] = 'a';
	attr[1].name[2] = 'm';
	attr[1].name[3] = 'e';
	attr[1].name[4] = '\0';
	attr[1].type = STRING;
	strcpy(attr[1].stringVal, names[n].c_str());
	attr[2].name[0] = 'c';
	attr[2].name[1] = 'i';
	attr[2].name[2] = 't';
	attr[2].name[3] = 'y';
	attr[2].name[4] = '\0';
	attr[2].type = STRING;
	strcpy(attr[2].stringVal, cities[c].c_str());
	attr[3].name[0] = 'T';
	attr[3].name[1] = '\0';
	attr[3].type = INT;
	attr[3].intVal = (rand()%100)+1;
	pkt = new PubPkt(smokeType, attr, 4);

	return pkt;
}
