/*
EGP 200 Graphics Starter - Cube Data
By Dan Buckstein
(C) 2016

Data structures and constants used for primitives.
*/


#ifndef __DEMO_PRIMITIVEDATA_H
#define __DEMO_PRIMITIVEDATA_H


namespace demo
{

	// simple data structure to hold 2D and 3D data points
	// use where appropriate
	struct Data2D
	{
		float x, y;
	};

	struct Data3D
	{
		float x, y, z;
	};

	// sizes/ constants 
	//	p1 = positive 1
	//	n1 = negative 1
	//	ph = positive half
	//	nh = negative half
	const float p1 = 1.0f, n1 = -1.0f, ph = 0.5f, nh = -0.5f;

}	// namespace demo


#endif	// __DEMO_PRIMITIVEDATA_H