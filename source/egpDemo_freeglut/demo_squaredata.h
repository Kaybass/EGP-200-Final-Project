/*
EGP 200 Graphics Starter - Square Data
By Dan Buckstein
(C) 2016

Raw data and utilities for creating a square VBO.
Complete with pictures to illustrate the data!
*/


#ifndef __DEMO_SQUAREDATA_H
#define __DEMO_SQUAREDATA_H


#include "demo_primitivedata.h"


namespace demo
{

// 3D vertex attribute data for a unit square centered about the origin
// includes vertices, colors, and texture coordinates
// unique data arranged first, then properly sequenced for VBO, or indexed VBO

//-----------------------------------------------------------------------------
// unique vertices, arranged in this order so we can form a triangle strip
// 2					 3	
//	o-------------------o	
//	|\					|	
//	|	\				|	
//	|	   \			|	
//	|		  \			|	
//	|			 \		|	
//	|				\	|	
//	|				   \|	
//	o-------------------o
// 0					 1

	const Data3D squareVertices[] = {
		{ n1, n1, 00 },					// 0
		{ p1, n1, 00 },					// 1
		{ n1, p1, 00 },					// 2
		{ p1, p1, 00 },					// 3
	};


//-----------------------------------------------------------------------------
// unique vertex colors
//	green					yellow
//		o-------------------o
//		|					|
//		|					|
//		|					|
//		|					|
//		|					|
//		|					|
//		|					|
//		o-------------------o
//	black					red


	const Data3D squareColors[] = {
		{ 00, 00, 00 },					// black
		{ p1, 00, 00 },					// red
		{ 00, p1, 00 },					// green
		{ p1, p1, 00 },					// yellow
	};



//-----------------------------------------------------------------------------
// the full texcoord arrangement for a quad is just a square, all dimensions
//	have the range [0, 1]
// see the diagram for how the texcoords align with positions
//	(0, 1)								(1, 1)	
//		+-------------------------------+	
//		|vert2					   vert3|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|								|	
//		|vert0					   vert1|	
//		+-------------------------------+		
//	(0, 0)								(1, 0)	
//					texcoords					

	const Data3D squareTexcoords[] = {
		{ 00, 00 },
		{ p1, 00 }, 
		{ 00, p1 }, 
		{ p1, p1 }, 
	};




//-----------------------------------------------------------------------------
// vertex attributes organized for **NON-INDEXED** VBO using TRIANGLES
// num faces = 1 (because it's just a square)
// num triangles = 2 x num faces = 2 (2 triangles make a square)
// num vertices = 3 x num triangles = 6

	const Data3D squareVertBuffer[6] = {
		squareVertices[0], squareVertices[1], squareVertices[2],	// first triangle
		squareVertices[3], squareVertices[2], squareVertices[1],	// second triangle
	};

	const Data3D squareColorBuffer[6] = {
		squareColors[0], squareColors[1], squareColors[2],
		squareColors[3], squareColors[2], squareColors[1],
	};

	const Data3D squareTexcoordBuffer[6] = {
		squareTexcoords[0], squareTexcoords[1], squareTexcoords[2],
		squareTexcoords[3], squareTexcoords[2], squareTexcoords[1],
	};




//-----------------------------------------------------------------------------
// support for indexed rendering (using IBO): organizes the geometry by index
// notice how there are now fewer vertices in each array...

	const Data3D squareVertIndexed[4] = {
		squareVertices[0], squareVertices[1], squareVertices[2], squareVertices[3],
	};

	const Data3D squareColorIndexed[4] = {
		squareColors[0], squareColors[1], squareColors[2], squareColors[3],
	};

	const Data3D squareTexcoordIndexed[4] = {
		squareTexcoords[0], squareTexcoords[1], squareTexcoords[2], squareTexcoords[3],
	};


	// index buffer used to sample the above data from the VBO
	// EXPERIMENT: in the order denoted below, grab the "indexed" data from each 
	//	of the above three arrays and you should end up with the same sequence 
	//	as the non-indexed arrays!!!  (0, 1, 2, 3, 2, 1)
	const unsigned int squareIndices[6] = {
		0,  1,  2,								// first triangle
		3,  2,  1,								// second triangle
	};




//-----------------------------------------------------------------------------

}	// namespace demo


#endif	// __DEMO_SQUAREDATA_H