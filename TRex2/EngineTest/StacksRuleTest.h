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

#ifndef STACKSRULETEST_H
#define STACKSRULETEST_H

#include "../Common/Consts.h"
#include "../Engine/StacksRule.h"
#include "../Packets/RulePkt.h"
#include "../Engine/IndexingTable.h"

namespace trex_testing {

/**
 * Starts all testS for the StacksRule class
 */
bool stacksRuleTest();

/**
 * Builds a constraint "V EQ val"
 */
void buildConstraint(Constraint *constraints, int val);

/**
 * Testing methods
 */
bool singleSequence();
bool singleSequenceEach();
bool singleSequenceFirst();
bool singleSequenceLast();
bool singleSequenceFirstEach();
bool singleSequenceLastEach();
bool testEventGeneration();
bool testBetweenAggregates();
bool testTimingAggregates();
bool testAggregatesParameters();
bool testJoins1();
bool testJoins2();
bool testJoins3();
bool testJoins4();
bool testExternalParams();
bool testConsumptionEach();
bool testConsumptionFirst();
bool testConsumptionLast();
bool testConsumptionJoin();
bool testConsumptionJoin2();
bool testConsumptionJoin3();

}

#endif
