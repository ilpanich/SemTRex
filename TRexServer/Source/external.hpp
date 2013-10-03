/*
 * Copyright (C) 2011 Francesco Feltrinelli <first_name DOT last_name AT gmail DOT com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

// Boost
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/variant.hpp>

#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/filters.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/core.hpp>
#include <boost/log/formatters.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/empty_deleter.hpp>

// C++ Standard Library
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

// C Standard Library
#include <unistd.h>

// TRex
#include <Common/Consts.h>
#include <Engine/TRexEngine.h>
#include <Marshalling/Marshaller.h>
#include <Marshalling/Unmarshaller.h>
#include <Packets/PubPkt.h>
#include <Packets/RulePkt.h>
#include <Packets/RulePktValueReference.h>

#endif /* EXTERNAL_H_ */
