#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#include "UTypes.h"
#include "UMath.h"

class FluidSystem
{
	uint32 ParticleCount;
	uint32 FirstInactive;
	Vector4* InParticles;
	Vector4* OutParticles;
	Vector4* ParticleVelocities;
public:
	FluidSystem(uint32 Count);
	~FluidSystem();
	void Update(float DeltaTime);
	Vector4 ParticleSolver(Vector4& Particle, Vector4& HitParticle);

};

#endif //_PARTICLES_H_
