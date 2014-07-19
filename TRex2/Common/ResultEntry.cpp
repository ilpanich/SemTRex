/*
 * ResultsCache.cpp
 *
 *  Created on: 03/giu/2014
 *      Author: lele
 */

#include "ResultsCache.hpp"

using namespace std;

ResultEntry::ResultEntry() {
	hitCounter = 0;
}

ResultEntry::ResultEntry(string dbId, string qId, Resultset rs) {
	db = dbId;
	query = qId;
	results = rs;
	hitCounter = 0;
}

ResultEntry::~ResultEntry() {
	// TODO Auto-generated destructor stub
}

string ResultEntry::getDb() {
	return db;
}
string ResultEntry::getQuery() {
	return query;
}
Resultset ResultEntry::getRes() {
	return results;
}

void ResultEntry::incrementHitCounter() {
	hitCounter++;
}

int ResultEntry::getHitCounter() {
	return hitCounter;
}
