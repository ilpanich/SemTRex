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

#include <stdlib.h>
#include <stdio.h>
#include "EngineTest/RunTest.h"
#include "Eval/RunEval.h"
#include "CommunicationTest/EchoClient.h"
#include "Examples/SimpleServer.h"

using namespace trex_testing;
using namespace std;

void usage() {
	cout << "Usage: TRex [-server [<port>] | -eval | -test | -commTest]" << endl;
	cout << "-server   -> run a T-Rex server listening for clients on the specified port (default: 9999)" << endl;
	cout << "-eval     -> run performance evaluation of the T-Rex engine (may take several hours)" << endl;
	cout << "-test     -> run testing for the T-Rex engine" << endl;
	cout << "-commTest -> run mashalling and communication testing (requires a running Java TRexServer)" << endl;
	exit (-1);
}

int main(int argc, char* argv[]) {
	if (argc==2 && strcmp(argv[1], "-test")==0) runTest();
	else if (argc==2 && strcmp(argv[1], "-eval")==0) runEval();
	else if (argc==3 && strcmp(argv[1], "-beval")==0) runBasicEval(atoi(argv[2]));
	else if (argc==2 && strcmp(argv[1], "-commTest")==0) launchEchoClient();
	else if (argc>=2 && argc<=3 && strcmp(argv[1], "-server")==0) {
		int port = 9999;
		if (argc==3) port = atoi(argv[2]);
		SimpleServer server(port);
		server.start();
	} else usage();
	return 0;
}

