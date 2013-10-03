#ifndef RULETHEFT_H_
#define RULETHEFT_H_

#include "TestRule.hpp"

namespace concept{
namespace test{

class RuleTheft: public concept::test::TestRule {
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
