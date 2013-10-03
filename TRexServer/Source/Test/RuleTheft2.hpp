#ifndef RULETHEFT2_H_
#define RULETHEFT2_H_

#include "TestRule.hpp"

namespace concept{
namespace test{

class RuleTheft2: public concept::test::TestRule {
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
