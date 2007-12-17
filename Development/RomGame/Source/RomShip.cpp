#include "RomShip.h" 

RomShip::RomShip()
{
	InitDefaults();
}

void RomShip::Tick(float DeltaTime)
{
	Velocity += Acceleration * DeltaTime;
	if(lengthSqr(Acceleration) < 0.01f)
		Velocity = Vector3(0.0f, 0.0f, 0.0f);
	Location += Velocity * DeltaTime;
	Rotation *= DeltaRot;
	DeltaRot = Quat::identity();
}

void RomShip::InitDefaults()
{
	ShipThrust = Vector3(1.0f, 1.0f, 1.0f);
	Rotation = Quat::identity();
	DeltaRot = Quat::identity();
}