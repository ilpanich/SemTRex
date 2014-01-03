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
 }

 QueryItem::~QueryItem() {
	 delete db;
	 delete query;
	 delete dbId;
	 delete qId;
	 delete params;
 }

 bool QueryItem::runQuery() {
	 if(params == NULL)
		 rs = RDFQuery::execQuery(kb*, query*, false);
	 else				// TODO: here external parameters of the query must be handled
		 rs = RDFQuery::execQuery(kb*, query*, false);
	 if(rs == NULL)
		 return false;
	 else
		 return true;
 }

 Resultset QueryItem::getResult() {
	 return rs;
 }
