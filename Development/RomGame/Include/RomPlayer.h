#ifndef _ROMPLAYER_H_
#define _ROMPLAYER_H_

#include "UGameObject.h"
#include "UMath.h"

class RomPlayer : public UGameObject
{
	struct
	{
		float AxisPitch;
		float AxisYaw;
		float AxisRoll;
		float AxisSlide;
		float AxisRise;
		float AxisForward;
	} PlayerInput;
	class UInput* RegisteredInputManager;

	class RomShip* Ship;
public:
	RomPlayer();
	~RomPlayer();
	void RegisterInput(UInput* InputManager);
	void DeregisterInput();
	void GenerateViewMatrix(Matrix4* Mat);
	void Tick(float DeltaTime);
};

#endif //_ROMPLAYER_H_
