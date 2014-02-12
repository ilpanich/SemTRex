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

package polimi.trex.common;

/**
 * A reference to a value. It is used inside an OpTree to link a leaf node to an actual value.
 */
public class ValueReference {
	private int index;
	private boolean aggIndex;
	private String name;

	public ValueReference(int index, boolean aggIndex, String name) {
		this.index = index;
		this.aggIndex = aggIndex;
		this.name = name;
	}

	public int getIndex() {
		return index;
	}

	public boolean isAggIndex() {
		return aggIndex;
	}

	public String getName() {
		return name;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) return true;
		if (obj == null) return false;
		if (! (obj instanceof ValueReference)) return false;
		ValueReference other = (ValueReference) obj;
		if (aggIndex != other.aggIndex) return false;
		if (index != other.index) return false;
		if (! aggIndex && ! name.equals(other.name)) return false;
		return true;
	}
}