#ifndef _UINPUT_H_
#define _UINPUT_H_
#include "Name.h"
#include "UTypes.h"

enum InputType
{
	InputType_Float,
	InputType_Bool
};

struct InputMap
{
	Name InputName;
	uint InputSet;
	void* InputLocation;
	InputType Type;
	InputMap(Name InName, uint InSet, void* InLocation, InputType InType) : InputName(InName), InputSet(InSet), InputLocation(InLocation), Type(InType)
	{}
};

class UInput
{
	KArray<InputMap*> InputMapping;
public:
	UInput();
	void RegisterInput(Name InputName, uint InputSet, void* Input, InputType Type);
	void ClearInputSet(uint InputSet);
	void SetAxisInput(Name Input, float Value);
	void SetButtonInput(Name Input, bool Value);
};

#endif //_UINPUT_H_
