#include "RuleTheft2.hpp"

using concept::test::RuleTheft2;
using namespace std;

RulePkt* RuleTheft2::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos = 0;

	int theftType = 301;
	int vehicleDataType = 17;
	int takenType = 112;

	TimeMs win =  60 * 60 * 24 * 10; // seconds -> minutes -> days

	// VehicleData predicate
	Constraint vehicleDataConstr[1];
	strcpy(vehicleDataConstr[0].name, "speed");
	vehicleDataConstr[0].type = FLOAT;
	vehicleDataConstr[0].op = GT;
	vehicleDataConstr[0].floatVal = 0.0;

	// Parameter name
	char greenBoxIdStr[] = "greenBox_id";
	char idStr[] = "ID";

	// Create predicates, parameters, and negations
	rule->addRootPredicate(vehicleDataType, vehicleDataConstr, 1);
	rule->addTimeBasedNegation(takenType, NULL, 0, 0, win);
	rule->addParameterForNegation(0, greenBoxIdStr, 0, greenBoxIdStr);

	// Template
	CompositeEventTemplate* templ= new CompositeEventTemplate(theftType);
	OpTree* idTree = new OpTree(new RulePktValueReference(indexRootPos, greenBoxIdStr), STRING);
	templ->addAttribute(idStr, idTree);
	rule->setCompositeEventTemplate(templ);

	return rule;
}
