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

void RomPlayer::GenerateViewMatrix(Matrix4* Mat)
{
	if(Ship)
	{	
		
		Matrix4 MatRotate = Matrix4::rotation(Ship->Rotation);
		Matrix4 MatTranslate = Matrix4::translation(Ship->Location);
		*Mat = MatRotate * MatTranslate;
		return;
	}
	*Mat = Matrix4::identity();
}

void RomPlayer::Tick(float DeltaTime)
{
	if(Ship)
	{
		Ship->Acceleration.setX(PlayerInput.AxisForward * Ship->ShipThrust.getX());
		Ship->Acceleration.setY(PlayerInput.AxisSlide * Ship->ShipThrust.getY());
		Ship->Acceleration.setZ(PlayerInput.AxisRise * Ship->ShipThrust.getZ());

		Ship->DeltaRot = Quat::rotationX(PlayerInput.AxisRoll) * Quat::rotationY(PlayerInput.AxisPitch) * Quat::rotationZ(PlayerInput.AxisYaw);
		PlayerInput.AxisPitch = 0.0f;
		PlayerInput.AxisYaw = 0.0f;
	}

}
