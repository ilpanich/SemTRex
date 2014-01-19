/*
 * QueryItem.cpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#include "QueryItem.hpp"

using namespace std;

QueryItem::QueryItem(string kb, string q, unsigned char * kbId, unsigned char * queryId, vector<ExtParameter> p) {
	db = kb;
	query = q;
	dbId = kbId;
	qId = queryId;
	params = p;

	const char* t;
	char * qy = new char[query.length() + 1];
	strcpy(qy, query.substr(query.find_first_of("?") - 1, query.find("where") - 6).c_str());

	for (t = strtok( qy, " " );  t;  t = strtok( NULL, " " ))
	{
		fields.push_back(t);
	}
}

QueryItem::~QueryItem() {
	//delete db;
	//delete query;
	delete dbId;
	delete qId;
	//delete *params;
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
		if(s.compare(name))
			return i;
		else
			i++;
	}

	return -1;
}
