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

#include "SimpleServer.h"

SimpleServer::SimpleServer(int port) {
	table = new SimpleRoutingTable();
	engine = new TRexEngine(1);
	installRules();
	server = new TRexServer(engine, port, table, 1000);
}

SimpleServer::~SimpleServer() {
	server->stop();
	delete server;
	delete table;
	delete engine;
}

void SimpleServer::start() {
	server->startListening();
}

void SimpleServer::installRules() {
	RulePkt *rule = new RulePkt(true);
	CompositeEventTemplate *eventTemplate = new CompositeEventTemplate(20);
	rule->setCompositeEventTemplate(eventTemplate);
	Constraint constr[1];
	strcpy(constr[0].name, "Name");
	constr[0].type = INT;
	constr[0].op = EQ;
	constr[0].intVal = 10;
	rule->addRootPredicate(10, constr, 1);
	engine->processRulePkt(rule);
	engine->finalize();
}
