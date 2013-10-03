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

#include "RulePktValueReference.h"

RulePktValueReference::RulePktValueReference(int stateIndex, char *parAttrName) {
	index = stateIndex;
	attrName = new char[strlen(parAttrName)+1];
	strcpy(attrName, parAttrName);
	aggIndex = false;
}

RulePktValueReference::RulePktValueReference(int aggregateIndex) {
	index = aggregateIndex;
	attrName = NULL;
	aggIndex = true;
}

RulePktValueReference::~RulePktValueReference() {
	if (attrName != NULL) delete [] attrName;
}

OpValueReference * RulePktValueReference::dup() {
	if (! aggIndex) return new RulePktValueReference(index, attrName);
	else return new RulePktValueReference(index);
}

int RulePktValueReference::getIndex() {
	return index;
}

bool RulePktValueReference::refersToAgg() {
	return aggIndex;
}

char * RulePktValueReference::getName() {
	if (! aggIndex) return attrName;
	return NULL;
}
