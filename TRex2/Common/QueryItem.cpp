/*
 * QueryItem.cpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#include "QueryItem.hpp"

using namespace std;

 QueryItem::QueryItem(string kb, string q, string kbId, string queryId) {
	 db = kb;
	 query = q;
	 dbId = kbId;
	 qId = queryId;
 }

 QueryItem::~QueryItem() {
	 delete db;
	 delete query;
	 delete dbId;
	 delete qId;
 }

 bool QueryItem::runQuery() {
	 rs = RDFQuery::execQuery(kb*, query*, false);
	 if(rs == NULL)
		 return false;
	 else
		 return true;
 }

 Resultset QueryItem::getResult() {
	 return rs;
 }
