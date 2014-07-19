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
#include "../../rdf3x-0.3.5/include/rts/operator/Result.hpp"
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"
#include "../../rdf3x-0.3.5/include/rts/operator/RDFQuery.hpp"
#include "../Common/ResultsCache.hpp"
#include <string>
#include <vector>
#include <algorithm>

#define RS_MAX_DIM 536870912

class QueryItem {

private:
	std::string db;									// The KB db reference for quering
	std::string query;								// The KB query
	std::string originalQuery;						// The original query, in case substitution occurs
	std::string nonParamQuery;						// The original query where each ExtParameter is transformed in SPARQL variable
	std::vector<ExtParameter> params;				// The KB query optional external parameters
	std::vector<std::string> fields;				// The list of fields that the query returns
	CompKind kind;									// The kind of constraint
	Resultset rs;									// The KB query retrieved results set
	int limit;										// The maximum number of results those can be stored in memory
	int offset;										// Offset for scanning query results
	std::map<std::string, bool> replacedParams;		// Tell if a parameter has been replaced or not
	std::map<std::string, std::string> paramsReplacement;	//Keeps track of which is the parameter value at run time
	bool isParametric;								// true if the query contains external parameters, false otherwise

	bool hasCachedResults(ResultsCache *qCache, std::string query);
	Resultset getCachedResults(ResultsCache *qCache, std::string query);
	void storeResults(ResultsCache *qCache, std::string query);

public:

	QueryItem(std::string & kb, std::string & q, std::vector<ExtParameter> p, CompKind k);

	~QueryItem();

	bool runQuery(ResultsCache *qCache);

	Resultset getResult();

	int getField(char * name);

	bool replaceExtParam(const std::string& pName, const std::string& pValue);

	bool needsReplace();

	bool hasMoreResults();

	std::vector<ExtParam>& getExtParams () { return params;}

	void resetExtParRepl();


};

#endif /* QUERYITEM_HPP_ */
