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
}

QueryItem::~QueryItem() {

}

bool QueryItem::runQuery() {
	if(params.empty())
		rs = RDFQuery::execQuery(db, query, false);
	else				// TODO: here external parameters of the query must be handled
		rs = RDFQuery::execQuery(db, query, false);
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
