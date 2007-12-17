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
		Matrix4 MatRotate = inverse(Matrix4::rotation(Ship->Rotation));
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
		Vector3 ShipThrust = Vector3(	(PlayerInput.AxisForward * Ship->ShipThrust.getX()),
										(PlayerInput.AxisSlide * Ship->ShipThrust.getY()),
										(PlayerInput.AxisRise * Ship->ShipThrust.getZ()));


		Ship->Acceleration = rotate(Ship->Rotation, ShipThrust);

		Ship->DeltaRot = Quat::rotationY(PlayerInput.AxisPitch) * Quat::rotationZ(PlayerInput.AxisYaw) * Quat::rotationX(PlayerInput.AxisRoll);
		PlayerInput.AxisPitch = 0.0f;
		PlayerInput.AxisYaw = 0.0f;
	}

}
