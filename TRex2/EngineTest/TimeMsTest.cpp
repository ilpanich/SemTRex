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

#include "TimeMsTest.h"

using namespace std;

bool trex_testing::timeMsTest() {
	cout << "*** TimeMs ***" << endl;
	TimeMs ms;
	TimeMs oneSec = 1000;
	if (! (ms>ms-oneSec)) {
		cout << "Error: current time is not grater than current time minus one sec." << endl;
		return false;
	}
	if (ms<ms-oneSec) {
		cout << "Error: current time is smaller than current time minus one sec." << endl;
		return false;
	}
	if (ms==ms-oneSec) {
		cout << "Error: current time is equal to current time minus one sec." << endl;
		return false;
	}
	if (! (ms!=ms-oneSec)) {
		cout << "Error: current time is not different from the current time minus one sec." << endl;
		return false;
	}
	if (! (ms-oneSec).elapsed()) {
		cout << "Error: current time minus one sec has not elapsed." << endl;
		return false;
	}
	if ((ms+oneSec).elapsed()) {
		cout << "Error: current time plus one sec has elapsed." << endl;
		return false;
	}
	oneSec=ms;
	if (oneSec!=ms) {
		cout << "Error: assignment failed" << endl;
		return false;
	}
	cout << "\nAll TimeMs tests ran successfully!\n" << endl;
	return true;
}
