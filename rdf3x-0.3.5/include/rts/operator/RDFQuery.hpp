/*
 * Resultset.hpp
 *
 *  Created on: 19/nov/2013
 *      Author: lele
 */

#ifndef RDFQUERY_HPP_
#define RDFQUERY_HPP_

#include <string>

#include "rts/operator/Resultset.hpp"
#include "rts/database/Database.hpp"

class RDFQuery {

private:

public:
	RDFQuery();
	virtual ~RDFQuery();

	static Resultset execQuery(std::string& kb,const std::string& query,bool explain);
};

#endif /* RESULTSET_HPP_ */
