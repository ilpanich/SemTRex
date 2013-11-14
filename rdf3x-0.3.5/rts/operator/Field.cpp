/*
 * Field.cpp
 *
 *  Created on: 14/nov/2013
 *      Author: lele
 */

#include "rts/operator/Field.h"

Field::Field() {
	// TODO Auto-generated constructor stub

}

Field::~Field() {
	// TODO Auto-generated destructor stub
}

//getters
int getIValue()
{
	return Field::value.i;
}

float getFValue()
{
	return Field::value.f;
}

bool getBValue()
{
	return Field::value.b;
}
char* getSValue()
{
	return Field::value.s;
}

ValType getType()
{
	return Field::type;
}

//setters
void setIValue(int v)
{
	Field::value.i = v;
	Field::type = INT;
}

void setFValue(float v)
{
	Field::value.f = v;
	Field::type = FLOAT;
}
void setBValue(bool v)
{
	Field::value.b = v;
	Field::type = BOOL;
}
void setSValue(char* v)
{
	Field::value.s = v;
	Field::type = STRING;
}

