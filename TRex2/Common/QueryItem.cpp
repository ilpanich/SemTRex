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
	nonParamQuery = string(q);
	originalQuery = string(q);
	//resID.dbId = kbId;
	params = p;
	kind = k;

	isParametric = false;

	if (!params.empty()) {
		int pos = 0;
		for(vector<ExtParameter>::iterator it=params.begin(); it!=params.end(); it++) {
			ExtParameter par = *it;
			string parStr = string(par.name2) + " where";
			replacedParams.insert(make_pair(string(par.name2), false));
			nonParamQuery.replace(nonParamQuery.find("where"), parStr.length() - 6, parStr);
			replace(nonParamQuery.begin(),nonParamQuery.end(), '&', '?');
			pos++;
		}
		isParametric = true;
	}

	if(!isParametric) {
		if(query.length() > 0) {
			const char* t;
			char * qy = new char[query.length() + 1];
			strcpy(qy, query.substr(query.find_first_of("?"), query.find("where") - query.find_first_of("?")).c_str());

			for (t = strtok( qy, " " );  t;  t = strtok( NULL, " " ))
			{
				fields.push_back(t);
			}
		}
	} else {
		if(nonParamQuery.length() > 0) {
			const char* t;
			char * qy = new char[nonParamQuery.length() + 1];
			strcpy(qy, nonParamQuery.substr(nonParamQuery.find_first_of("?"), nonParamQuery.find("where") - nonParamQuery.find_first_of("?")).c_str());

			for (t = strtok( qy, " " );  t;  t = strtok( NULL, " " ))
			{
				fields.push_back(t);
			}
		}
	}

	limit = RS_MAX_DIM / (fields.size() * sizeof(char[STRING_LEN]));
	offset = 0;

}

QueryItem::~QueryItem() {
	rs.clearRes();
}

bool QueryItem::runQuery(ResultsCache *qCache) {

	// TODO: verify if the query is parametric and extract the results filtering the in-memory resultset

	if(!needsReplace()) {
		if(!isParametric) {
			if(!hasCachedResults(qCache, query)) {
				rs = RDFQuery::execQuery(db, query, false);
				storeResults(qCache, query);
			}
			else {

				rs = getCachedResults(qCache, query);

			}
		} else {
			Resultset auxRS;
			if(!hasCachedResults(qCache, nonParamQuery)) {
				auxRS = RDFQuery::execQuery(db, nonParamQuery, false);
				storeResults(qCache, nonParamQuery, &auxRS);
			} else {
				auxRS = getCachedResults(qCache, nonParamQuery);
			}

			for(Resultset::iterator it = auxRS.first(); it != auxRS.last(); it++) {
				bool ok = true;
				bool okArray[paramsReplacement.size()];
				okArray[0] = false;
				int curr = 0;
				Result res = *it;
				for(map<string, string>::iterator iter = paramsReplacement.begin(); iter != paramsReplacement.end(); iter++) {
					string pN = iter->first;
					char * paramN;
					strcpy(paramN, pN.c_str());
					string pV = iter->second;
					Field f = res.getResult()[getField(paramN)];
					if(strcmp(pV.c_str(), f.getSValue()) != 0) {
						okArray[curr] = true;
						if(curr < paramsReplacement.size() - 1) {
							curr++;
							okArray[curr] = false;
						}
						res.removeElement(getField(paramN));
						break;
					} //else
//						res.removeElement(getField(paramN));
				}
				for(int i = 0; i < curr + 1; i++)
					ok = ok && okArray[i];
				if(ok)
					rs.addResult(res);
			}
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

bool QueryItem::hasCachedResults(ResultsCache *qCache, string query) {
	return qCache->hasEntry(db, query);
}

Resultset QueryItem::getCachedResults(ResultsCache *qCache, string query) {

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


void QueryItem::storeResults(ResultsCache *qCache, string query, Resultset *res) {
	if(res == NULL)
		qCache->addEntry(db, query, rs);
	else
		qCache->addEntry(db,query, *res);

	//	for (Cache::iterator it = qCache->begin(); it != qCache->end(); it ++)
	//		cerr << "Cache Entry: " << it->first << endl;
}
