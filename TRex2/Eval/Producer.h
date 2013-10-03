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

#ifndef PRODUCER_H_
#define PRODUCER_H_

#include <set>
#include "../Packets/PubPkt.h"
#include "../Common/Consts.h"
#include "ParamHandler.h"

/**
 * This class is used to produce publication packets, i.e. to simulate the load for
 * the system according to a set of given parameters.
 */
class Producer {

public:

	/**
	 * Constructor: generates a producer with parameters stored in the given ParamHandler
	 */
	Producer(ParamHandler *parParamHandler);

	/**
	 * Destructor
	 */
	virtual ~Producer();

	/**
	 * Creates a new publication packet
	 */
	PubPkt * createPubPkt();

	/**
	 * Returns a random event type
	 */
	int getRandomEventType();

	/**
	 * Initializes the given set of attributes
	 */
	void initAttributes(Attribute *attributes, int attributesSize);

	/**
	 * Creates a packet for the fire detection test case
	 */
	PubPkt * createFirePkt();

	/**
	 * Crates a packet for the rain test case
	 */
	PubPkt * createRainPkt();

	/**
	 * Creates a packet for the length test case
	 */
	PubPkt * createLengthPkt();

	/**
	 * Creates a packet for the selection test case
	 */
	PubPkt * createSelectionPkt();

private:
	ParamHandler *paramHandler;		// Handler of parameters

};

#endif
