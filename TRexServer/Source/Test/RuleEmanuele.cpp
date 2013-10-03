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

#include "RuleEmanuele.hpp"

using concept::test::RuleEmanuele;

RulePkt* RuleEmanuele::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos= 0;

	// root Position predicate
	Constraint constr[1];
	strcpy(constr[0].name, "speed");
	constr[0].type = FLOAT;
	constr[0].op = GT;
	constr[0].intVal = 0.0;
	rule->addRootPredicate(17, constr, 1);

	// template
	CompositeEventTemplate* templ= new CompositeEventTemplate(17);

	OpTree* speedTree = new OpTree(new RulePktValueReference(indexRootPos, "speed"), FLOAT);
	templ->addAttribute("speed", speedTree);

	OpTree* idTree = new OpTree(new RulePktValueReference(indexRootPos, "greenboxid"), STRING);
	templ->addAttribute("greenboxid", idTree);

	OpTree* socTree = new OpTree(new RulePktValueReference(indexRootPos, "soc"), INT);
	templ->addAttribute("soc", socTree);

	OpTree* dsTree = new OpTree(new RulePktValueReference(indexRootPos, "drivestyle"), STRING);
	templ->addAttribute("drivestype", dsTree);

	OpTree* voltageTree = new OpTree(new RulePktValueReference(indexRootPos, "voltage"), FLOAT);
	templ->addAttribute("voltage", voltageTree);

	OpTree* currentTree = new OpTree(new RulePktValueReference(indexRootPos, "current"), FLOAT);
	templ->addAttribute("current", currentTree);

	OpTree* directionTree = new OpTree(new RulePktValueReference(indexRootPos, "direction"), BOOL);
	templ->addAttribute("direction", directionTree);

	OpTree* doorstatusTree = new OpTree(new RulePktValueReference(indexRootPos, "doorstatus"), INT);
	templ->addAttribute("doorstatus", doorstatusTree);

	OpTree* batterystatusTree = new OpTree(new RulePktValueReference(indexRootPos, "batterystatus"), INT);
	templ->addAttribute("batterystatus", batterystatusTree);

	OpTree* heatingstatusTree = new OpTree(new RulePktValueReference(indexRootPos, "heatingstatus"), BOOL);
	templ->addAttribute("heatingstatus", heatingstatusTree);

	OpTree* latitudeTree = new OpTree(new RulePktValueReference(indexRootPos, "latitude"), FLOAT);
	templ->addAttribute("latitude", latitudeTree);

	OpTree* longitudeTree = new OpTree(new RulePktValueReference(indexRootPos, "longitude"), FLOAT);
	templ->addAttribute("longitude", longitudeTree);

	OpTree* heightTree = new OpTree(new RulePktValueReference(indexRootPos, "height"), FLOAT);
	templ->addAttribute("height", heightTree);

	OpTree* gpsTree = new OpTree(new RulePktValueReference(indexRootPos, "gpsspeed"), FLOAT);
	templ->addAttribute("gpsspeed", gpsTree);

	rule->setCompositeEventTemplate(templ);
	return rule;
}
