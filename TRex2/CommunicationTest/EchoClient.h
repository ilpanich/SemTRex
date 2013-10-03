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

#ifndef ECHOCLIENT_H_
#define ECHOCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../Marshalling/Marshaller.h"
#include "../Marshalling/Unmarshaller.h"
#include "../Common/Consts.h"

#define PORT 9999
#define NUM_PACKETS 40

namespace trex_testing {

/**
 * This Client is used to test serialization with external clients:
 * for example it can be used with the Java server.
 */
void launchEchoClient();

}

#endif /* ECHOCLIENT_H_ */
