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

void replaceAll(std::string& str, const std::string& from, const std::string& to);

int main(int argc, char * argv[]) {

	string db = "/home/lele/git/SemTRex/rdf3x-0.3.5/bin/db";
	string query = "select ?name where { ?p <isCalled> ?name. ?p <bornInLocation> \"Dover\" }";
	string test = "name";

	string test1 = "a &pippo piace la &paprica. viva &pippo";
	string test2 = "&pippo";
	string test3 = "0";

	const char* p;
	const char* z;

	char * q = new char[query.length() + 1];
	strcpy(q, query.substr(query.find_first_of("?"), query.find("where") - query.find_first_of("?")).c_str());

	char * t = new char[test.length() + 1];
	strcpy(t, test.c_str());

	vector<string> fields;
	vector<string> params;

	Resultset rs;

	replaceAll(test1, test2, test3);

	cout << "Executing query... ";

	rs = RDFQuery::execQuery(db, query, false);

	for (p = strtok( q, " " );  p;  p = strtok( NULL, " " ))
	{
		fields.push_back(p);
	}
	for (z = strtok( t, "," );  z;  z = strtok( NULL, "," ))
	{
		params.push_back(z);
	}

	for(Resultset::iterator it=rs.first(); it!=rs.last(); it++) {
		Result res = *it;
		Field f1 = res.getResult()[0];
		//Field f2 = res.getResult()[1];

		cout << f1.getSValue() << endl;
	}

	cout << "DONE!\n";

	return 0;

}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
