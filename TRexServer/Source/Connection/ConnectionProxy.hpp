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

#ifndef CONNECTIONPROXY_H_
#define CONNECTIONPROXY_H_

#include "../external.hpp"
#include "../util.hpp"

namespace concept{
namespace connection{

class ConnectionProxy{
	public:
		virtual ~ConnectionProxy();

		virtual void asyncWrite(concept::util::CharVectorPtr bytes) = 0;

		virtual int getLocalPort() = 0;
		virtual int getRemotePort() = 0;
		virtual std::string getLocalAddress() = 0;
		virtual std::string getRemoteAddress() = 0;

		virtual std::string localToString() = 0;
		virtual std::string remoteToString() = 0;
		virtual std::string toString() = 0;
};

} // connection
} // concept

#endif /* CONNECTIONPROXY_H_ */
