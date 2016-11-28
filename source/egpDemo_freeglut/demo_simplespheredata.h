/*
EGP 200 Graphics Starter - Simple Sphere Data
By Dan Buckstein
(C) 2016

Raw data and utilities for creating a simple "sphere" VBO.
Complete with pictures to illustrate the data!
*/


#ifndef __DEMO_SIMPLESPHEREDATA_H
#define __DEMO_SIMPLESPHEREDATA_H


#include "demo_primitivedata.h"


namespace demo
{



	// value used for "subdivisions": 
	//	1 / root 3 = 0.57735026918962576450914878050196f
	const float pv = 0.57735026918962576450914878050196f;
	const float nv = -pv;


// 3D vertex attribute data for a very-slightly tessellated cube, which 
//	results in a sphere-like shape
// good news is that since it's a unit-shape (+/-1 in each direction), we can 
//	make things easy by reusing the position attribute (since they're just 
//	vectors) as the normals!

	const Data3D simpleSphereVertices[] = {
		{ nv, nv, pv },					// 0
		{ pv, nv, pv },					// 1
		{ nv, pv, pv },					// 2
		{ pv, pv, pv },					// 3
		{ nv, pv, nv },					// 4
		{ pv, pv, nv },					// 5
		{ nv, nv, nv },					// 6
		{ pv, nv, nv },					// 7

		{ 00, 00, p1 },					// 08
		{ 00, p1, 00 },					// 09
		{ 00, 00, n1 },					// 10
		{ 00, n1, 00 },					// 11
		{ p1, 00, 00 },					// 12
		{ n1, 00, 00 },					// 13
	};


/*
	cube: 
	0, 1, 2, 3, 2, 1,	// +z
	2, 3, 4, 5, 4, 3,	// +y
	4, 5, 6, 7, 6, 5,	// -z
	6, 7, 0, 1, 0, 7,	// -y
	1, 7, 3, 5, 3, 1,	// +x, 
	6, 0, 4, 2, 4, 0,	// -x

	with tessellation: 
*/
	const unsigned int simpleSphereIndices[] = {
		0, 1,  8, 1, 3,  8, 3, 2,  8, 2, 0,  8,	// +z
		2, 3,  9, 3, 5,  9, 5, 4,  9, 4, 2,  9,	// +y
		4, 5, 10, 5, 7, 10, 7, 6, 10, 6, 4, 10,	// -z
		6, 7, 11, 7, 1, 11, 1, 0, 11, 0, 6, 11,	// -y
		1, 7, 12, 7, 5, 12, 5, 3, 12, 3, 1, 12,	// +x, 
		6, 0, 13, 0, 2, 13, 2, 4, 13, 4, 6, 13,	// -x
	};


	
	// sorted by triangle: 
	const Data3D simpleSphereVertBuffer[] = {
		simpleSphereVertices[0], simpleSphereVertices[1], simpleSphereVertices[ 8], simpleSphereVertices[1], simpleSphereVertices[3], simpleSphereVertices[ 8], simpleSphereVertices[3], simpleSphereVertices[2], simpleSphereVertices[ 8], simpleSphereVertices[2], simpleSphereVertices[0], simpleSphereVertices[ 8],	// +z
		simpleSphereVertices[2], simpleSphereVertices[3], simpleSphereVertices[ 9], simpleSphereVertices[3], simpleSphereVertices[5], simpleSphereVertices[ 9], simpleSphereVertices[5], simpleSphereVertices[4], simpleSphereVertices[ 9], simpleSphereVertices[4], simpleSphereVertices[2], simpleSphereVertices[ 9],	// +y
		simpleSphereVertices[4], simpleSphereVertices[5], simpleSphereVertices[10], simpleSphereVertices[5], simpleSphereVertices[7], simpleSphereVertices[10], simpleSphereVertices[7], simpleSphereVertices[6], simpleSphereVertices[10], simpleSphereVertices[6], simpleSphereVertices[4], simpleSphereVertices[10],	// -z
		simpleSphereVertices[6], simpleSphereVertices[7], simpleSphereVertices[11], simpleSphereVertices[7], simpleSphereVertices[1], simpleSphereVertices[11], simpleSphereVertices[1], simpleSphereVertices[0], simpleSphereVertices[11], simpleSphereVertices[0], simpleSphereVertices[6], simpleSphereVertices[11],	// -y
		simpleSphereVertices[1], simpleSphereVertices[7], simpleSphereVertices[12], simpleSphereVertices[7], simpleSphereVertices[5], simpleSphereVertices[12], simpleSphereVertices[5], simpleSphereVertices[3], simpleSphereVertices[12], simpleSphereVertices[3], simpleSphereVertices[1], simpleSphereVertices[12],	// +x, 
		simpleSphereVertices[6], simpleSphereVertices[0], simpleSphereVertices[13], simpleSphereVertices[0], simpleSphereVertices[2], simpleSphereVertices[13], simpleSphereVertices[2], simpleSphereVertices[4], simpleSphereVertices[13], simpleSphereVertices[4], simpleSphereVertices[6], simpleSphereVertices[13],	// -x
	};


}	// namespace demo


#endif	// __DEMO_SIMPLESPHEREDATA_H