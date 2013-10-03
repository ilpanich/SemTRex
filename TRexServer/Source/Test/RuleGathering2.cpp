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

#include "RuleGathering2.hpp"

using concept::test::RuleGathering2;

char RuleGathering2::ATTR_LATITUDE[]= "Latitude";
char RuleGathering2::ATTR_LONGITUDE[]= "Longitude";
char RuleGathering2::ATTR_CENTERLAT[]= "CenterLat";
char RuleGathering2::ATTR_CENTERLONG[]= "CenterLong";
char RuleGathering2::ATTR_DELTALAT[]= "DeltaLat";
char RuleGathering2::ATTR_DELTALONG[]= "DeltaLong";
char RuleGathering2::ATTR_SPEED[]= "Speed";

RulePkt* RuleGathering2::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos= 0;

	// root Position predicate
	Constraint constr[1];
	strcpy(constr[0].name, RuleGathering2::ATTR_LATITUDE);
	constr[0].type= FLOAT;
	constr[0].op= LT;
	constr[0].floatVal= 90.1;
	rule->addRootPredicate(EVENT_POSITION, constr, 1);
	TimeMs win = 30000;
	rule->addPredicate(EVENT_POSITION, constr, 1, 0, win, LAST_WITHIN);

	// event template
	CompositeEventTemplate* templ= new CompositeEventTemplate(EVENT_GATHERING);

	// CenterLat attribute
	OpTree* cenLatTree= new OpTree(new RulePktValueReference(indexRootPos, RuleGathering2::ATTR_LATITUDE), FLOAT);
	templ->addAttribute(RuleGathering2::ATTR_CENTERLAT, cenLatTree);

	// CenterLong attribute
	OpTree* cenLongTree= new OpTree(new RulePktValueReference(indexRootPos, RuleGathering2::ATTR_LONGITUDE), FLOAT);
	templ->addAttribute(RuleGathering2::ATTR_CENTERLONG, cenLongTree);

	rule->setCompositeEventTemplate(templ);

	return rule;
}
