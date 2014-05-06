/*
 * QueryItem.cpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#include "QueryItem.hpp"

using namespace std;

QueryItem::QueryItem(string & kb, string & q, unsigned char * kbId, unsigned char * queryId, vector<ExtParameter> p) {

	db = string(kb);
	query = string(q);
	originalQuery = string(q);
	dbId = kbId;
	qId = queryId;
	params = p;

	if(query.length() > 0) {
		const char* t;
		char * qy = new char[query.length() + 1];
		strcpy(qy, query.substr(query.find_first_of("?"), query.find("where") - query.find_first_of("?")).c_str());

		for (t = strtok( qy, " " );  t;  t = strtok( NULL, " " ))
		{
			fields.push_back(t);
		}
	}

	if (!params.empty()) {
		for(vector<ExtParameter>::iterator it=params.begin(); it!=params.end(); it++) {
			ExtParameter par = *it;
			replacedParams.insert(make_pair(string(par.name2), false));
		}
	}
}

QueryItem::~QueryItem() {

}

bool QueryItem::runQuery() {

	//cerr << query << endl;

	if(!needsReplace())
		rs = RDFQuery::execQuery(db, query, false);
	else				// TODO: here external parameters of the query must be handled
		return false;
	if(rs.getAllRes().empty())
		return false;
	else
		return true;
}

Resultset QueryItem::getResult() {
	return rs;
}

int QueryItem::getField(char * name) {
	int i = 0;
	for(vector<string>::iterator it = fields.begin(); it != fields.end(); it++) {
		string s = *it;
		if(strcmp(name, s.c_str()) == 0)
			return i;
		else
			i++;
	}

	return -1;
}

bool QueryItem::replaceExtParam(const std::string& pName, const std::string& pValue) {

	map<string,bool>::iterator foundEl;

    if(pName.empty())
        return false;
    size_t start_pos = 0;
    while((start_pos = query.find(pName, start_pos)) != std::string::npos) {
        query.replace(start_pos, pName.length(), "\"" +  pValue + "\"");
        start_pos += pValue.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    foundEl = replacedParams.find(pName);
    if (foundEl == replacedParams.end())
    	return false;
    else {
    	foundEl->second = true;
    	return true;
    }
}

bool QueryItem::needsReplace() {

	if (params.empty())
		return false;
	else {
		for (map<string,bool>::iterator it=replacedParams.begin(); it!=replacedParams.end(); it++) {
			if (it->second == false)
				return true;
		}
		return false;
	}
}

void QueryItem::resetExtParRepl() {
	if (!params.empty()) {
		for (map<string,bool>::iterator it=replacedParams.begin(); it!=replacedParams.end(); it++)
			it->second = false;
		query = originalQuery;
	}
}
