/*
Collision Detection - D. Buckstein
*/


#include "demo_collisions.h"


#include <math.h>


//-----------------------------------------------------------------------------
// internal utilities
namespace demo
{

	// constant: unit AABV for easy OBV tests
	static const BBV unitBBV2D = demo::createBBV(cbmath::v3zero, cbmath::vec3(-1.0f, -1.0f, 0.0f), cbmath::vec3(1.0f, 1.0f, 0.0f), true, false);
	static const BBV unitBBV3D = demo::createBBV();

	// correction method for min/max
	static inline void correctLimitsLocal(BBV *bv)
	{
		float tmp;

		// if the minimums are greater than the maximums, must correct
		if ((bv->localMinValues.x - bv->localMaxValues.x) > 0.0f)
		{
			tmp = bv->localMaxValues.x;						// store tmp
			bv->localMaxValues.x = bv->localMinValues.x;	// write min to max
			bv->localMinValues.x = tmp;						// write tmp to min
		}
		if ((bv->localMinValues.y - bv->localMaxValues.y) > 0.0f)
		{
			tmp = bv->localMaxValues.y;
			bv->localMaxValues.y = bv->localMinValues.y;
			bv->localMinValues.y = tmp;
		}
		if (bv->is3D && ((bv->localMinValues.z - bv->localMaxValues.z) > 0.0f))
		{
			tmp = bv->localMaxValues.z;
			bv->localMaxValues.z = bv->localMinValues.z;
			bv->localMinValues.z = tmp;
		}
	}

	static inline void correctLimitsWorld(BBV *bv)
	{
		float tmp;

		if ((bv->worldMinValues.x - bv->worldMaxValues.x) > 0.0f)
		{
			tmp = bv->worldMaxValues.x;
			bv->worldMaxValues.x = bv->worldMinValues.x;
			bv->worldMinValues.x = tmp;
		}
		if ((bv->worldMinValues.y - bv->worldMaxValues.y) > 0.0f)
		{
			tmp = bv->worldMaxValues.y;
			bv->worldMaxValues.y = bv->worldMinValues.y;
			bv->worldMinValues.y = tmp;
		}
		if (bv->is3D && ((bv->worldMinValues.z - bv->worldMaxValues.z) > 0.0f))
		{
			tmp = bv->worldMaxValues.z;
			bv->worldMaxValues.z = bv->worldMinValues.z;
			bv->worldMinValues.z = tmp;
		}
	}

	// axis-aligned vs axis-aligned
	Collision testCollisionAABVvsAABV(const BBV *aaba_a, const BBV *aabv_b);

	// axis-aligned vs object-aligned
	Collision testCollisionAABVvsOBV(const BBV *aabv, const BBV *obv);

	// object-aligned vs object-aligned
	Collision testCollisionOBVvsOBV(const BBV *obv_a, const BBV *obv_b);

}



//-----------------------------------------------------------------------------
// COLLISION UTILITY FUNCTIONS

// create SBV
demo::SBV demo::createSBV(
	cbmath::vec3 center, 
	float radius
)
{
	// set values
	SBV newBV;
	newBV.center.set(center, 1.0f);
	newBV.radius = (radius > 0.0f) ? radius : 0.0f;

	// done
	return newBV;
}

// update SBV
void demo::setCenterSBV(SBV *bv, cbmath::vec3 newPosition)
{
	bv->center.set(newPosition, 1.0f);
}


// create BBV
demo::BBV demo::createBBV(
	cbmath::vec3 center,
	cbmath::vec3 localMinValues,
	cbmath::vec3 localMaxValues, 
	bool isAxisAligned, 
	bool is3D
	// other
)
{
	// set values
	BBV newBV;
	newBV.center.set(center, 1.0f);
	newBV.localMinValues.set(localMinValues);
	newBV.localMaxValues.set(localMaxValues);

	newBV.isAxisAligned = isAxisAligned;
	newBV.is3D = is3D;

	correctLimitsLocal(&newBV);

	updateWorldLimits(&newBV);


	// correct limits

	// update world limits


	// TO-DO: SET MORE STUFF


	// done
	return newBV;
}

// update BBV
void demo::setCenterBBV(BBV *bv, cbmath::vec3 newPosition)
{
	bv->center.set(newPosition, 1.0f);
}

void demo::updateWorldLimits(BBV *bv)
{
	// if axis-aligned, just add local corners to position
	bv->worldMinValues = bv->center + bv->localMinValues;
	bv->worldMaxValues = bv->center + bv->localMaxValues;


	// TO-DO: OBJECT-ALIGNED LIMITS
	// if object-aligned, transform local corners fully
	// use anti-scale matrix to correct if necessary


	// correct if necessary

}



// collision tests
demo::Collision demo::testCollisionSBVvsSBV(const SBV *a, const SBV *b)
{
	Collision result = { BVT_NONE };

	cbmath::vec4 displacements = a->center - b->center;

	float distSq = cbmath::dot(displacements, displacements);

	float radiiSum = a->radius + b->radius;

	if (distSq < radiiSum * radiiSum) {
		result.bvtA = result.bvtB = BVT_SPHERE_CIRCLE;

		//todo response
	}
	// done
	return result;
}

demo::Collision demo::testCollisionBBVvsBBV(const BBV *a, const BBV *b)
{
	Collision result;

	// select which test to use for the case of two boxes
	if (a->isAxisAligned && b->isAxisAligned)
	{
		// both AA
		result = testCollisionAABVvsAABV(a, b);
	}
	else if (a->isAxisAligned)
	{
		// first AA
		result = testCollisionAABVvsOBV(a, b);
	}
	else if (b->isAxisAligned)
	{
		// second AA
		result = testCollisionAABVvsOBV(b, a);

		// for this case need to swap order of return params
		//	so the user sees that their OBV was the first arg
		if (result.bvtA)
		{
			result.bvtA = BVT_BOX_OBJECT;
			result.bvtB = BVT_BOX_AXIS_ALIGNED;
		}
	}
	else
	{
		// neither AA
		result = testCollisionOBVvsOBV(a, b);
	}

	// done
	return result;
}

// aabv-aabv
demo::Collision demo::testCollisionAABVvsAABV(const BBV *aabv_a, const BBV *aabv_b)
{
	Collision result = { BVT_NONE };
	bool pass;
	if (aabv_a->worldMaxValues.x > aabv_b->worldMinValues.x &&
		aabv_b->worldMaxValues.x > aabv_a->worldMinValues.x &&
		aabv_a->worldMaxValues.y > aabv_b->worldMinValues.y &&
		aabv_b->worldMaxValues.y > aabv_a->worldMinValues.y)
	{
		if (aabv_a->is3D && aabv_b->is3D) 
		{
			pass = aabv_a->worldMaxValues.z > aabv_b->worldMinValues.z &&
				aabv_b->worldMaxValues.z > aabv_a->worldMinValues.z;
		}
		else 
		{
			pass = true;
		}
		if (pass)
		{
			result.bvtA = result.bvtB = BVT_BOX_AXIS_ALIGNED;
		}
	}

	// done
	return result;
}

// aabv-obv
demo::Collision demo::testCollisionAABVvsOBV(const BBV *aabv, const BBV *obv)
{
	Collision result = { BVT_NONE };


	{
		// TO-DO: HANDLE RESPONSE
	}


	// done
	return result;
}

// obv-obv
demo::Collision demo::testCollisionOBVvsOBV(const BBV *obv_a, const BBV *obv_b)
{
	Collision result = { BVT_NONE };


	{
		// TO-DO: HANDLE RESPONSE
	}


	// done
	return result;
}


demo::Collision demo::testCollisionSBVvsBBV(const SBV *a, const BBV *b)
{
	Collision result = { BVT_NONE };


	{
		// TO-DO: HANDLE RESPONSE
	}


	// done
	return result;
}
