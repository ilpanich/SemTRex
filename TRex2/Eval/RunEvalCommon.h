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

#ifndef RUNEVALCOMMON_H_
#define RUNEVALCOMMON_H_

#include <string>

namespace trex_testing {

/**
 * Creates the name file for storing results of evaluation.
 * The file name is stored in result.
 */
void getFileName(std::string &result, std::string &name, int mode, int seed);

/**
 * Creates the name file for storing throughput results
 */
void getThroughputFile(std::string &result, std::string &name, int seed);

/**
 * Creates the name of the file for storing results about dropped packets
 */
void getDroppedFile(std::string &result, std::string &name, int seed);

/**
 * Creates the name of the file for storing the average processing time
 */
void getMeanTimeFile(std::string &result, std::string &name, int seed);

/**
 * Creates the name of the file for storing the minimum processing time
 */
void getMinTimeFile(std::string &result, std::string &name, int seed);

/**
 * Creates the name of the file for storing the maximum processing time
 */
void getMaxTimeFile(std::string &result, std::string &name, int seed);

/**
 * Creates the name of the file for storing the 99th percentile of the processing time
 */
void getPercTimeFile(std::string &result, std::string &name, int seed);

} // end namespace trex_testing

#endif
