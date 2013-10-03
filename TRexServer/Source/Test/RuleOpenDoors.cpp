#include "RuleOpenDoors.hpp"

using concept::test::RuleOpenDoors;
using namespace std;

RulePkt* RuleOpenDoors::buildRule(){
	RulePkt* rule = new RulePkt(false);

	int indexRootPos = 0;

	int compEvtType = 310;
	int vehicleDataType = 17;

	// VehicleData predicate
	Constraint vehicleDataConstr[2];
	strcpy(vehicleDataConstr[0].name, "speed");
	vehicleDataConstr[0].type = FLOAT;
	vehicleDataConstr[0].op = GT;
	vehicleDataConstr[0].floatVal = 0.0;
	strcpy(vehicleDataConstr[1].name, "door_status");
	vehicleDataConstr[1].type = INT;
	vehicleDataConstr[1].op = EQ;
	vehicleDataConstr[1].intVal = 3;

	// Parameter name
	char greenBoxIdStr[] = "greenBox_id";
	char idStr[] = "ID";

	// Create predicates, parameters, and negations
	rule->addRootPredicate(vehicleDataType, vehicleDataConstr, 2);

	// Template
	CompositeEventTemplate* templ = new CompositeEventTemplate(compEvtType);
	OpTree* idTree = new OpTree(new RulePktValueReference(indexRootPos, greenBoxIdStr), STRING);
	templ->addAttribute(idStr, idTree);
	rule->setCompositeEventTemplate(templ);

	return rule;
}
