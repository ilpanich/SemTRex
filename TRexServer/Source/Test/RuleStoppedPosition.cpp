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

#include "RuleStoppedPosition.hpp"
using namespace std;

using concept::test::RuleStoppedPosition;

char RuleStoppedPosition::ATTR_PHONE[]= "Id";
char RuleStoppedPosition::ATTR_LATITUDE[]= "Latitude";
char RuleStoppedPosition::ATTR_LONGITUDE[]= "Longitude";
char RuleStoppedPosition::ATTR_SPEED[]= "Speed";

RulePkt* RuleStoppedPosition::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos= 0;

	// root Position predicate
	Constraint constr[1];
	strcpy(constr[0].name, ATTR_SPEED);
	constr[0].type= FLOAT;
	constr[0].op= LT;
	constr[0].floatVal= 0.1;
	rule->addRootPredicate(EVENT_POSITION, constr, 1);

	// template
	CompositeEventTemplate* templ= new CompositeEventTemplate(EVENT_STOPPEDPOSITION);

	// Phone's id attribute
	OpTree* phoneTree= new OpTree(new RulePktValueReference(indexRootPos, ATTR_PHONE), STRING);
	templ->addAttribute(ATTR_PHONE, phoneTree);

	// Latitude attribute
	OpTree* latTree= new OpTree(new RulePktValueReference(indexRootPos, ATTR_LATITUDE), FLOAT);
	templ->addAttribute(ATTR_LATITUDE, latTree);

	// Longitude attribute
	OpTree* longTree= new OpTree(new RulePktValueReference(indexRootPos, ATTR_LONGITUDE), FLOAT);
	templ->addAttribute(ATTR_LONGITUDE, longTree);

	rule->setCompositeEventTemplate(templ);

	return rule;
}
