/*
 * test.cpp
 *
 *  Created on: 16/gen/2014
 *      Author: lele
 */

#include <string>
#include <vector>

#include "rts/operator/RDFQuery.hpp"
#include "rts/operator/Resultset.hpp"

using namespace std;

int main(int argc, char * argv[]) {

	string db = "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db";
	string query = "select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> <London> }";

	const char* p;

	char * q = new char[query.length() + 1];
	strcpy(q, query.substr(query.find_first_of("?") - 1, query.find("where") - 6).c_str());

	vector<string> fields;

	Resultset rs;

	cout << "Executing query... ";

	rs = RDFQuery::execQuery(db, query, false);

	for (p = strtok( q, " " );  p;  p = strtok( NULL, " " ))
	  {
	    fields.push_back(p);
	  }

	for(Resultset::iterator it=rs.first(); it!=rs.last(); it++) {
		Result res = *it;
		Field f = res.getResult()[0];

		cout << f.getSValue() << "\n";
	}

	cout << "DONE!\n";

	return 0;

}
