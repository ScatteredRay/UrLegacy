#ifndef _UGAMEOBJECT_H_
#define _UGAMEOBJECT_H_

#include "UMath.h"

class UGameObject
{

};

class UMovableObject : public UGameObject
{
public:
	KVector Location;
	KQuat Rotation;
};

#endif //_UGAMEOBJECT_H_
