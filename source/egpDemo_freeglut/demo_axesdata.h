/*
EGP 200 Graphics Starter - Axes Data
By Dan Buckstein
(C) 2016

Raw data and utilities for creating a VBO for coordinate axes.
Complete with pictures to illustrate the data!
*/


#ifndef __DEMO_AXESDATA_H
#define __DEMO_AXESDATA_H


#include "demo_primitivedata.h"


namespace demo
{

// 3D vertex attribute data for coordinate axes centered about the origin
// includes vertices and colors
// unique data arranged first, then properly sequenced for VBO, or indexed VBO


	const unsigned int axesVertexCount = 6;


//-----------------------------------------------------------------------------
// unique vertices
//			+y
//			o
//			|
//			|
//			|		  +x
//			o---------o
//		   / origin (zero)
//		  /
//		 o
//		+z
//	

	const Data3D axesVertices[] = {
		{ 00, 00, 00 },					// 0
		{ p1, 00, 00 },					// 1
		{ 00, p1, 00 },					// 2
		{ 00, 00, p1 },					// 3
	};


//-----------------------------------------------------------------------------
// unique colors
//			green
//			o
//			|
//			|
//			|		  red
//			o---------o
//		   / grey
//		  /
//		 o
//		blue
//	


	const Data3D axesColors[] = {
		{ p1, 00, 00 },					// red
		{ 00, p1, 00 },					// green
		{ 00, 00, p1 },					// blue
	};



//-----------------------------------------------------------------------------
// vertex attributes organized for **NON-INDEXED** VBO using LINES
// num lines = 3 (x, y, and z)
// num vertices = 2 x num lines = 6

	const Data3D axesVertBuffer[axesVertexCount] = {
		axesVertices[0], axesVertices[1],	// first line
		axesVertices[0], axesVertices[2],	// second line
		axesVertices[0], axesVertices[3],	// third line
	};

	const Data3D axesColorBuffer[axesVertexCount] = {
		axesColors[0], axesColors[0],
		axesColors[1], axesColors[1],
		axesColors[2], axesColors[2],
	};




//-----------------------------------------------------------------------------

}	// namespace demo


#endif	// __DEMO_AXESDATA_H