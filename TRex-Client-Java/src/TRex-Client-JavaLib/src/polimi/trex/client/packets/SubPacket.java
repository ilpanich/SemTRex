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

package polimi.trex.client.packets;

import polimi.trex.packets.SubPkt;

public class SubPacket extends SubPkt implements Packet {

	public SubPacket(int eventType) {
		super(eventType);
	}
	
	public SubPacket(SubPkt trexSubPkt){
		this(trexSubPkt.getEventType());
		constraints = trexSubPkt.getConstraints();
	}
}
