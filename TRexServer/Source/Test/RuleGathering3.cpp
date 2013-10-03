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

#include "RuleGathering3.hpp"

using concept::test::RuleGathering3;

char RuleGathering3::ATTR_CENTERLAT[]= "CenterLat";
char RuleGathering3::ATTR_CENTERLONG[]= "CenterLong";
char RuleGathering3::ATTR_DELTALAT[]= "DeltaLat";
char RuleGathering3::ATTR_DELTALONG[]= "DeltaLong";

RulePkt* RuleGathering3::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos= 0;

	// root Position predicate
	Constraint constr[1];
	strcpy(constr[0].name, RuleGathering3::ATTR_CENTERLAT);
	constr[0].type= FLOAT;
	constr[0].op= LT;
	constr[0].floatVal= 90.1;
	rule->addRootPredicate(EVENT_GATHERING, constr, 1);

	// event template
	CompositeEventTemplate* templ= new CompositeEventTemplate(EVENT_GATHERING);

	// CenterLat attribute
	OpTree* cenLatTree= new OpTree(new RulePktValueReference(indexRootPos, RuleGathering3::ATTR_CENTERLAT), FLOAT);
	templ->addAttribute(RuleGathering3::ATTR_CENTERLAT, cenLatTree);

	// CenterLong attribute
	OpTree* cenLongTree= new OpTree(new RulePktValueReference(indexRootPos, RuleGathering3::ATTR_CENTERLONG), FLOAT);
	templ->addAttribute(RuleGathering3::ATTR_CENTERLONG, cenLongTree);

	rule->setCompositeEventTemplate(templ);

	return rule;
}
