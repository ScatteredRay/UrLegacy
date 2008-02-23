#include "Particles.h"

const float CollisionRange = 0.1f;
const float MaxCollisionThrust = 0.1f;

FluidSystem::FluidSystem(uint32 Count)
{
	InParticles = new Vector4[Count];
	OutParticles = new Vector4[Count];
	ParticleVelocities = new Vector4[Count];
	ParticleCount = Count;
	FirstInactive = 0;
}

FluidSystem::~FluidSystem()
{
	delete[] InParticles;
	delete[] OutParticles;
	delete[] ParticleVelocities;
}

void FluidSystem::Update(float DeltaTime)
{
	Vector4* TempParticles = InParticles;
	InParticles = OutParticles;
	OutParticles = TempParticles;

	uint32 Impulse = MaxCollisionThrust/CollisionRange;

	for(int i=0; i<ParticleCount; i++)
	{
		if(i>=FirstInactive)
			break;

		OutParticles[i] = InParticles[i];
		ParticleVelocities[i] *= 0.8f; // Dampen the velocity.

		for(int j=0; j<ParticleCount; j++)
		{
			if(i>=FirstInactive)
				break;
			if(i==j)
				continue;

			Vector4 Delta = InParticles[i] - InParticles[j];
			
			
			ParticleVelocities[i] += normalize(Delta) * (CollisionRange - lengthSqr(Delta)) * Impulse; // Normalize almost nullifies doing the lengthSqr at all.
			
		}

		// Constrain the velocity to create a more slow/deliberate/stable simulation.
	}
}