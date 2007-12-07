#ifndef _ROMSHIP_H_ 
#define _ROMSHIP_H_
#include "UGameObject.h"
#include "UMath.h"

class RomShip : public UMovableObject
{
public:
	KVector Acceleration;
	KVector DeltaRot;
	KVector ShipThrust;
};

#endif //_ROMSHIP_H_
