/*
 * Field.h
 *
 *  Created on: 14/nov/2013
 *      Author: lele
 */

#ifndef FIELD_H_
#define FIELD_H_

#define STRING_LEN 32

#include <string.h>

typedef union fielddata {
	int i;
	float f;
	bool b;
	char s[STRING_LEN];
} fielddata_t;

enum ValType {
	INT=0,
	FLOAT=1,
	BOOL=2,
	STRING=3
};

class Field {
private:
	fielddata_t value;
	ValType type;
public:
	Field();
	virtual ~Field();

	//getters
	int getIValue();
	float getFValue();
	bool getBValue();
	char* getSValue();
	ValType getType();

	//setters
	void setIValue(int v);
	void setFValue(float v);
	void setBValue(bool v);
	void setSValue(char* v);

};

#endif /* FIELD_H_ */
