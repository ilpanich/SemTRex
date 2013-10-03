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

#ifndef RULEGATHERING_H_
#define RULEGATHERING_H_

#include "TestRule.hpp"

namespace concept{
namespace test{

/**
 * FIXME this is a oversimplified version of the rule for testing purposes,
 * because at current state TRex cannot handle original version
 *
 * Rule PeopleGathering:
 *
 * define   PeopleGathering(CenterLat: float, CenterLong: float)
 * from     StoppedPosition() as P
 * where    CenterLat=P.Latitude, CenterLong=P.Longitude
 *
 */
class RuleGathering: public concept::test::TestRule {
public:

	// Attribute names
	static char ATTR_CENTERLAT[];
	static char ATTR_CENTERLONG[];
	static char ATTR_DELTALAT[];
	static char ATTR_DELTALONG[];

	RulePkt* buildRule();

	SubPkt* buildSubscription() {
		throw std::logic_error("Not implemented yet");
	}

	std::vector<PubPkt*> buildPublication(){
		throw std::logic_error("Not implemented yet");
	}
};

} // test
} // concept

#endif /* RULEGATHERING_H_ */
