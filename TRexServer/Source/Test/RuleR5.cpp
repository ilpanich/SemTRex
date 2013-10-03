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

#include "RuleR5.hpp"
#include "RuleR0.hpp"

using namespace concept::test;
using namespace std;

RulePkt* RuleR5::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexPredSmoke= 0;
	int indexAggrTemp= 0;

	TimeMs fiveMin(1000*60*5);

	// Smoke root predicate
	rule->addRootPredicate(EVENT_SMOKE, NO_CONSTRAINTS, 0);

	// Temp avg aggregate
	rule->addTimeBasedAggregate(EVENT_TEMP, NO_CONSTRAINTS, 0, indexPredSmoke, fiveMin, RuleR0::ATTR_TEMPVALUE, AVG);

	// Parameter: Smoke.area=Temp.area
	rule->addParameterForAggregate(indexPredSmoke, RuleR0::ATTR_TEMPVALUE, indexAggrTemp, RuleR0::ATTR_TEMPVALUE);

	// Constraint: 45< Avg(Temp.value)
	// TODO: constraint not possible for now

	// Fire template
	CompositeEventTemplate* fireTemplate= new CompositeEventTemplate(EVENT_FIRE);

	// Area attribute in template
	OpTree* areaOpTree= new OpTree(new RulePktValueReference(indexPredSmoke, RuleR0::ATTR_TEMPVALUE), STRING);
	fireTemplate->addAttribute(RuleR0::ATTR_TEMPVALUE, areaOpTree);
	// MeasuredTemp attribute in template
	OpTree* measuredTempOpTree= new OpTree(new RulePktValueReference(indexAggrTemp), INT);
	fireTemplate->addAttribute(RuleR0::ATTR_MEASUREDTEMP, measuredTempOpTree);

	rule->setCompositeEventTemplate(fireTemplate);

	return rule;
}
