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

#include "RuleGathering.hpp"
#include "RuleStoppedPosition.hpp"

using concept::test::RuleGathering;
using concept::test::RuleStoppedPosition;

char RuleGathering::ATTR_CENTERLAT[]= "CenterLat";
char RuleGathering::ATTR_CENTERLONG[]= "CenterLong";
char RuleGathering::ATTR_DELTALAT[]= "DeltaLat";
char RuleGathering::ATTR_DELTALONG[]= "DeltaLong";

RulePkt* RuleGathering::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos= 0;

	// root StoppedPosition predicate
	// FIXME Fake constraint as a workaround to an engine's bug
	Constraint fakeConstr[1];
	strcpy(fakeConstr[0].name, RuleStoppedPosition::ATTR_LATITUDE);
	fakeConstr[0].type= FLOAT;
	fakeConstr[0].op = LT;
	fakeConstr[0].floatVal= 90.1; // latitude is always < 90
	rule->addRootPredicate(EVENT_STOPPEDPOSITION, fakeConstr, 1);

	// event template
	CompositeEventTemplate* templ= new CompositeEventTemplate(EVENT_GATHERING);

	// CenterLat attribute
	OpTree* cenLatTree= new OpTree(new RulePktValueReference(indexRootPos, RuleStoppedPosition::ATTR_LATITUDE), FLOAT);
	templ->addAttribute(ATTR_CENTERLAT, cenLatTree);

	// CenterLong attribute
	OpTree* cenLongTree= new OpTree(new RulePktValueReference(indexRootPos, RuleStoppedPosition::ATTR_LONGITUDE), FLOAT);
	templ->addAttribute(ATTR_CENTERLONG, cenLongTree);

	rule->setCompositeEventTemplate(templ);

	return rule;
}
