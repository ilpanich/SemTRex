/*
 * ResultsCache.hpp
 *
 *  Created on: 03/giu/2014
 *      Author: lele
 */

#ifndef RESULTSCACHE_HPP_
#define RESULTSCACHE_HPP_

#include <vector>
#include <string>
#include <pthread.h>
#include "ResultEntry.hpp"

class ResultsCache {
public:
	ResultsCache();
	virtual ~ResultsCache();

	void addEntry(std::string dbId, std::string qId, Resultset rs);

	ResultEntry getEntry(std::string dbId, std::string qId);

	bool hasEntry(std::string dbId, std::string qId);

private:
	std::vector<ResultEntry> cache;

#if MP_MODE == MP_LOCK
	pthread_mutex_t *mutex;	// Mutex for synchronized methods
#endif

};

#endif /* RESULTSCACHE_HPP_ */
