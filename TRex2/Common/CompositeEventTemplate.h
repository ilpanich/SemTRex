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

#ifndef COMPOSITEEVENTTEMPLATE_H_
#define COMPOSITEEVENTTEMPLATE_H_

#include "../Common/Consts.h"
#include "../Common/OpTree.h"
#include <map>

/**
 * Defines an attribute in the composite event template data structure
 */
class CompositeEventTemplateAttr {
public:
	CompositeEventTemplateAttr(char *parName, OpTree *parValue) {
		name = new char[strlen(parName)+1];
		strcpy(name, parName);
		value = parValue;
	}

	virtual ~CompositeEventTemplateAttr() {
		delete name;
		delete value;
	}

	void getName(char *parName) {
		strcpy(parName, name);
	}

	char *getName() {
		return name;
	}

	OpTree * getValue() {
		return value;
	}

	CompositeEventTemplateAttr * dup() {
		return new CompositeEventTemplateAttr(name, value->dup());
	}

private:
	char *name;
	OpTree *value;
};

/**
 * This class represents a template for a composite event.
 * It contains all information that are needed to create a composite event
 * starting from a set of sequences satisfying all the constraints in a rule.
 */
class CompositeEventTemplate {

public:

	/**
	 * Constructor: takes the event type as its input
	 */
	CompositeEventTemplate(int eventType);

	/**
	 * Destructor
	 */
	virtual ~CompositeEventTemplate();

	/**
	 * Adds a new attribute to the composite event
	 */
	void addAttribute(char *name, OpTree *value);

	/**
	 * Returns the type of the composite event
	 */
	int getEventType();

	/**
	 * Returns the number of attributes defined in the template
	 */
	int getAttributesNum();

	/**
	 * Copies the name of the attribute with the given index
	 */
	void getAttributeName(char *name, int index);

	/**
	 * Returns the name of the attribute with the given index
	 */
	char *getAttributeName(int index);

	/**
	 * Returns the operation tree for the attribute with the given index
	 */
	OpTree * getAttributeTree(int index);

	/**
	 * Creates an exact copy (deep copy) of the data structure
	 */
	CompositeEventTemplate * dup();

private:
	int eventType;																					// Type of the composite event
	std::map<int, CompositeEventTemplateAttr *> attributes;	// Attributes of the composite event

	/**
	 * Adds a new attribute to the composite event
	 */
	void addAttribute(CompositeEventTemplateAttr *parAttr);
};

#endif /* COMPOSITEEVENTTEMPLATE_H_ */
