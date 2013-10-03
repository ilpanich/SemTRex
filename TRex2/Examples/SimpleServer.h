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

#ifndef SIMPLESERVER_H_
#define SIMPLESERVER_H_

#include "../Communication/SimpleRoutingTable.h"
#include "../Communication/TRexServer.h"
#include "../Engine/TRexEngine.h"

/**
 * This example creates a simple server with only one installed rules.
 * The rules accepts an event of type 10 with constraint "Name"=10 and
 * creates an event of type 20 without attributes.
 */
class SimpleServer {
public:

	/**
	 * Creates the server listening on the given port
	 */
	SimpleServer(int port);


	virtual ~SimpleServer();

	/**
	 * Starts the server
	 */
	void start();

private:
	SimpleRoutingTable *table;
	TRexEngine *engine;
	TRexServer *server;

	/**
	 * Create and install the rules on the engine
	 */
	void installRules();

};

#endif /* SIMPLESERVER_H_ */
