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

#include "Funs.h"

using namespace std;

int getFirstValidElement(vector<PubPkt *> &column, int columnSize, TimeMs minTimeStamp) {
	if (columnSize<=0) return -1;
	int minValue = 0;
	int maxValue = columnSize-1;
	if (column[maxValue]->getTimeStamp()<=minTimeStamp) return -1;
	while (maxValue-minValue>1) {
		int midPoint = minValue+(maxValue-minValue)/2;
		if (column[midPoint]->getTimeStamp()<=minTimeStamp) {
			minValue = midPoint;
		} else {
			maxValue = midPoint;
		}
	}
	if (maxValue-minValue==0) return minValue;
	if (column[minValue]->getTimeStamp()>minTimeStamp) return minValue;
	return maxValue;
}

int getLastValidElement(vector<PubPkt *> &column, int columnSize, TimeMs maxTimeStamp, int minIndex) {
	int minValue = minIndex;
	int maxValue = columnSize-1;
	if (minIndex==-1) return -1;
	if (column[minIndex]->getTimeStamp()>=maxTimeStamp) return -1;
	while (maxValue-minValue>1) {
		int midPoint = minValue+(maxValue-minValue)/2;
		if (column[midPoint]->getTimeStamp()>=maxTimeStamp) {
			maxValue = midPoint;
		} else {
			minValue = midPoint;
		}
	}
	if (maxValue-minValue==0) return minValue;
	if (column[maxValue]->getTimeStamp()<=maxTimeStamp) return maxValue;
	return minValue;
}

int deleteInvalidElements(vector<PubPkt *> &column, int columnSize, TimeMs minTimeStamp) {
	if (columnSize==0) return columnSize;
	if (column[columnSize-1]->getTimeStamp()==minTimeStamp) return 1;
	int firstValidElement = getFirstValidElement(column, columnSize, minTimeStamp);
	if (firstValidElement<0) return 0;
	else return columnSize-firstValidElement;
}
