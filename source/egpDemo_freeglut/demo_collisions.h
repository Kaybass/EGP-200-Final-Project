/*
EGP Graphics Starter
Collision bounding box structures and detection function prototypes by Dan Buckstein
*/

#ifndef __DEMO_COLLISIONS_H
#define __DEMO_COLLISIONS_H


#include "demo_mover.h"


namespace demo
{

	enum BoundingVolumeType
	{
		BVT_NONE, 
		BVT_SPHERE_CIRCLE, 
		BVT_BOX_AXIS_ALIGNED, 
		BVT_BOX_OBJECT, 
	};


	// SCBV: sphere/circle bounding volume
	// just needs size of sphere or circle (same in 3D and 2D)
	struct SphereCircleBoundingVolume
	{
		// center of sphere or circle
		cbmath::vec4 center;

		// size of sphere or circle: radius
		float radius;
	};
	// alias
	typedef SphereCircleBoundingVolume SBV;

	// creation
	SBV createSBV(
		cbmath::vec3 center = cbmath::v3zero, 
		float radius = 1.0f
	);

	// update SBV
	void setCenterSBV(SBV *bv, cbmath::vec3 newPosition);


	// BBV: box bounding volume
	// a.k.a. AABV, axis-aligned bounding volume or 
	//	OBV, object bounding volume
	struct BoxBoundingVolume
	{
		// center
		cbmath::vec4 center;

		// min values in local and common space, and max
		cbmath::vec4 localMinValues, localMaxValues;
		cbmath::vec4 worldMinValues, worldMaxValues;

		// additional info to help optimize
		// is this box axis-aligned (faster, simplified calculations)
		bool isAxisAligned;

		// do calculations in 3D, otherwise 2D (faster)
		bool is3D;


		// TO-DO: EXTEND TO SUPPORT OBB COLLISIONS

	};
	// alias
	typedef BoxBoundingVolume BBV;

	// creation
	BBV createBBV(
		cbmath::vec3 center = cbmath::v3zero,
		cbmath::vec3 localMinValues = cbmath::vec3(-1.0f),
		cbmath::vec3 localMaxValues = cbmath::vec3(+1.0f),
		bool isAxisAligned = true, 
		bool is3D = true
		// other
	);

	// update world limits for bounding box/volume
	void setCenterBBV(BBV *bv, cbmath::vec3 newPosition);
	void updateWorldLimits(BBV *bv);


	// detection algorithms and structure to hold result
	// no collision occurred if returned struct has no BB type for A
	struct Collision
	{
		BoundingVolumeType bvtA, bvtB;


		// TO-DO: ADD SUPPORT FOR RESPONSE

	};

	// circle/sphere vs circle/sphere
	Collision testCollisionSBVvsSBV(const SBV *a, const SBV *b);

	// box vs box: select the correct algorithm
	Collision testCollisionBBVvsBBV(const BBV *a, const BBV *b);

	// circle/sphere vx box
	Collision testCollisionSBVvsBBV(const SBV *a, const BBV *b);
	inline Collision testCollisionBBVvsSBV(const BBV *a, const SBV *b) { return testCollisionSBVvsBBV(b, a); }


}	// demo

#endif	// __DEMO_COLLISIONS_H