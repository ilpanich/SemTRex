/*
 * Resultset.hpp
 *
 *  Created on: 19/nov/2013
 *      Author: lele
 */

#ifndef RESULTSET_HPP_
#define RESULTSET_HPP_

#include <vector>

#include "rts/operator/Result.hpp"

class Resultset {
private:
	std::vector<Result> results;
public:
	Resultset();
	virtual ~Resultset();

	void addResult(Result r);
	std::vector<Result> getAllRes();

	std::vector<Result>::iterator first();
};

#endif /* RESULTSET_HPP_ */
