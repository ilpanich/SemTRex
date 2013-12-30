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

	typedef std::vector<Result>::iterator iterator;

	iterator first() {return results.begin();}

	iterator last() {return results.end();}
};

#endif /* RESULTSET_HPP_ */
