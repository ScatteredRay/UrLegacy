#ifndef _ROMSHIP_H_ 
#define _ROMSHIP_H_
#include "UGameObject.h"
#include "UMath.h"

class RomShip : public UMovableObject
{
public:
	KVector Acceleration;
	KVector Velocity;
	KVector DeltaRot;
	KVector ShipThrust;
	RomShip();
	void Tick(float DeltaTime);
	virtual void InitDefaults();
};

#endif //_ROMSHIP_H_
