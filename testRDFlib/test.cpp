/*
 * test.cpp
 *
 *  Created on: 16/gen/2014
 *      Author: lele
 */

#include <string>

#include "rts/operator/RDFQuery.hpp"
#include "rts/operator/Resultset.hpp"

using namespace std;

int main(int argc, char * argv[]) {

	string db = "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db";
	string query = "select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> <London> }";

	Resultset rs;

	cout << "Executing query... ";

	rs = RDFQuery::execQuery(db, query, false);

	cout << "DONE!\n";

	return 0;

}
