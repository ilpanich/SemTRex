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

#if MP_MODE == MP_LOCK
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);
#endif

}

ResultsCache::~ResultsCache() {
	// TODO Auto-generated destructor stub

#if MP_MODE == MP_LOCK
	pthread_mutex_destroy(mutex);
	delete mutex;
#endif
}

void ResultsCache::addEntry(std::string dbId, std::string qId, Resultset rs) {
#if MP_MODE == MP_LOCK
	pthread_mutex_lock(mutex);
#endif
	ResultEntry * re = new ResultEntry(dbId, qId, rs);
	cache.push_back(*re);
#if MP_MODE == MP_LOCK
	pthread_mutex_unlock(mutex);
#endif
}

ResultEntry ResultsCache::getEntry(std::string dbId, std::string qId) {
#if MP_MODE == MP_LOCK
	pthread_mutex_lock(mutex);
#endif
	ResultEntry re;
	for (vector<ResultEntry>::iterator it = cache.begin(); it != cache.end(); it++) {
		re = *it;
		if (re.getDb().compare(dbId) == 0 && re.getQuery().compare(qId) == 0)
			break;
	}
#if MP_MODE == MP_LOCK
	pthread_mutex_unlock(mutex);
#endif

	return re;
}

bool ResultsCache::hasEntry(std::string dbId, std::string qId) {
	for (vector<ResultEntry>::iterator it = cache.begin(); it != cache.end(); it++) {
		ResultEntry re = *it;
		if (re.getDb().compare(dbId) == 0 && re.getQuery().compare(qId) == 0)
			return true;
	}
	return false;
}
