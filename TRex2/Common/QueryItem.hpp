/*
 * QueryItem.hpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#ifndef QUERYITEM_HPP_
#define QUERYITEM_HPP_

#include <string>
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"
#include "../../rdf3x-0.3.5/include/rts/operator/RDFQuery.hpp"

class QueryItem {

private:
	std::string db;				// The KB db reference for quering
	std::string query;			// The KB query
	std::string dbId;			// The KB db identifier for cache access
	std::string qId;			// The KB query identifier for cache access
	Resultset rs;				// The KB query retrieved results set

public:

	QueryItem(std::string kb, std::string q, std::string kbId, std::string queryId);

	~QueryItem();

	bool runQuery();

	Resultset getResult();


};

#endif /* QUERYITEM_HPP_ */
