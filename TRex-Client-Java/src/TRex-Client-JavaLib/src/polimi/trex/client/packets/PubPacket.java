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

import polimi.trex.packets.PubPkt;

public class PubPacket extends PubPkt implements Packet {

	public PubPacket(int eventType) {
		super(eventType);
	}
	
	public PubPacket(PubPkt trexPubPkt) {
		this(trexPubPkt.getEventType());
		timeStamp= trexPubPkt.getTimeStamp();
		attributes= trexPubPkt.getAttributes();
	}
}
