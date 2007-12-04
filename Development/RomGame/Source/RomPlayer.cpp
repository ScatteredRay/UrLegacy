#include "RomPlayer.h" 
#include "UInput.h"

RomPlayer::RomPlayer()
{
}

RomPlayer::~RomPlayer()
{
	DeregisterInput();
}

void RomPlayer::RegisterInput(UInput* InputManager)
{
	RegisteredInputManager = InputManager;
	InputManager->RegisterInput(Name("PlayerPitch"), 1, &PlayerInput.AxisPitch, InputType_Float);
	InputManager->RegisterInput(Name("PlayerYaw"), 1, &PlayerInput.AxisYaw, InputType_Float);
	InputManager->RegisterInput(Name("PlayerRoll"), 1, &PlayerInput.AxisRoll, InputType_Float);

	InputManager->RegisterInput(Name("PlayerThrottle"), 1, &PlayerInput.AxisForward, InputType_Float);
	InputManager->RegisterInput(Name("PlayerSlide"), 1, &PlayerInput.AxisSlide, InputType_Float);
	InputManager->RegisterInput(Name("PlayerRise"), 1, &PlayerInput.AxisRise, InputType_Float);
}

void RomPlayer::DeregisterInput()
{
	if(RegisteredInputManager)
	{
		RegisteredInputManager->ClearInputSet(1);
		RegisteredInputManager = NULL;
	}
}
