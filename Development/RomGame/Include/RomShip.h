#ifndef _ROMSHIP_H_ 
#define _ROMSHIP_H_
#include "UGameObject.h"
#include "UMath.h"

class RomShip : public UMovableObject
{
public:
	Vector3 Acceleration;
	Vector3 Velocity;
	Quat DeltaRot;
	Vector3 ShipThrust;
	RomShip();
	void Tick(float DeltaTime);
	virtual void InitDefaults();
};

#endif //_ROMSHIP_H_
