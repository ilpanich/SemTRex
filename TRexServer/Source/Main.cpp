/*
 * Copyright (C) 2011 Francesco Feltrinelli <first_name DOT last_name AT gmail DOT com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "external.hpp"
#include "server.hpp"
#include "test.hpp"
#include "util.hpp"

using concept::server::SOEPServer;
using namespace concept::test;
using concept::util::Logging;
using namespace std;

void addBootstrapTestRules(TRexEngine& engine){
	//RuleStoppedPosition stoppedPos;
	//RuleGathering gathering;
	//engine.processRulePkt(stoppedPos.buildRule());
	//engine.processRulePkt(gathering.buildRule());
	//RuleGathering2 gathering2;
	//engine.processRulePkt(gathering2.buildRule());
	//RuleGathering3 gathering3;
	//engine.processRulePkt(gathering3.buildRule());
	//RuleEmanuele ruleEm;
	//engine.processRulePkt(ruleEm.buildRule());
	RuleTheft ruleTheft;
	engine.processRulePkt(ruleTheft.buildRule());
	RuleTheft2 ruleTheft2;
	engine.processRulePkt(ruleTheft2.buildRule());
	RuleNotGivenBack ruleNotGivenBack;
	engine.processRulePkt(ruleNotGivenBack.buildRule());
	RuleOpenDoors ruleOpenDoors;
	engine.processRulePkt(ruleOpenDoors.buildRule());
	engine.finalize();
}

void runServer(){
	// Create server with default port and #threads = #CPUs
	SOEPServer server(SOEPServer::DEFAULT_PORT, boost::thread::hardware_concurrency(), false);

	// add some test rules
	addBootstrapTestRules(server.getEngine());

	server.run();
}

void testEngine(){
	TRexEngine engine(2);
	RuleR1 testRule;

	engine.processRulePkt(testRule.buildRule());
	engine.finalize();

	ResultListener* listener= new TestResultListener(testRule.buildSubscription());
	engine.addResultListener(listener);

	vector<PubPkt*> pubPkts= testRule.buildPublication();
	for (vector<PubPkt*>::iterator it= pubPkts.begin(); it != pubPkts.end(); it++){
		engine.processPubPkt(*it);
	}
	/* Expected output: complex event should be created by T-Rex and published
	 * to the TestResultListener, which should print it to screen.
	 */
}

int main(){
	Logging::init();
	runServer();
}
