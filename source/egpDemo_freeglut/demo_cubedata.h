/*
EGP 200 Graphics Starter - Cube Data
By Dan Buckstein
(C) 2016

Raw data and utilities for creating a cube VBO.
Complete with pictures to illustrate the data!
*/


#ifndef __DEMO_CUBEDATA_H
#define __DEMO_CUBEDATA_H


#include "demo_primitivedata.h"


namespace demo
{

// 3D vertex attribute data for a unit cube centered about the origin
// includes vertices, colors, normals and texture coordinates
// unique data arranged first, then properly sequenced for VBO, or indexed VBO


	const unsigned int simpleCubeVertexCount = 36;
	const unsigned int simpleCubeVertexCountWire = 24;
	const unsigned int simpleCubeVertexCountIndexed = 24;
	const unsigned int simpleCubeVertexCountWireIndexed = 8;
	const unsigned int simpleCubeIndexCount = simpleCubeVertexCount;
	const unsigned int simpleCubeIndexCountWire = simpleCubeVertexCountWire;


//-----------------------------------------------------------------------------
// unique vertices
//	   4					 5
//		o-------------------o
//	   /|				   /|
//	  /	|				  /	|
// 2 /	|			   3 /	|
//	o-------------------o	|
//	|	|				|	|
//	|	|				|	|
//	|	|				|	|
//	|	o---------------|---o
//	|  / 6				|  / 7
//	| /					| /
//	|/					|/
//	o-------------------o
// 0					 1

	const Data3D cubeVertices[simpleCubeVertexCountWireIndexed] = {
		{ n1, n1, p1 },					// 0
		{ p1, n1, p1 },					// 1
		{ n1, p1, p1 },					// 2
		{ p1, p1, p1 },					// 3
		{ n1, p1, n1 },					// 4
		{ p1, p1, n1 },					// 5
		{ n1, n1, n1 },					// 6
		{ p1, n1, n1 },					// 7
	};


//-----------------------------------------------------------------------------
// unique vertex colors
//	   green				 yellow
//		o-------------------o
//	   /|				   /|
//	  /	|				  /	|
// cyan	|			   white|
//	o-------------------o	|
//	|	|				|	|
//	|	|				|	|
//	|	|				|	|
//	|	o---------------|---o
//	|  / black			|  / red
//	| /					| /
//	|/					|/
//	o-------------------o
// blue					 magenta

	const Data3D cubeColors[simpleCubeVertexCountWireIndexed] = {
		{ 00, 00, p1 },					// blue
		{ p1, 00, p1 },					// magenta
		{ 00, p1, p1 },					// cyan
		{ p1, p1, p1 },					// white
		{ 00, p1, 00 },					// green
		{ p1, p1, 00 },					// yellow
		{ 00, 00, 00 },					// black
		{ p1, 00, 00 },					// red
	};


//-----------------------------------------------------------------------------
// normals are oriented to each face
//				+y
//				^
//				|
//				|  /
//				| /
//				|/
//	 -----------o-----------> +x
//			   /|
//			  /	|
//			 /	|
//			+z	|

	const Data3D cubeNormals[] = {
		{ 00, 00, p1 },					// positive z
		{ 00, p1, 00 },					// positive y
		{ 00, 00, n1 },					// negative z
		{ 00, n1, 00 },					// negative y
		{ p1, 00, 00 },					// positive x
		{ n1, 00, 00 },					// negative x
	};


//-----------------------------------------------------------------------------
// texcoords are arranged in an upside down 'T'............and here's how they map to our vertex positions: 
//	(0, 1)								(1, 1)				(0, 1)								(1, 1)	
//		+-----------8-------9-----------+				 		+-----------0-------1-----------+		
//		|			|		|			|				 		|			|		|			|		
//		|			|		|			|				 		|			|		|			|		
//		|			6-------7			|			->			|			6-------7			|		
//		|			|		|			|			->->	 	|			|		|			|		
//		|			|		|			|	->->->->->->->		|			|		|			|		
//		|			4-------5			|	->->->->->->->->	|			4-------5			|		
//		|			|		|			|	->->->->->->->		|			|		|			|		
//		|			|		|			|			->->		|			|		|			|		
//		|	10------2-------3------11	|			->			|	4-------2-------3-------5	|		
//		|	|		|		|		|	|				 		|	|		|		|		|	|		
//		|	|		|		|		|	|				 		|	|		|		|		|	|		
//		+---12------0-------1------13---+				 		+---6-------0-------1-------7---+		
//	(0, 0)								(1, 0)				(0, 0)								(1, 0)
//					texcoords										vertices unfolded into net

	const Data3D cubeTexcoords[] = {
		{ 0.375f, 0.0f },{ 0.625f, 0.0f },{ 0.375f, 0.25f },{ 0.625f, 0.25f },
		{ 0.375f, 0.5f },{ 0.625f, 0.5f },{ 0.375f, 0.75f },{ 0.625f, 0.75f },
		{ 0.375f, 1.0f },{ 0.625f, 1.0f },{ 0.125f, 0.25f },{ 0.875f, 0.25f },
		{ 0.125f, 0.0f },{ 0.875f, 0.0f }
	};




//-----------------------------------------------------------------------------
// vertex attributes organized for **NON-INDEXED** VBO using TRIANGLES
// all of this data gets interleaved into a single VBO... notice how some of 
//	the data is redundant...
// num faces = 6 (because cube)
// num triangles = 2 x num faces = 12
// num vertices = 3 x num triangles = 36
// want to verify that this data is correct?  look at the data indices and 
//	match them with the diagrams provided above!

	// vertices for solid cube (triangles)
	const Data3D cubeVertBuffer[simpleCubeVertexCount] = {
		cubeVertices[0], cubeVertices[1], cubeVertices[2],	// first triangle
		cubeVertices[3], cubeVertices[2], cubeVertices[1],	// second triangle...
		cubeVertices[2], cubeVertices[3], cubeVertices[4],
		cubeVertices[5], cubeVertices[4], cubeVertices[3],
		cubeVertices[4], cubeVertices[5], cubeVertices[6],
		cubeVertices[7], cubeVertices[6], cubeVertices[5],
		cubeVertices[6], cubeVertices[7], cubeVertices[0],
		cubeVertices[1], cubeVertices[0], cubeVertices[7],
		cubeVertices[1], cubeVertices[7], cubeVertices[3],
		cubeVertices[5], cubeVertices[3], cubeVertices[7],
		cubeVertices[6], cubeVertices[0], cubeVertices[4],
		cubeVertices[2], cubeVertices[4], cubeVertices[0],
	};

	// vertices for wire cube (lines)
	const Data3D cubeVertBufferWire[simpleCubeVertexCountWire] = {
		cubeVertices[0], cubeVertices[1],	// first line (front face)
		cubeVertices[1], cubeVertices[3],	// second line
		cubeVertices[3], cubeVertices[2],
		cubeVertices[2], cubeVertices[0],
		cubeVertices[0], cubeVertices[6],	// (connecting edges)
		cubeVertices[1], cubeVertices[7],
		cubeVertices[3], cubeVertices[5],
		cubeVertices[2], cubeVertices[4],
		cubeVertices[4], cubeVertices[5],	// (back face)
		cubeVertices[5], cubeVertices[7],
		cubeVertices[7], cubeVertices[6],
		cubeVertices[6], cubeVertices[4],
	};

	const Data3D cubeColorBuffer[simpleCubeVertexCount] = {
		cubeColors[0], cubeColors[1], cubeColors[2],
		cubeColors[3], cubeColors[2], cubeColors[1],
		cubeColors[2], cubeColors[3], cubeColors[4],
		cubeColors[5], cubeColors[4], cubeColors[3],
		cubeColors[4], cubeColors[5], cubeColors[6],
		cubeColors[7], cubeColors[6], cubeColors[5],
		cubeColors[6], cubeColors[7], cubeColors[0],
		cubeColors[1], cubeColors[0], cubeColors[7],
		cubeColors[1], cubeColors[7], cubeColors[3],
		cubeColors[5], cubeColors[3], cubeColors[7],
		cubeColors[6], cubeColors[0], cubeColors[4],
		cubeColors[2], cubeColors[4], cubeColors[0],
	};

	const Data3D cubeNormBuffer[simpleCubeVertexCount] = {
		cubeNormals[0], cubeNormals[0], cubeNormals[0],
		cubeNormals[0], cubeNormals[0], cubeNormals[0],
		cubeNormals[1], cubeNormals[1], cubeNormals[1],
		cubeNormals[1], cubeNormals[1], cubeNormals[1],
		cubeNormals[2], cubeNormals[2], cubeNormals[2],
		cubeNormals[2], cubeNormals[2], cubeNormals[2],
		cubeNormals[3], cubeNormals[3], cubeNormals[3],
		cubeNormals[3], cubeNormals[3], cubeNormals[3],
		cubeNormals[4], cubeNormals[4], cubeNormals[4],
		cubeNormals[4], cubeNormals[4], cubeNormals[4],
		cubeNormals[5], cubeNormals[5], cubeNormals[5],
		cubeNormals[5], cubeNormals[5], cubeNormals[5],
	};

	const Data3D cubeTexcoordBuffer[simpleCubeVertexCount] = {
		cubeTexcoords[0], cubeTexcoords[1], cubeTexcoords[2],
		cubeTexcoords[3], cubeTexcoords[2], cubeTexcoords[1],
		cubeTexcoords[2], cubeTexcoords[3], cubeTexcoords[4],
		cubeTexcoords[5], cubeTexcoords[4], cubeTexcoords[3],
		cubeTexcoords[4], cubeTexcoords[5], cubeTexcoords[6],
		cubeTexcoords[7], cubeTexcoords[6], cubeTexcoords[5],
		cubeTexcoords[6], cubeTexcoords[7], cubeTexcoords[8],
		cubeTexcoords[9], cubeTexcoords[8], cubeTexcoords[7],
		cubeTexcoords[1], cubeTexcoords[13], cubeTexcoords[3],
		cubeTexcoords[11], cubeTexcoords[3], cubeTexcoords[13],
		cubeTexcoords[12], cubeTexcoords[0], cubeTexcoords[10],
		cubeTexcoords[2], cubeTexcoords[10], cubeTexcoords[0],
	};




//-----------------------------------------------------------------------------
// support for indexed rendering (using IBO): organizes the geometry by index
// all this data gets interleaved into a single VBO, just like above, but 
//	there is less data in the VBO

	const Data3D cubeVertIndexed[simpleCubeVertexCountIndexed] = {
		cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3],
		cubeVertices[2], cubeVertices[3], cubeVertices[4], cubeVertices[5],
		cubeVertices[4], cubeVertices[5], cubeVertices[6], cubeVertices[7],
		cubeVertices[6], cubeVertices[7], cubeVertices[0], cubeVertices[1],
		cubeVertices[1], cubeVertices[7], cubeVertices[3], cubeVertices[5],
		cubeVertices[6], cubeVertices[0], cubeVertices[4], cubeVertices[2],
	};

	const Data3D cubeColorIndexed[simpleCubeVertexCountIndexed] = {
		cubeColors[0], cubeColors[1], cubeColors[2], cubeColors[3],
		cubeColors[2], cubeColors[3], cubeColors[4], cubeColors[5],
		cubeColors[4], cubeColors[5], cubeColors[6], cubeColors[7],
		cubeColors[6], cubeColors[7], cubeColors[0], cubeColors[1],
		cubeColors[1], cubeColors[7], cubeColors[3], cubeColors[5],
		cubeColors[6], cubeColors[0], cubeColors[4], cubeColors[2],
	};

	const Data3D cubeNormIndexed[simpleCubeVertexCountIndexed] = {
		cubeNormals[0], cubeNormals[0], cubeNormals[0], cubeNormals[0],
		cubeNormals[1], cubeNormals[1], cubeNormals[1], cubeNormals[1],
		cubeNormals[2], cubeNormals[2], cubeNormals[2], cubeNormals[2],
		cubeNormals[3], cubeNormals[3], cubeNormals[3], cubeNormals[3],
		cubeNormals[4], cubeNormals[4], cubeNormals[4], cubeNormals[4],
		cubeNormals[5], cubeNormals[5], cubeNormals[5], cubeNormals[5],
	};

	const Data3D cubeTexcoordIndexed[simpleCubeVertexCountIndexed] = {
		cubeTexcoords[0], cubeTexcoords[1], cubeTexcoords[2], cubeTexcoords[3],
		cubeTexcoords[2], cubeTexcoords[3], cubeTexcoords[4], cubeTexcoords[5],
		cubeTexcoords[4], cubeTexcoords[5], cubeTexcoords[6], cubeTexcoords[7],
		cubeTexcoords[6], cubeTexcoords[7], cubeTexcoords[8], cubeTexcoords[9],
		cubeTexcoords[1], cubeTexcoords[13], cubeTexcoords[3], cubeTexcoords[11],
		cubeTexcoords[12], cubeTexcoords[0], cubeTexcoords[10], cubeTexcoords[2],
	};

	// data for IBO (solid)
	const unsigned int cubeIndices[simpleCubeIndexCount] = {
		0,  1,  2,								// first triangle
		3,  2,  1,								// second triangle...
		4,  5,  6,
		7,  6,  5,
		8,  9, 10,
		11, 10,  9,
		12, 13, 14,
		15, 14, 13,
		16, 17, 18,
		19, 18, 17,
		20, 21, 22,
		23, 22, 21,
	};

	// IBO (wire)
	const unsigned int cubeIndicesWire[simpleCubeIndexCountWire] = {
		0, 1, 1, 3, 3, 2, 2, 0, 
		0, 6, 1, 7, 3, 5, 2, 4, 
		4, 5, 5, 7, 7, 6, 6, 4, 
	};




//-----------------------------------------------------------------------------

}	// namespace demo


#endif	// __DEMO_CUBEDATA_H