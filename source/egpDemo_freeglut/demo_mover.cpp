/*
Dan Buckstein - Movable object
*/


#include "demo_mover.h"


// initializers
demo::Mover demo::createMover(
	cbmath::vec3 initialPosition,
	cbmath::vec3 initialVelocity,
	cbmath::vec3 initialAcceleration,
	float initialMass,
	cbmath::mat4 * transformPtr	
)
{
	Mover newMover;

	// set functions at time t
	// e.g. position: x(t) **set w to 1 for pos, 0 for derivs
	newMover.position.set(initialPosition, 1.0f);
	newMover.velocity.set(initialVelocity);
	newMover.acceleration.set(initialAcceleration);

	newMover.mass = initialMass;
	newMover.transformPtr = transformPtr;
	// TO-DO: SET MORE STUFF


	return newMover;
}


// setters
void demo::setPosition(Mover *mover, cbmath::vec3 newPosition)
{
	mover->position.set(newPosition, 1.0f);
}

void demo::addPosition(Mover *mover, cbmath::vec3 deltaPosition)
{
	mover->position.xyz += deltaPosition;
}

void demo::setVelocity(Mover *mover, cbmath::vec3 newVelocity)
{
	mover->velocity.set(newVelocity);
}

void demo::addVelocity(Mover *mover, cbmath::vec3 deltaVelocity)
{
	mover->velocity.xyz += deltaVelocity;
}

void demo::setAcceleration(Mover *mover, cbmath::vec3 newAcceleration)
{
	mover->acceleration.set(newAcceleration);


}

void demo::addAcceleration(Mover *mover, cbmath::vec3 deltaAcceleration)
{
	mover->acceleration.xyz += deltaAcceleration;


}


void demo::setForce(Mover *mover, cbmath::vec3 newForce)
{

}

void demo::addForce(Mover *mover, cbmath::vec3 deltaForce)
{

}

void demo::setMass(Mover *mover, float newMass)
{

}

void demo::addMass(Mover *mover, float deltaMass)
{

}



// updates
void demo::updateMover1stOrder(Mover *mover, float dt)
{
	// TO-DO: PRE-UPDATE TASKS


	// FIRST-ORDER NUMERICAL INTEGRATION: 
	mover->position += mover->velocity * dt;
	mover->velocity += mover->acceleration * dt;

	// TO-DO: POST-UPDATE TASKS
	if (mover->transformPtr)
		mover->transformPtr->c3 = mover->position;

}

//update the model matrix is something you have to do

void demo::updateMover2ndOrder(Mover *mover, float dt)
{
	// start with 1st order...

	// finalize position

}

void demo::updateMoverEuler(Mover *mover, float dt)
{
	// start with 1st order...
	updateMover1stOrder(mover, dt);

	// finalize position
	cbmath::vec4 addAccel = mover->acceleration*(dt * dt * 0.5);
	mover->position += addAccel;
	if (mover->transformPtr)
		mover->transformPtr->c3 += addAccel;

}
