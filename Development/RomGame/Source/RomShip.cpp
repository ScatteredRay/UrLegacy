#include "RomShip.h" 

RomShip::RomShip()
{
	InitDefaults();
}

void RomShip::Tick(float DeltaTime)
{
	Velocity += Acceleration * DeltaTime;
	if(Acceleration.IsZero())
		Velocity = KVector();
	Location += Velocity * DeltaTime;
	Rotation *= DeltaRot;
	DeltaRot = KQuat();
}

void RomShip::InitDefaults()
{
	ShipThrust = KVector(1.0f, 1.0f, 1.0f);
}