/*
 * ResultsCache.cpp
 *
 *  Created on: 03/giu/2014
 *      Author: lele
 */

#include "ResultsCache.hpp"

using namespace std;

ResultsCache::ResultsCache() {
	// TODO Auto-generated constructor stub

}

ResultsCache::~ResultsCache() {
	// TODO Auto-generated destructor stub
}

void ResultsCache::addEntry(std::string dbId, std::string qId, Resultset rs) {
	ResultEntry * re = new ResultEntry(dbId, qId, rs);
	cache.push_back(*re);
}

ResultEntry ResultsCache::getEntry(std::string dbId, std::string qId) {
	for (vector<ResultEntry>::iterator it = cache.begin(); it != cache.end(); it++) {
		ResultEntry re = *it;
		if (re.getDb().compare(dbId) == 0 && re.getQuery().compare(qId) == 0)
			return re;
	}
}

bool ResultsCache::hasEntry(std::string dbId, std::string qId) {
	for (vector<ResultEntry>::iterator it = cache.begin(); it != cache.end(); it++) {
		ResultEntry re = *it;
		if (re.getDb().compare(dbId) == 0 && re.getQuery().compare(qId) == 0)
			return true;
	}
	return false;
}
