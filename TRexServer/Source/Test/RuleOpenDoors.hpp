#ifndef RULEOPENDOORS_H_
#define RULEOPENDOORS_H_

#include "TestRule.hpp"

namespace concept{
namespace test{

class RuleOpenDoors: public concept::test::TestRule {
public:

	RulePkt* buildRule();

	SubPkt* buildSubscription() {
		throw std::logic_error("Not implemented yet");
	}

	std::vector<PubPkt*> buildPublication(){
		throw std::logic_error("Not implemented yet");
	}
};

} // test
} // concept

#endif
