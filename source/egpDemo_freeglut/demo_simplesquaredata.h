/*
EGP 200 Graphics Starter - Simple Square Data
By Dan Buckstein
(C) 2016

Raw data and utilities for creating a simple square VBO.
Complete with pictures to illustrate the data!
*/


#ifndef __DEMO_SIMPLESQUAREDATA_H
#define __DEMO_SIMPLESQUAREDATA_H


#include "demo_primitivedata.h"


namespace demo
{

// 3D vertex attribute data for a unit square centered about the origin
// includes vertices, colors, and texture coordinates
// unique data arranged first, then properly sequenced for VBO, or indexed VBO


	const unsigned int simpleSquareVertexCount = 4;


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

	// vertex positions for solid square (triangle strip)
	const Data3D simpleSquareVertices[] = {
		{ n1, n1, 00 },					// 0
		{ p1, n1, 00 },					// 1
		{ n1, p1, 00 },					// 2
		{ p1, p1, 00 },					// 3
	};

	// vertex positions for wireframe (line loop)
	const Data3D simpleSquareVerticesWire[] = {
		simpleSquareVertices[0],
		simpleSquareVertices[1], 
		simpleSquareVertices[3], 
		simpleSquareVertices[2], 
	};


//-----------------------------------------------------------------------------
// unique vertex normals
//		+z					+z
//		o-------------------o
//	   /|				   /|
//	  /	|				  /	|
//	 	|				 	|
//		|					|
//		|					|
//		|					|
//		|					|
//		o-------------------o
//	   /+z				   /+z
//	  /					  /					 

	const Data3D simpleSquareNormals[] = {
		{ 00, 00, p1 },					
		{ 00, 00, p1 },					
		{ 00, 00, p1 },					
		{ 00, 00, p1 },					
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

	const Data3D simpleSquareTexcoords[] = {
		{ 00, 00 },
		{ p1, 00 }, 
		{ 00, p1 }, 
		{ p1, p1 }, 
	};




//-----------------------------------------------------------------------------

}	// namespace demo


#endif	// __DEMO_SIMPLESQUAREDATA_H