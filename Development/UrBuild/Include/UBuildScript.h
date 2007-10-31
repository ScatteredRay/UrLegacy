#ifndef _UBUILDSCRIPT_H_
#define _UBUILDSCRIPT_H_
#include "Core.h"
#include "Name.h"

void RunBuildScripts();
void RunBuildScript(const char* Filename);

struct BuildVar
{
	Name VarName;
	enum 
	{
		Var_None,
		Var_String,
		Var_Int,
		Var_Float
	} ValueType;
	union
	{
		char* StringVal;
		int IntVal;
		float FloatVal;
	};
	BuildVar* Next;
	BuildVar() : VarName(), Next(NULL), ValueType(Var_None) {}
	~BuildVar()
	{
		//delete VarName;
		if(ValueType == Var_String)
			delete StringVal;
		delete Next;
	}
};

#endif //_UBUILDSCRIPT_H_
