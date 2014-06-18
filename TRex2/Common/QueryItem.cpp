/*
 * QueryItem.cpp
 *
 *  Created on: 31/dic/2013
 *      Author: lele
 */

#include "QueryItem.hpp"

using namespace std;
using namespace boost;

QueryItem::QueryItem(string & kb, string & q, vector<ExtParameter> p, CompKind k) {

	db = string(kb);
	query = string(q);
	originalQuery = string(q);
	//resID.dbId = kbId;
	params = p;
	kind = k;

	isParametric = false;

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
		isParametric = true;
	}

	limit = RS_MAX_DIM / (fields.size() * sizeof(char[STRING_LEN]));
	offset = 0;

	if(isParametric) {
		// TODO: Convert parameters in variables and add them to the query select clause
	}
}

QueryItem::~QueryItem() {
	rs.clearRes();
}

bool QueryItem::runQuery(ResultsCache *qCache) {

	//cerr << query << endl;

//	cout << "Processing query: " << query << endl;

	// TODO: verify if the query is parametric and extract the results filtering the in-memory resultset

	if(!needsReplace()) {
				if(!hasCachedResults(qCache)) {
					rs = RDFQuery::execQuery(db, query, false);
					storeResults(qCache);
				}
				else {
		//			//			cerr << "CACHE HIT: DB -> " << resID.dbId << " QUERY -> " << resID.qId << endl;
//		rs = RDFQuery::execQuery(db, query, false);
					rs = getCachedResults(qCache);
		//			Resultset rs1 = RDFQuery::execQuery(db, query, false);
		//
		//			if (rs1.getAllRes().size() != rs.getAllRes().size()) {
		//				cerr << query << endl;
		//				cerr << ")CACHE SIZE (" << rs.getAllRes().size() << ") DIFFERENT FROM REAL QUERY ANSWER SET SIZE (" << rs1.getAllRes().size() << ")!" << endl;
		//			}
		//			else {
		//				for (int i = 0; i < rs1.getAllRes().size(); i++) {
		//					Result res1 = rs1.getAllRes().at(i);
		//					Result res2 = rs.getAllRes().at(i);
		//					if (res1.getResult().size() != res2.getResult().size())
		//						cerr << "CACHE CONTENT DIFFERENT FROM REAL QUERY ANSWER SET CONTENT!" << endl;
		//					else {
		//						for (int j = 0; j < res1.getResult().size(); j++) {
		//							Field f1 = res1.getResult()[j];
		//							Field f2 = res2.getResult()[j];
		//							if(strcmp(f1.getSValue(), f2.getSValue()) != 0)
		//								cerr << "CACHE FIELD CONTENT DIFFERENT FROM REAL QUERY ANSWER SET FIELD CONTENT!" << endl;
		//						}
		//
		//					}
		//
		//				}
		//			}
				}
	}
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
		paramsReplacement.insert(make_pair(pName, pValue));
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
		paramsReplacement.clear();
	}
}

bool QueryItem::hasMoreResults() {
	offset += limit;
	string nextQuery = originalQuery + " LIMIT " + lexical_cast<string>(limit) + "OFFSET " + lexical_cast<string>(offset);
	return RDFQuery::execQuery(db, query, false).getAllRes().empty();
}

bool QueryItem::hasCachedResults(ResultsCache *qCache) {
	return qCache->hasEntry(db, query);
}

Resultset QueryItem::getCachedResults(ResultsCache *qCache) {

	return qCache->getEntry(db, query).getRes();

	//	Cache::iterator it = qCache->find(resID);
	//
	////		for(Resultset::iterator iter=it->second.first(); iter!=it->second.last(); iter++) {
	////			Result res = *iter;
	////			for (int i = 0; i < res.getResult().size(); i++) {
	////				Field f = res.getResult()[i];
	////				cerr << f.getSValue() << "\t";
	////			}
	////			cerr << endl;
	////		}
	//	return it->second;
}


void QueryItem::storeResults(ResultsCache *qCache) {
	qCache->addEntry(db, query, rs);

	//	for (Cache::iterator it = qCache->begin(); it != qCache->end(); it ++)
	//		cerr << "Cache Entry: " << it->first << endl;
}
