#include "RomShip.h" 

RomShip::RomShip()
{
	InitDefaults();
}

void RomShip::Tick(float DeltaTime)
{
	Velocity += Acceleration * DeltaTime;
	Location += Velocity * DeltaTime;
}

void RomShip::InitDefaults()
{
	ShipThrust = KVector(1.0f, 1.0f, 1.0f);
}