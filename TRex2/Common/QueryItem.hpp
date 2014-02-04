/*
 * QueryItem.hpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#ifndef QUERYITEM_HPP_
#define QUERYITEM_HPP_

#include "../Common/Consts.h"
#include "../../rdf3x-0.3.5/include/rts/operator/Field.hpp"
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
	std::vector<std::string> fields;			// The list of fields that the query returns
	Resultset rs;							// The KB query retrieved results set
	bool runnable;							// Define if a query is ready to be run
	std::map<std::string, bool> replacedParams;	// Tell if a parameter has been replaced or not

public:

	QueryItem(std::string & kb, std::string & q, unsigned char * kbId, unsigned char * queryId, std::vector<ExtParameter> p);

	~QueryItem();

	bool runQuery();

	Resultset getResult();

	int getField(char * name);

	bool replaceExtParam(const std::string& pName, const std::string& pValue);

	bool isRunnable() { return runnable; }

	bool needsReplace();

	std::vector<ExtParam>& getExtParams () { return params;}

	void resetExtParRepl();


};

#endif /* QUERYITEM_HPP_ */
