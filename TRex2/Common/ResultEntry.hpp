/*
 * ResultsCache.hpp
 *
 *  Created on: 03/giu/2014
 *      Author: lele
 */

#ifndef RESULTENTRY_HPP_
#define RESULSENTRY_HPP_

#include <string>
#include "../../rdf3x-0.3.5/include/rts/operator/Resultset.hpp"

class ResultEntry {
public:
	ResultEntry(std::string dbId, std::string qId, Resultset rs);
	virtual ~ResultEntry();

	std::string getDb();
	std::string getQuery();
	Resultset getRes();

private:
	std::string db;
	std::string query;
	Resultset results;

};

#endif /* RESULTENTRY_HPP_ */