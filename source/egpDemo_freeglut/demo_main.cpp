///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////	EGP Graphics Demo using Freeglut
////	By Dan Buckstein
////	August 2016
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "IL/ilut.h"

#include "demo_utils.h"

// raw data for primitives
#include "demo_axesdata.h"
#include "demo_cubedata.h"
#include "demo_simplesquaredata.h"
#include "demo_spherelowresdata.h"
#include "demo_spherehighresdata.h"

// sprite utilities
#include "demo_spritecontroller.h"

// physics utilities
#include "demo_collisions.h"

// third party math lib
#include "cbmath/cbtkMatrix.h"



//-----------------------------------------------------------------------------
// globals

// window
int win_x = 0;
int win_y = 0;
int win_w = 1280;
int win_h = 720;
int win_handle;
float win_aspect;
const char *win_name = "EGP Graphics Demo Window - D. Buckstein";
const char win_close = 27;	// escape key closes window

// frame rate and timing
double fps = 60.0;
double spf = 1.0 / fps;
double mspf = 1000.0 / fps;

// update flag
int playing = 0;



//-----------------------------------------------------------------------------
// our game objects
// good practice: default values for everything

// VAO handle for primitives
unsigned int testAxesVAO = 0;
unsigned int testCubeVAO = 0;
unsigned int testSquareVAO = 0;
unsigned int testCubeWireVAO = 0;
unsigned int testSquareWireVAO = 0;
unsigned int testSphereLoResVAO = 0;
unsigned int testSphereHiResVAO = 0;

// VBO handles
unsigned int testAxesInterleavedVBO = 0;
unsigned int testCubeInterleavedVBO = 0;
unsigned int testSquareInterleavedVBO = 0;
unsigned int testCubeWireInterleavedVBO = 0;
unsigned int testSquareWireInterleavedVBO = 0;
unsigned int testSphereLoResInterleavedVBO = 0;
unsigned int testSphereHiResInterleavedVBO = 0;

// IBO handles (indexed rendering)
unsigned int testAxesIBO = 0;
unsigned int testCubeIBO = 0;
unsigned int testCubeWireIBO = 0;


// program handles
unsigned int drawAttribColorProgram = 0;
unsigned int drawSolidColorProgram = 0;

/*
unsigned int drawTextureProgram = 0;

unsigned int phongShadingProgram = 0;
unsigned int celShadingProgram = 0;
unsigned int goochShadingProgram = 0;
unsigned int celGoochShadingProgram = 0;
*/


// uniform handles
unsigned int drawAttribColor_mvp = 0;
unsigned int drawSolidColor_mvp = 0, drawSolidColor_color = 0;

// some color presets for quick and easy uniforms
const float redTrans[4] = { 1.0f, 0.0f, 0.0f, 0.5f };
const float greenTrans[4] = { 0.0f, 1.0f, 0.0f, 0.5f };
const float blueTrans[4] = { 0.0f, 0.0f, 1.0f, 0.5f };


/*
int drawTexture_mvp = 0;

int phongShading_mvp = 0, phongShading_lightPos = 0, phongShading_lightCol = 0, phongShading_eyePos = 0;
int celShading_mvp = 0, celShading_lightPos = 0, celShading_eyePos = 0;
int goochShading_mvp = 0, goochShading_lightPos = 0, goochShading_eyePos = 0;
int celGoochShading_mvp = 0, celGoochShading_lightPos = 0, celGoochShading_eyePos = 0;
*/



// general: camera's view matrix and projection matrix
cbtk::cbmath::mat4 viewMatrix, projectionMatrix, viewProjMat;
// camera controls
float cameraElevation = 0.0f, cameraAzimuth = 0.0f, deltaCamElev = 0.0f, deltaCamAzim = 0.0f;
float cameraRotateSpeed = 0.3f, cameraMoveSpeed = 1.0f, cameraDistance = 8.0f;
cbtk::cbmath::vec4 cameraPosWorld(0.0f, 0.0f, cameraDistance, 1.0f), deltaCamPos;

// simple mouse and keyboard tracker, used to control camera's movement
int mousePressed = 0;
int mouseX = 0, mouseY = 0, lastMouseX = 0, lastMouseY = 0, deltaMouseX = 0, deltaMouseY = 0;
int keyRight = 0, keyLeft = 0, keyUp = 0, keyDown = 0, keyFwd = 0, keyBack = 0;


/*
// light positions and colors
const unsigned int numLights = 3;
cbtk::cbmath::vec4 lightPosWorld[numLights];
const cbtk::cbmath::vec4 lightOffset[numLights] = {
	cbtk::cbmath::vec4(50.0f, 20.0f, 50.0f, 0.0f), 
	cbtk::cbmath::vec4(-50.0f, 20.0f, 50.0f, 0.0f), 
	cbtk::cbmath::vec4(50.0f, -20.0f, -50.0f, 0.0f)
};

// light and camera position to be calculated in object space
cbtk::cbmath::vec4 cameraPosObj;
cbtk::cbmath::vec4 lightPosObj[numLights];

// light colors
cbtk::cbmath::vec4 lightCol[numLights] = {
	cbtk::cbmath::vec4(1.0f),						// white
	cbtk::cbmath::vec4(1.0f, 0.5, 0.0f, 1.0f),		// orange
	cbtk::cbmath::vec4(0.0f, 0.5f, 1.0f, 1.0f)		// blue
};
*/


// transforms
cbtk::cbmath::mat4	sphereModelMatrix0, sphereModelMatrixInv0, sphereModelViewProjectionMatrix0, 
					sphereModelMatrix1, sphereModelMatrixInv1, sphereModelViewProjectionMatrix1,
					cubeModelMatrix0, cubeModelMatrixInv0, cubeModelViewProjectionMatrix0,
					cubeModelMatrix1, cubeModelMatrixInv1, cubeModelViewProjectionMatrix1;


// physics objects
demo::Mover sphere0, sphere1, cube0, cube1;
demo::SBV sphereSBV0, sphereSBV1;
demo::BBV cubeAABV0, cubeAABV1;

bool sphere0intersect = false, sphere1intersect = false, cube0intersect = false, cube1intersect = false;


/*
// cube model transformation in world space, and fully-concatenated matrix for cube
// we'll use it as a skybox, so no more model matrix
cbtk::cbmath::mat4 cubeViewMatrix, cubeViewProjectionMatrix;

// square model and model-view-projection
cbtk::cbmath::mat4 squareModelMatrix, squareModelMatrixInv, squareModelViewProjectionMatrix;

// transforms for spheres
cbtk::cbmath::mat4 sphereLoResModelMatrix, sphereLoResModelMatrixInv, sphereLoResModelViewProjectionMatrix;
cbtk::cbmath::mat4 sphereHiResModelMatrix, sphereHiResModelMatrixInv, sphereHiResModelViewProjectionMatrix;
float sphereTilt = Deg2Rad(23.5f);
float sphereRotate = 0.0f;
float sphereLoResHt = 0.0f;
float sphereHiResHt = 0.0f;
*/


/*
// textures:

// skybox
unsigned int skyboxHandle = 0;

// earth
unsigned int earthHandle_dm = 0;
unsigned int earthHandle_sm = 0;

// cel-shading map
unsigned int celRampHandle_dm = 0;
unsigned int celRampHandle_sm = 0;
*/



//-----------------------------------------------------------------------------
// game functions

// load and delete geometry
void loadGeometry()
{
	// attribute indices (match up with the attribute inputs in the VERTEX SHADER!!!)
	const unsigned int testGeomAttribs[] = { demo::A_POSITION, demo::A_NORMAL, demo::A_TEXCOORD0 };
	const unsigned int testAxesAttribs[] = { demo::A_POSITION, demo::A_COLOR0 };

//-----------------------------------------------------------------------------

	// SETUP SIMPLE CUBE VAO AND VBO
	const float *allCubeAttribData[] = { (float*)(demo::cubeVertBuffer) };//, (float *)(demo::cubeNormBuffer), (float*)(demo::cubeTexcoordBuffer) };
	testCubeVAO = demo::createVAO(demo::simpleCubeVertexCount, 1, allCubeAttribData, testGeomAttribs, &testCubeInterleavedVBO);

	// WIRE CUBE
	const float *allCubeWireAttribData[] = { (float *)(demo::cubeVertBufferWire) };
	testCubeWireVAO = demo::createVAO(demo::simpleCubeVertexCountWire, 1, allCubeWireAttribData, testGeomAttribs, &testCubeWireInterleavedVBO);
/**/
	// SETUP AXES
	const float *allAxesAttribData[] = { (float *)(demo::axesVertBuffer), (float *)(demo::axesColorBuffer) };
	testAxesVAO = demo::createVAO(demo::axesVertexCount, 2, allAxesAttribData, testAxesAttribs, &testAxesInterleavedVBO);

	// SETUP SQUARE VAO AND VBO
	const float *allSquareAttribData[] = { (float*)(demo::simpleSquareVertices) };//, (float*)(demo::simpleSquareNormals), (float*)(demo::simpleSquareTexcoords) };
	testSquareVAO = demo::createVAO(demo::simpleSquareVertexCount, 1, allSquareAttribData, testGeomAttribs, &testSquareInterleavedVBO);

	// WIRE SQUARE
	const float *allSquareWireAttribData[] = { (float *)(demo::simpleSquareVerticesWire) };
	testSquareWireVAO = demo::createVAO(demo::simpleSquareVertexCount, 1, allSquareWireAttribData, testGeomAttribs, &testSquareWireInterleavedVBO);

	// SETUP LOW-RES SPHERE VAO AND VBO
	const float *allSphereLoResAttribData[] = { (float*)(demo::sphere8x6verticesOrganized), (float*)(demo::sphere8x6verticesOrganized), (float*)(demo::sphere8x6texcoordsOrganized) };
	testSphereLoResVAO = demo::createVAO(demo::sphere8x6vertexCount, 3, allSphereLoResAttribData, testGeomAttribs, &testSphereLoResInterleavedVBO);

	// SETUP HIGH-RES SPHERE VAO AND VBO
	const float *allSphereHiResAttribData[] = { (float*)(demo::sphere32x24verticesOrganized), (float*)(demo::sphere32x24verticesOrganized), (float*)(demo::sphere32x24texcoordsOrganized) };
	testSphereHiResVAO = demo::createVAO(demo::sphere32x24vertexCount, 3, allSphereHiResAttribData, testGeomAttribs, &testSphereHiResInterleavedVBO);

//-----------------------------------------------------------------------------
/*
	// SETUP INDEXED CUBE VAO, VBO AND IBO
	const float *allCubeAttribDataIndexed[] = { (float*)(demo::cubeVertIndexed) };//, (float *)(demo::cubeNormIndexed), (float*)(demo::cubeTexcoordIndexed) };
	testCubeVAO = demo::createIndexedVAO(
		demo::simpleCubeVertexCountIndexed, demo::simpleCubeIndexCount, 
		1, allCubeAttribDataIndexed, testGeomAttribs, demo::cubeIndices, 
		&testCubeInterleavedVBO, &testCubeIBO);

	const float *allCubeWireAttribDataIndexed[] = { (float *)(demo::cubeVertices) };
	testCubeWireVAO = demo::createIndexedVAO(
		demo::simpleCubeVertexCountWireIndexed, demo::simpleCubeIndexCountWire,
		1, allCubeWireAttribDataIndexed, testGeomAttribs, demo::cubeIndicesWire, 
		&testCubeWireInterleavedVBO, &testCubeWireIBO);
*/
}

void deleteGeometry()
{
	// delete renderable data
	demo::deleteVAO(testAxesVAO);
	demo::deleteBufferObject(testAxesInterleavedVBO);
	demo::deleteBufferObject(testAxesIBO);

	demo::deleteVAO(testCubeVAO);
	demo::deleteBufferObject(testCubeIBO);
	demo::deleteBufferObject(testCubeInterleavedVBO);

	demo::deleteVAO(testSquareVAO);
	demo::deleteBufferObject(testSquareInterleavedVBO);

	demo::deleteVAO(testCubeWireVAO);
	demo::deleteBufferObject(testCubeWireIBO);
	demo::deleteBufferObject(testCubeWireInterleavedVBO);

	demo::deleteVAO(testSquareWireVAO);
	demo::deleteBufferObject(testSquareWireInterleavedVBO);

	demo::deleteVAO(testSphereLoResVAO);
	demo::deleteBufferObject(testSphereLoResInterleavedVBO);

	demo::deleteVAO(testSphereHiResVAO);
	demo::deleteBufferObject(testSphereHiResInterleavedVBO);
}


// load and delete shader programs... great utility for not having to 
//	restart the program whenever there is a shader bug!!!  :)
void loadShaderPrograms()
{
	// SETUP SHADERS
	demo::FileInfo vsFile, fsFile;
	unsigned int vsHandle, fsHandle;


	// draw attrib color
	{
		vsFile = demo::loadFile("../../../../resource/glsl/450/passColorAttrib_vs.glsl");
		fsFile = demo::loadFile("../../../../resource/glsl/450/drawColorAttrib_fs.glsl");

		vsHandle = demo::createShaderfromSource(vsFile.str, GL_VERTEX_SHADER);
		fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

		// configure program
		drawAttribColorProgram = demo::createProgram();
		demo::attachShaderToProgram(drawAttribColorProgram, vsHandle);
		demo::attachShaderToProgram(drawAttribColorProgram, fsHandle);
		demo::linkProgram(drawAttribColorProgram);
		demo::validateProgram(drawAttribColorProgram);

		demo::deleteShader(vsHandle);
		demo::deleteShader(fsHandle);
		demo::unloadFile(&vsFile);
		demo::unloadFile(&fsFile);
	}


	// draw solid color
	{
		vsFile = demo::loadFile("../../../../resource/glsl/450/applyTransform_vs.glsl");
		fsFile = demo::loadFile("../../../../resource/glsl/450/drawSolidColor_fs.glsl");

		vsHandle = demo::createShaderfromSource(vsFile.str, GL_VERTEX_SHADER);
		fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

		// configure program
		drawSolidColorProgram = demo::createProgram();
		demo::attachShaderToProgram(drawSolidColorProgram, vsHandle);
		demo::attachShaderToProgram(drawSolidColorProgram, fsHandle);
		demo::linkProgram(drawSolidColorProgram);
		demo::validateProgram(drawSolidColorProgram);

		demo::deleteShader(vsHandle);
		demo::deleteShader(fsHandle);
		demo::unloadFile(&vsFile);
		demo::unloadFile(&fsFile);
	}

	// uniforms
	drawAttribColor_mvp = glGetUniformLocation(drawAttribColorProgram, "mvp");

	drawSolidColor_mvp = glGetUniformLocation(drawSolidColorProgram, "mvp");
	drawSolidColor_color = glGetUniformLocation(drawSolidColorProgram, "solidColor");


/*
	// texture visualization & sprite program
	{
		// FRAGMENT SHADER WILL BE USED IN BOTH PROGRAMS... LOAD ONCE
		fsFile = demo::loadFile("../../../../resource/glsl/450/drawTexture_fs.glsl");
		fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

		// texture visualization
		{
			// create vertex shader
			vsFile = demo::loadFile("../../../../resource/glsl/450/passTexcoord_vs.glsl");
			vsHandle = demo::createShaderfromSource(vsFile.str, GL_VERTEX_SHADER);

			// configure program
			drawTextureProgram = demo::createProgram();
			demo::attachShaderToProgram(drawTextureProgram, vsHandle);
			demo::attachShaderToProgram(drawTextureProgram, fsHandle);
			demo::linkProgram(drawTextureProgram);
			demo::validateProgram(drawTextureProgram);

			demo::deleteShader(vsHandle);
			demo::unloadFile(&vsFile);
		}

		// done with shared shader object
		demo::deleteShader(fsHandle);
		demo::unloadFile(&fsFile);
	}

	// lighting
	{
		// VERTEX SHADER WILL BE USED IN BOTH PROGRAMS... LOAD ONCE
		vsFile = demo::loadFile("../../../../resource/glsl/450/phongShading_vs.glsl");
		vsHandle = demo::createShaderfromSource(vsFile.str, GL_VERTEX_SHADER);

		// phong shading
		{
			fsFile = demo::loadFile("../../../../resource/glsl/450/phongShading_fs.glsl");
			fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

			phongShadingProgram = demo::createProgram();
			demo::attachShaderToProgram(phongShadingProgram, vsHandle);
			demo::attachShaderToProgram(phongShadingProgram, fsHandle);
			demo::linkProgram(phongShadingProgram);
			demo::validateProgram(phongShadingProgram);

			demo::deleteShader(fsHandle);
			demo::unloadFile(&fsFile);
		}

		// cel shading
		{
			fsFile = demo::loadFile("../../../../resource/glsl/450/celShading_fs.glsl");
			fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

			celShadingProgram = demo::createProgram();
			demo::attachShaderToProgram(celShadingProgram, vsHandle);
			demo::attachShaderToProgram(celShadingProgram, fsHandle);
			demo::linkProgram(celShadingProgram);
			demo::validateProgram(celShadingProgram);

			demo::deleteShader(fsHandle);
			demo::unloadFile(&fsFile);
		}

		// gooch shading
		{
			fsFile = demo::loadFile("../../../../resource/glsl/450/goochShading_fs.glsl");
			fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

			goochShadingProgram = demo::createProgram();
			demo::attachShaderToProgram(goochShadingProgram, vsHandle);
			demo::attachShaderToProgram(goochShadingProgram, fsHandle);
			demo::linkProgram(goochShadingProgram);
			demo::validateProgram(goochShadingProgram);

			demo::deleteShader(fsHandle);
			demo::unloadFile(&fsFile);
		}

		// cel-gooch shading combo
		{
			fsFile = demo::loadFile("../../../../resource/glsl/450/celGoochShading_fs.glsl");
			fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

			celGoochShadingProgram = demo::createProgram();
			demo::attachShaderToProgram(celGoochShadingProgram, vsHandle);
			demo::attachShaderToProgram(celGoochShadingProgram, fsHandle);
			demo::linkProgram(celGoochShadingProgram);
			demo::validateProgram(celGoochShadingProgram);

			demo::deleteShader(fsHandle);
			demo::unloadFile(&fsFile);
		}

		demo::deleteShader(vsHandle);
		demo::unloadFile(&vsFile);
	}



	// get uniform locations
	// (-1 means uniform is not used... not a bad thing, just won't do anything)

	drawTexture_mvp = glGetUniformLocation(drawTextureProgram, "mvp");
	
	phongShading_mvp = glGetUniformLocation(phongShadingProgram, "mvp");
	phongShading_lightPos = glGetUniformLocation(phongShadingProgram, "lightPos");
	phongShading_eyePos = glGetUniformLocation(phongShadingProgram, "eyePos");

	celShading_mvp = glGetUniformLocation(celShadingProgram, "mvp");
	celShading_lightPos = glGetUniformLocation(celShadingProgram, "lightPos");
	celShading_eyePos = glGetUniformLocation(celShadingProgram, "eyePos");

	goochShading_mvp = glGetUniformLocation(goochShadingProgram, "mvp");
	goochShading_lightPos = glGetUniformLocation(goochShadingProgram, "lightPos");
	goochShading_eyePos = glGetUniformLocation(goochShadingProgram, "eyePos");

	celGoochShading_mvp = glGetUniformLocation(celGoochShadingProgram, "mvp");
	celGoochShading_lightPos = glGetUniformLocation(celGoochShadingProgram, "lightPos");
	celGoochShading_eyePos = glGetUniformLocation(celGoochShadingProgram, "eyePos");


	// send constant uniforms
	// light colors don't change so why not send them here
	phongShading_lightCol = glGetUniformLocation(phongShadingProgram, "lightCol");
	demo::activateProgram(phongShadingProgram);
	glUniform4fv(phongShading_lightCol, numLights, (float *)lightCol);
*/


	// done
	demo::activateProgram(0);
}

void deleteShaderPrograms()
{
	demo::deleteProgram(drawAttribColorProgram);
	demo::deleteProgram(drawSolidColorProgram);

/*
	demo::deleteProgram(drawTextureProgram);

	demo::deleteProgram(phongShadingProgram);
	demo::deleteProgram(celShadingProgram);
	demo::deleteProgram(goochShadingProgram);
	demo::deleteProgram(celGoochShadingProgram);
*/
}


// load and delete textures
void loadTextures()
{
/*
	// skybox
	skyboxHandle = ilutGLLoadImage("../../../../resource/tex/bg/sky_clouds.png");

	// here we can change the texture settings: 
	//	1. smoothing: do we want it to appear 
	//		rough/pixelated (GL_NEAREST) or smooth (GL_LINEAR)?
	//	2. do we want it to repeat (GL_REPEAT) 
	//		or clamp to the edges (GL_CLAMP) if texcoords are 
	//		less than 0 or greater than 1?
	glBindTexture(GL_TEXTURE_2D, skyboxHandle);							// activate 2D texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// texture gets large, smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// texture gets small, smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// texture repeats on horiz axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// texture repeats on vert axis


	// earth textures
	earthHandle_dm = ilutGLLoadImage("../../../../resource/tex/earth/2k/earth_dm_2k.png");
	earthHandle_sm = ilutGLLoadImage("../../../../resource/tex/earth/2k/earth_sm_2k.png");

	glBindTexture(GL_TEXTURE_2D, earthHandle_dm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// these two are deliberately different

	glBindTexture(GL_TEXTURE_2D, earthHandle_sm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	// value clamp map for cel-shading
	celRampHandle_dm = ilutGLLoadImage("../../../../resource/tex/sprite/celRamp_dm.png");
	celRampHandle_sm = ilutGLLoadImage("../../../../resource/tex/sprite/celRamp_sm.png");

	glBindTexture(GL_TEXTURE_2D, celRampHandle_dm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, celRampHandle_sm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
*/


	glBindTexture(GL_TEXTURE_2D, 0);									// deactivate 2D texture
}

void deleteTextures()
{
/*
	iluDeleteImage(skyboxHandle);

	iluDeleteImage(earthHandle_dm);
	iluDeleteImage(earthHandle_sm);

	iluDeleteImage(celRampHandle_dm);
	iluDeleteImage(celRampHandle_sm);
*/
}


// physics objects
void resetPhysics(int playFlag)
{
	// **acceleration** due to gravity on earth (not a force!)
	const cbmath::vec3 gravity(0.0f, -9.80665f, 0.0f);

	// set playing flag
	playing = playFlag;


	// movable objects
	sphere0 = demo::createMover(
		cbmath::vec3(-2.0f, -2.0f, 0.0f), cbmath::vec3(5.0f, 10.0f, 0.0f), gravity
//	);
		, 1.0f, &sphereModelMatrix0);
	sphere1 = demo::createMover(
		cbmath::vec3(+2.0f, -2.0f, 0.01f), cbmath::vec3(0.0f, 8.0f, 0.0f), gravity
//	);
		, 1.0f, &sphereModelMatrix1);
	cube0 = demo::createMover(
		cbmath::vec3(-2.0f, 2.0f, 0.02f), cbmath::v3zero, cbmath::v3zero
//	);
		, 1.0f, &cubeModelMatrix0);
	cube1 = demo::createMover(
		cbmath::vec3(+2.0f, 2.0f, 0.03f), cbmath::vec3(-2.0f, 10.0f, 0.0f), gravity
//	);
		, 1.0f, &cubeModelMatrix1);


	// force first update
	demo::updateMoverEuler(&sphere0, 0.0f);
	demo::updateMoverEuler(&sphere1, 0.0f);
	demo::updateMoverEuler(&cube0, 0.0f);
	demo::updateMoverEuler(&cube1, 0.0f);


	// create default colliders
	sphereSBV0 = demo::createSBV();
	sphereSBV1 = demo::createSBV();
	cubeAABV0 = demo::createBBV();
	cubeAABV1 = demo::createBBV();
}


//-----------------------------------------------------------------------------

// update camera
void updateCameraControlled(float dt)
{
	// TRACK MOUSE AND KEYS TO CONTROL CAMERA: 
	// remember, the view transform is the INVERSE of the camera's model transform, 
	//	so we can start by making a proper camera transform then invert it
	// 1. compute deltas (velocities)
	// 2. integrate!
	// 3. update view matrix (camera's transformation)

	deltaCamElev = (float)(deltaMouseY);
	deltaCamAzim = (float)(deltaMouseX);

	deltaMouseX = deltaMouseY = 0;	// reset mouse

	cameraElevation += deltaCamElev * dt * cameraRotateSpeed;
	cameraAzimuth += deltaCamAzim * dt * cameraRotateSpeed;

	viewMatrix = cbtk::cbmath::makeRotationEuler4ZYX(cameraElevation, cameraAzimuth, 0.0f);

	// apply current rotation to our movement vector so that "forward" is the direction the camera is facing
	deltaCamPos.set((float)(keyRight + keyLeft), (float)(keyUp + keyDown), (float)(keyFwd + keyBack), 0.0f);
	deltaCamPos = viewMatrix * deltaCamPos;

	cameraPosWorld += cbtk::cbmath::normalize(deltaCamPos) * dt * cameraMoveSpeed;
}

void updateCameraOrbit(float dt)
{
	// force camera to orbit around center of world
	cameraAzimuth += dt * cameraRotateSpeed;

	viewMatrix = cbtk::cbmath::makeRotationEuler4ZYX(-0.1f, cameraAzimuth, 0.0f);

	cameraPosWorld.set(sinf(cameraAzimuth)*cameraDistance, 1.0f, cosf(cameraAzimuth)*cameraDistance, 1.0f);
}



// initialize game objects
//	****mainly for good memory management, handling ram and vram
int initGame()
{
	// setup geometry
	loadGeometry();

	// setup shaders
	loadShaderPrograms();
	
	// setup textures
	loadTextures();

	// physics
	resetPhysics(0);

	// done
	return 1;
}

// destroy game objects (mem)
int termGame()
{
	// good practice to do this in reverse order of creation
	//	in case something is referencing something else

	// delete geometry
	deleteGeometry();

	// delete shader programs
	deleteShaderPrograms();

	// delete textures
	deleteTextures();


	// done
	return 1;
}


// game update to perform each frame
void update(float dt)
{
	// update game state using the time since the last update (dt) in seconds


	// camera
	updateCameraControlled(dt);
//	updateCameraOrbit(dt);

	viewMatrix.c3 = cameraPosWorld;
	viewMatrix = cbtk::cbmath::transformInverseNoScale(viewMatrix);
	viewProjMat = projectionMatrix * viewMatrix;


/*
	// SKYBOX TRANSFORM: 
	// easy way: exact same as the camera's transform (view), but 
	//	we remove the translation (which makes the background seem 
	//	infinitely far away), and scale up instead
	cubeViewMatrix = viewMatrix;
	cubeViewMatrix.c3.set(0.0f, 0.0f, 0.0f, 1.0f);
	cubeViewMatrix = cubeViewMatrix * cbtk::cbmath::makeScale4(50.0f);


	// update light positions (keep one of them attached to the skybox)
	lightPosWorld[0] = lightOffset[0] + cameraPosWorld;
	lightPosWorld[1] = lightOffset[1];
	lightPosWorld[2] = lightOffset[2];
*/


	// update control
	if (playing)
	{
		// update movable objects
		demo::updateMoverEuler(&sphere0, dt);
		demo::updateMoverEuler(&sphere1, dt);
		demo::updateMoverEuler(&cube0, dt);
		demo::updateMoverEuler(&cube1, dt);


		// update colliders
		demo::setCenterSBV(&sphereSBV0, sphere0.position.xyz);
		demo::setCenterSBV(&sphereSBV1, sphere1.position.xyz);

		demo::setCenterBBV(&cubeAABV0, cube0.position.xyz);
		demo::setCenterBBV(&cubeAABV1, cube1.position.xyz);

		demo::updateWorldLimits(&cubeAABV0);
		demo::updateWorldLimits(&cubeAABV1);


		// update collision checks
		demo::Collision ssbvTest, aabvTest;

		ssbvTest = demo::testCollisionSBVvsSBV(&sphereSBV0, &sphereSBV1);
		sphere0intersect = sphere1intersect = (ssbvTest.bvtA != demo::BVT_NONE);

		aabvTest = demo::testCollisionBBVvsBBV(&cubeAABV0, &cubeAABV1);
		cube0intersect = cube1intersect = (aabvTest.bvtA != demo::BVT_NONE);


/*
		// apply rotation to spheres' transforms
		sphereRotate += dt;
		sphereLoResHt = cosf(sphereRotate);
		sphereHiResHt = sinf(sphereRotate);

		// calculate rotation, shift over on x and apply height on y
		sphereLoResModelMatrix = cbtk::cbmath::makeRotationEuler4ZYX(sphereRotate, sphereRotate, sphereRotate);
		sphereLoResModelMatrix.c3.x = 5.0f;
		sphereLoResModelMatrix.c3.y = sphereLoResHt;
		sphereLoResModelMatrixInv = cbtk::cbmath::transformInverseNoScale(sphereLoResModelMatrix);

		// make it spin like the earth
		sphereHiResModelMatrix = cbtk::cbmath::makeRotationEuler4ZYX(0.0f, sphereRotate, sphereTilt);
		sphereHiResModelMatrix.c3.x = 2.5f;
		sphereHiResModelMatrixInv = cbtk::cbmath::transformInverseNoScale(sphereHiResModelMatrix);
*/

	}
}


// draw frame
//	****DRAWING AND UPDATING SHOULD BE SEPARATE (good practice)
void render()
{
	// typically begin frame by clearing buffers
	// alternatively, just redraw the background - clearing is expensive :)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// DRAW ALL OBJECTS - ALGORITHM: 
	//	- activate shader program if different from last object
	//	- bind texture we want to apply (skybox)
	//	- send appropriate uniforms if different from last time we used this program
	//	- call appropriate draw function, based on whether we are indexed or not


//-----------------------------------------------------------------------------
// AXES: 

	// just activate the color attrib program, send vp mat and draw as lines
	demo::activateProgram(drawAttribColorProgram);
	glUniformMatrix4fv(drawAttribColor_mvp, 1, 0, viewProjMat.m);

	demo::drawVAO(demo::axesVertexCount, GL_LINES, testAxesVAO);


	
//-----------------------------------------------------------------------------
// test geometry

	// draw objects at center
	// red for solid object, green if colliding, blue for outline
	demo::activateProgram(drawSolidColorProgram);
	
	// cube 0: 
	// solid
	cubeModelViewProjectionMatrix0 = viewProjMat * cubeModelMatrix0;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, cubeModelViewProjectionMatrix0.m);
	glUniform4fv(drawSolidColor_color, 1, (cube0intersect ? greenTrans : redTrans));
	demo::drawVAO(demo::simpleCubeVertexCount, GL_TRIANGLES, testCubeVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCount, GL_TRIANGLES, GL_UNSIGNED_INT, testCubeVAO);

	// outline
	glUniform4fv(drawSolidColor_color, 1, blueTrans);
	demo::drawVAO(demo::simpleCubeVertexCountWire, GL_LINES, testCubeWireVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCountWire, GL_LINES, GL_UNSIGNED_INT, testCubeWireVAO);
	
	// cube 1: 
	cubeModelViewProjectionMatrix1 = viewProjMat * cubeModelMatrix1;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, cubeModelViewProjectionMatrix1.m);
	glUniform4fv(drawSolidColor_color, 1, (cube1intersect ? greenTrans : redTrans));
	demo::drawVAO(demo::simpleCubeVertexCount, GL_TRIANGLES, testCubeVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCount, GL_TRIANGLES, GL_UNSIGNED_INT, testCubeVAO);

	glUniform4fv(drawSolidColor_color, 1, blueTrans);
	demo::drawVAO(demo::simpleCubeVertexCountWire, GL_LINES, testCubeWireVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCountWire, GL_LINES, GL_UNSIGNED_INT, testCubeWireVAO);


	// sphere 0: 
	sphereModelViewProjectionMatrix0 = viewProjMat * sphereModelMatrix0;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, sphereModelViewProjectionMatrix0.m);
	glUniform4fv(drawSolidColor_color, 1, (sphere0intersect ? greenTrans : redTrans));
	demo::drawVAO(demo::sphere8x6vertexCount, GL_TRIANGLES, testSphereLoResVAO);

	glUniform4fv(drawSolidColor_color, 1, blueTrans);
	glCullFace(GL_FRONT);
	demo::drawVAO(demo::sphere32x24vertexCount, GL_TRIANGLES, testSphereHiResVAO);
	glCullFace(GL_BACK);

	// sphere 1: 
	sphereModelViewProjectionMatrix1 = viewProjMat * sphereModelMatrix1;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, sphereModelViewProjectionMatrix1.m);
	glUniform4fv(drawSolidColor_color, 1, (sphere1intersect ? greenTrans : redTrans));
	demo::drawVAO(demo::sphere8x6vertexCount, GL_TRIANGLES, testSphereLoResVAO);

	glUniform4fv(drawSolidColor_color, 1, blueTrans);
	glCullFace(GL_FRONT);
	demo::drawVAO(demo::sphere32x24vertexCount, GL_TRIANGLES, testSphereHiResVAO);
	glCullFace(GL_BACK);


	
//-----------------------------------------------------------------------------


/*
//-----------------------------------------------------------------------------
// SKYBOX: 

	// use simple texturing program
	demo::activateProgram(drawTextureProgram);

	// bind skybox texture 
	//	(won't matter if we're not using a program that uses it...)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxHandle);

	// send skybox transform
	cubeViewProjectionMatrix = projectionMatrix * cubeViewMatrix;
	glUniformMatrix4fv(0, 1, 0, cubeViewProjectionMatrix.m);

	// draw skybox - flip culling
	glCullFace(GL_FRONT);
	demo::drawVAO(36, GL_TRIANGLES, testCubeVAO);
	glCullFace(GL_BACK);

	// alternatively draw indexed
//	demo::drawIndexedVAO(36, GL_TRIANGLES, GL_UNSIGNED_INT, testCubeVAO);



//-----------------------------------------------------------------------------
// DRAWING SCENE OBJECTS 
// first, activate program and textures


	// ACTIVATE SHADER PROGRAM
//	demo::activateProgram(drawTextureProgram);
	demo::activateProgram(phongShadingProgram);
//	demo::activateProgram(celShadingProgram);
//	demo::activateProgram(goochShadingProgram);
//	demo::activateProgram(celGoochShadingProgram);


	// change texture
	// if you want to use a different texture for each object, 
	//	need to do this whenever this texture (slot 0) should change
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthHandle_dm);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthHandle_sm);


	// activate our "cel-shading map": called a RAMP
	// this way we can avoid if-statements in our shader
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, celRampHandle_dm);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, celRampHandle_sm);



//-----------------------------------------------------------------------------
// FLOATING SPHERES: 
// may need to update stuff depending on which space lighting happens in...

	// update transform and send via uniform
	sphereLoResModelViewProjectionMatrix = viewProjMat * sphereLoResModelMatrix;
	glUniformMatrix4fv(0, 1, 0, sphereLoResModelViewProjectionMatrix.m);

	// LIGHTING: fastest if in object-space (local to object)
	// update light positions in object space
	for (unsigned int i = 0; i < numLights; ++i)
		lightPosObj[i] = sphereLoResModelMatrixInv * lightPosWorld[i];

	// send light positions in object space
	glUniform4fv(phongShading_lightPos, numLights, (float *)lightPosObj);

	// update and send eye position in object space
	cameraPosObj = sphereLoResModelMatrixInv * cameraPosWorld;
	glUniform4fv(phongShading_eyePos, 1, cameraPosObj.v);

	// draw
	demo::drawVAO(demo::sphere8x6vertexCount, GL_TRIANGLES, testSphereLoResVAO);


	// hi-res
	sphereHiResModelViewProjectionMatrix = viewProjMat * sphereHiResModelMatrix;
	glUniformMatrix4fv(0, 1, 0, sphereHiResModelViewProjectionMatrix.m);

	// lights
	for (unsigned int i = 0; i < numLights; ++i)
		lightPosObj[i] = sphereHiResModelMatrixInv * lightPosWorld[i];
	glUniform4fv(phongShading_lightPos, numLights, (float *)lightPosObj);

	// camera
	cameraPosObj = sphereHiResModelMatrixInv * cameraPosWorld;
	glUniform4fv(phongShading_eyePos, 1, cameraPosObj.v);

	// action
	demo::drawVAO(demo::sphere32x24vertexCount, GL_TRIANGLES, testSphereHiResVAO);



//-----------------------------------------------------------------------------
// SIMPLE SQUARE USING SAME SHADER: 


	squareModelViewProjectionMatrix = viewProjMat * squareModelMatrix;
	glUniformMatrix4fv(0, 1, 0, squareModelViewProjectionMatrix.m);

	for (unsigned int i = 0; i < numLights; ++i)
		lightPosObj[i] = squareModelMatrixInv * lightPosWorld[i];
	glUniform4fv(phongShading_lightPos, numLights, (float *)lightPosObj);

	cameraPosObj = squareModelMatrixInv * cameraPosWorld;
	glUniform4fv(phongShading_eyePos, 1, cameraPosObj.v);

	demo::drawVAO(demo::simpleSquareVertexCount, GL_TRIANGLE_STRIP, testSquareVAO);


//-----------------------------------------------------------------------------
*/

	// disable or reset all states
	demo::activateProgram(0);
	glActiveTexture(GL_TEXTURE0);



	// end frame by swapping buffers
	glutSwapBuffers();
}



//-----------------------------------------------------------------------------
// callbacks

// display - refers to the actual display refreshing, not draw time
void onDisplay()
{
	// ****DO NOT USE
}

// trigger new frame
void onTimer(int value)
{
	// trigger update and render
	update((float)spf);
	render();

	// reset timer
	glutTimerFunc((int)mspf, onTimer, 1);
}

// window closed
void onCloseWindow()
{
	// clean up
	termGame();
}

// window resized
void onResizeWindow(int w, int h)
{
	win_w = w;
	win_h = h;
	win_aspect = ((float)w) / ((float)h);

	// update viewport
	glViewport(0, 0, win_w, win_h);

	// update projection matrix
	projectionMatrix = cbtk::cbmath::makePerspective(1.0f, win_aspect, 1.0f, 100.0f);
	viewProjMat = projectionMatrix * viewMatrix;
}

// window moved
void onPositionWindow(int x, int y)
{
	win_x = x;
	win_y = y;
}

// key pressed (with cursor position)
void onKeyboard(unsigned char key, int x, int y)
{
	if (key != win_close)
	{
		switch (key)
		{
			// CONVENIENCE FEATURE: 
			// reload shaders
		case 'P': 
			deleteShaderPrograms();
			loadShaderPrograms();
			break;
			// reset physics shapes
		case 'L': 
			resetPhysics(0);
			break;

			// PAUSE
		case 'p': 
			playing = (1 - playing);
			break;

			// CAMERA CONTROLS
		case 'A':
		case 'a':
			keyLeft = -1;
			break;
		case 'D':
		case 'd':
			keyRight = 1;
			break;
		case 'Q': 
		case 'q': 
			keyDown = -1;
			break;
		case 'E':
		case 'e':
			keyUp = 1;
			break;
		case 'W':
		case 'w':
			keyFwd = -1;
			break;
		case 'S':
		case 's':
			keyBack = 1;
			break;
		}
	}
	else
	{
		// quit button pressed, stop game loop
		glutLeaveMainLoop();
	}
}

// key released
void onKeyboardUp(unsigned char key, int x, int y)
{
	if (key != win_close)
	{
		switch (key)
		{
			// CAMERA CONTROLS - STOP
		case 'A':
		case 'a':
			keyLeft = 0;
			break;
		case 'D':
		case 'd':
			keyRight = 0;
			break;
		case 'Q':
		case 'q':
			keyDown = 0;
			break;
		case 'E':
		case 'e':
			keyUp = 0;
			break;
		case 'W':
		case 'w':
			keyFwd = 0;
			break;
		case 'S':
		case 's':
			keyBack = 0;
			break;
		}
	}
}

// mouse button pressed or released (with cursor)
void onMouseButton(int button, int state, int x, int y)
{
	if (button == 0)
	{
		mousePressed = 1 - state;
		mouseX = lastMouseX = x;
		mouseY = lastMouseY = y;
		deltaMouseX = 0;
		deltaMouseY = 0;
	}
}

// mouse moved
void onMouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (mousePressed)
	{
		deltaMouseX = lastMouseX - mouseX;
		deltaMouseY = lastMouseY - mouseY;
	}
	else
	{
		deltaMouseX = 0;
		deltaMouseY = 0;
	}

	lastMouseX = mouseX;
	lastMouseY = mouseY;
}



//-----------------------------------------------------------------------------
// core functions

// initialize OpenGL using freeglut and GLEW
int initGL(int argc, char **argv)
{
	// create a window with double-buffering, RGBA colour and depth buffer
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(win_x, win_y);
	glutInitWindowSize(win_w, win_h);
	win_handle = glutCreateWindow(win_name);

	// check version
	int version[2];
	glGetIntegerv(GL_MAJOR_VERSION, version);
	glGetIntegerv(GL_MINOR_VERSION, version + 1);
	printf("\n GL version: %d.%d", *(version), *(version + 1));

	// initialize extensions
	int init = glewInit();
	if (init == GLEW_OK)
	{
		// check version again to confirm initialization was successful
		// should be higher than the previous check if default is not highest
		glGetIntegerv(GL_MAJOR_VERSION, version);
		glGetIntegerv(GL_MINOR_VERSION, version + 1);
		printf("\n GL version: %d.%d", *(version), *(version + 1));

		// set callbacks
		glutDisplayFunc(onDisplay);
		glutTimerFunc(0, onTimer, 0);
		glutCloseFunc(onCloseWindow);
		glutReshapeFunc(onResizeWindow);
		glutPositionFunc(onPositionWindow);
		glutKeyboardFunc(onKeyboard);
		glutKeyboardUpFunc(onKeyboardUp);
		glutMouseFunc(onMouseButton);
		glutMotionFunc(onMouseMove);
		glutPassiveMotionFunc(onMouseMove);

		// set states
		// backface culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// turn on depth (for now...)
	//	glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// alpha blending
		// result = ( new*[new alpha]) + (old*[1 - new alpha] )
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// textures
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);


		// lines
		glLineWidth(4.0f);


		// done
		return 1;
	}
	
	// fail
	return 0;
}

// initialize third-party image handling using DevIL
int initIL()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	// done
	return 1;
}

// entry function
int main(int argc, char **argv)
{
	// initialize graphics library
	if (initGL(argc, argv))
	{
		// initialize image library
		if (initIL())
		{
			// initialize game objects (pre-start)
			if (initGame())
			{
				// start loop (launch game)
				glutMainLoop();
			}
		}
	}

//	printf("\n\n");
//	system("pause");
}



//-----------------------------------------------------------------------------
