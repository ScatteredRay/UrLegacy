#include "UInput.h"

UInput::UInput()
{
	InputMapping.Add(MaxNames);
	for(int i=0; i<MaxNames; i++)
	{
		InputMapping[i] = NULL;
	}
}

void UInput::RegisterInput(Name InputName, uint InputSet, void* Input, InputType Type)
{
	assert(InputMapping[InputName.GetIndex()] == NULL);
	InputMap* NewInput = new InputMap(InputName, InputSet, Input, Type);
	InputMapping[InputName.GetIndex()] = NewInput;
}

void UInput::ClearInputSet(uint InputSet)
{
	for(int i=0; i<MaxNames; i++)
	{
		if(InputMapping[i] && InputMapping[i]->InputSet == InputSet)
		{
			delete InputMapping[i];
			InputMapping[i] = NULL;
		}
	}
}

void UInput::SetAxisInput(Name Input, float Value)
{
	if(InputMapping[Input.GetIndex()])
	{
		assert(InputMapping[Input.GetIndex()]->Type == InputType_Float);
		assert(InputMapping[Input.GetIndex()]->InputLocation);
		*((float*)InputMapping[Input.GetIndex()]->InputLocation) = Value;
	}
}

void UInput::SetButtonInput(Name Input, bool Value)
{
	if(InputMapping[Input.GetIndex()])
	{
		assert(InputMapping[Input.GetIndex()]->Type == InputType_Bool);
		assert(InputMapping[Input.GetIndex()]->InputLocation);
		*((bool*)InputMapping[Input.GetIndex()]->InputLocation) = Value;
	}
}
