/*
 * QueryItem.hpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#ifndef QUERYITEM_HPP_
#define QUERYITEM_HPP_

#include "../Common/Consts.h"
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"
#include "../../rdf3x-0.3.5/include/rts/operator/RDFQuery.hpp"
#include <string>
#include <vector>

class QueryItem {

private:
	std::string db;							// The KB db reference for quering
	std::string query;						// The KB query
	unsigned char * dbId;						// The KB db identifier for cache access
	unsigned char * qId;						// The KB query identifier for cache access
	std::vector<ExtParameter> params;			// The KB query optional external parameters
	Resultset rs;							// The KB query retrieved results set

public:

	QueryItem(std::string kb, std::string q, unsigned char * kbId, unsigned char * queryId, std::vector<ExtParameter> p);

	~QueryItem();

	bool runQuery();

	Resultset getResult();


};

#endif /* QUERYITEM_HPP_ */