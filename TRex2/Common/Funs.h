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

#ifndef FUNS_H_
#define FUNS_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../Engine/Stack.h"
#include "../Common/TimeMs.h"
#include "../Packets/PubPkt.h"

/**
 * Returns the id of the first element in the given column having a
 * value greater than minTimeStamp.
 * Returns -1 if such an element cannot be found.
 * The search is performed in logarithmic time, using a binary search.
 */
int getFirstValidElement(std::vector<PubPkt *> &column, int columnSize, TimeMs minTimeStamp);

/**
 * Returns the id of the last element in the given column having a
 * value smaller than maxTimeStamp and an index greater than minIndex.
 * Returns -1 if such an element cannot be found.
 * The search is performed in logarithmic time, using a binary search.
 */
int getLastValidElement(std::vector<PubPkt *> &column, int columnSize, TimeMs maxTimeStamp, int minIndex);

/**
 * Returns the new size of the column.
 */
int deleteInvalidElements(std::vector<PubPkt *> &column, int columnSize, TimeMs minTimeStamp);

#endif /* FUNS_H_ */
