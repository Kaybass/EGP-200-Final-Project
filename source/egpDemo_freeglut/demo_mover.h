/*
EGP Graphics Starter
Movable object structure and function prototypes by Dan Buckstein
*/

#ifndef __DEMO_MOVER_H
#define __DEMO_MOVER_H


#include "cbmath/cbtkMatrix.h"


namespace demo
{

	// basic movable object structure
	struct Mover
	{
		// functions of position over time
		cbmath::vec4 position;		// what we ultimately use to display the object
		cbmath::vec4 velocity;		// first-order derivative
		cbmath::vec4 acceleration;	// second-order derivative
		

		// TO-DO: ADD MORE STUFF
		float mass;

		cbmath::mat4 * transformPtr;
	};


	// functions

	// create a mover with settings
	Mover createMover(
		cbmath::vec3 initialPosition,
		cbmath::vec3 initialVelocity,
		cbmath::vec3 initialAcceleration,
		// other
		float initialMass = 1.0f,
		cbmath::mat4 * transformPtr = 0
		
	);


	// setters
	void setPosition(Mover *mover, cbmath::vec3 newPosition);
	void addPosition(Mover *mover, cbmath::vec3 deltaPosition);
	void setVelocity(Mover *mover, cbmath::vec3 newVelocity);
	void addVelocity(Mover *mover, cbmath::vec3 deltaVelocity);
	void setAcceleration(Mover *mover, cbmath::vec3 newAcceleration);
	void addAcceleration(Mover *mover, cbmath::vec3 deltaAcceleration);

	void setForce(Mover *mover, cbmath::vec3 newForce);
	void addForce(Mover *mover, cbmath::vec3 deltaForce);
	void setMass(Mover *mover, float newMass);
	void addMass(Mover *mover, float deltaMass);


	// updates
	void updateMover1stOrder(Mover *mover, float dt);
	void updateMover2ndOrder(Mover *mover, float dt);
	void updateMoverEuler(Mover *mover, float dt);


}	// namespace demo

#endif	// __DEMO_MOVER_H