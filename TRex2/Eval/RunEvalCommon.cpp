//
// This file is part of T-Rex, a Complex Event Processing Middleware.
// See http://home.dei.polimi.it/margara
//
// Authors: Alessandro Margara
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <string>
#include <sstream>
#include "RunEvalCommon.h"

using namespace std;

void trex_testing::getFileName(string &result, string &name, int mode, int seed) {
	result+="Results/";
	result+=name;
	string type;
	if (mode==1) type = "_throughput";
	else if (mode==2) type = "_dropped";
	else if (mode==3) type = "_time";
	else if (mode==4) type = "_minTime";
	else if (mode==5) type = "_maxTime";
	else type = "_percTime";
	result+=type;
	stringstream stream;
	stream << seed;
	string stringSeed = stream.str();
	result+=stringSeed;
}

void trex_testing::getThroughputFile(string &result, string &name, int seed) {
	getFileName(result, name, 1, seed);
}

void trex_testing::getDroppedFile(string &result, string &name, int seed) {
	getFileName(result, name, 2, seed);
}

void trex_testing::getMeanTimeFile(string &result, string &name, int seed) {
	getFileName(result, name, 3, seed);
}

void trex_testing::getMinTimeFile(string &result, string &name, int seed) {
	getFileName(result, name, 4, seed);
}

void trex_testing::getMaxTimeFile(string &result, string &name, int seed) {
	getFileName(result, name, 5, seed);
}

void trex_testing::getPercTimeFile(string &result, string &name, int seed) {
	getFileName(result, name, 6, seed);
}

