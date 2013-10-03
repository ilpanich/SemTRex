#include "RuleNotGivenBack.hpp"

using concept::test::RuleNotGivenBack;
using namespace std;

RulePkt* RuleNotGivenBack::buildRule(){
	RulePkt* rule= new RulePkt(false);

	int indexRootPos = 0;

	int notGivenBackType = 320;
	int reservationExpiredType = 214;
	int releaseType = 132;
	int takenType = 112;

	TimeMs win =  60 * 60 * 24 * 2; // seconds -> minutes -> days

	// Parameter name
	char greenBoxIdStr[] = "greenBox_id";
	char idStr[] = "ID";

	// Create predicates, parameters, and negations
	rule->addRootPredicate(reservationExpiredType, NULL, 0);
	rule->addPredicate(takenType, NULL, 0, indexRootPos, win, LAST_WITHIN);
	rule->addNegationBetweenStates(releaseType, NULL, 0, 0, 1);
	rule->addParameterBetweenStates(0, greenBoxIdStr, 1, greenBoxIdStr);
	rule->addParameterForNegation(0, greenBoxIdStr, 0, greenBoxIdStr);

	// Template
	CompositeEventTemplate* templ= new CompositeEventTemplate(notGivenBackType);
	OpTree* idTree0 = new OpTree(new RulePktValueReference(indexRootPos, greenBoxIdStr), STRING);
	templ->addAttribute(greenBoxIdStr, idTree0);
	OpTree* idTree1 = new OpTree(new RulePktValueReference(indexRootPos, "id"), INT);
	templ->addAttribute("reservation_id", idTree1);
	rule->setCompositeEventTemplate(templ);

	return rule;
}
