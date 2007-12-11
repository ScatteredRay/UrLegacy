#include "RomPlayer.h" 
#include "RomShip.h"
#include "UInput.h"

RomPlayer::RomPlayer()
{
	Ship = new RomShip();
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

void RomPlayer::GenerateViewMatrix(KMatrix* Mat)
{
	if(Ship)
	{	
		MatrixTranslate(Mat, Ship->Location);
		return;
	}
	MatrixIdentity(Mat);
}

void RomPlayer::Tick(float DeltaTime)
{
	if(Ship)
	{
		Ship->Acceleration.x = PlayerInput.AxisForward * Ship->ShipThrust.x;
		Ship->Acceleration.y = PlayerInput.AxisSlide * Ship->ShipThrust.y;
		Ship->Acceleration.x = PlayerInput.AxisRise * Ship->ShipThrust.z;
	}

}
