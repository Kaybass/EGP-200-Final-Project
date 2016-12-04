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

//Constants
#define BOINGO_SPEEDO cbmath::vec3(1.0f, 0.0f, 0.0f)
#define BOINGO_DRAG   cbmath::vec4(0.1f, 0.0f, 0.0f, 0.0f)

#define BONGO_ACCEL   cbmath::vec3(1.12f,0.0f,0.0f)



//-----------------------------------------------------------------------------
// globals

// window
int win_x = 0;
int win_y = 0;
int win_w = 1280;
int win_h = 720;
int win_handle;
float win_aspect;
const char *win_name = "Boingo's Big Bingo Bango";
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

unsigned int punty = 0; //<- so important!

unsigned int puntyCount[] = { punty, punty }; // gigapunty. 

// VAO handle for primitives

unsigned int testCubeVAO = 0;
unsigned int testSkyboxVAO = 0;
unsigned int testSquareVAO = 0;
unsigned int testCubeWireVAO = 0;
unsigned int testSquareWireVAO = 0;

// VBO handles

unsigned int testCubeInterleavedVBO = 0;
unsigned int testSquareInterleavedVBO = 0;
unsigned int testCubeWireInterleavedVBO = 0;
unsigned int testSquareWireInterleavedVBO = 0;

// IBO handles (indexed rendering)

unsigned int testCubeIBO = 0;
unsigned int testCubeWireIBO = 0;


// program handles
unsigned int drawAttribColorProgram = 0;
unsigned int drawSolidColorProgram = 0;
unsigned int drawTextureProgram = 0;


// uniform handles
unsigned int drawAttribColor_mvp = 0;
unsigned int drawSolidColor_mvp = 0, drawSolidColor_color = 0;
unsigned int frameHandle = 0;


// Sprites
unsigned int bob = 0;
unsigned int badBoi = 0;
unsigned int skybox = 0;

// some color presets for quick and easy uniforms
const float redTrans[4] = { 1.0f, 0.0f, 0.0f, 0.5f };
const float greenTrans[4] = { 0.0f, 1.0f, 0.0f, 0.5f };
const float blueTrans[4] = { 0.0f, 0.0f, 1.0f, 0.5f };

cbtk::cbmath::mat4 cubeViewMatrix, cubeViewProjectionMatrix;


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


// transforms
cbtk::cbmath::mat4	sphereModelMatrix0, sphereModelMatrixInv0, sphereModelViewProjectionMatrix0, 
					sphereModelMatrix1, sphereModelMatrixInv1, sphereModelViewProjectionMatrix1,
					cubeModelMatrix0, cubeModelMatrixInv0, cubeModelViewProjectionMatrix0,
					cubeModelMatrix1, cubeModelMatrixInv1, cubeModelViewProjectionMatrix1;


// physics objects
demo::Mover cube0, cube1;

demo::BBV cubeAABV0, cubeAABV1;

bool cube0intersect = false, cube1intersect = false;

//unsigned int ani = 0;// Increments whenever you press the button. 


//-----------------------------------------------------------------------------
// game functions

// load and delete geometry
void loadGeometry()
{
	// attribute indices (match up with the attribute inputs in the VERTEX SHADER!!!)
	const unsigned int testGeomAttribs[] = { demo::A_POSITION, demo::A_NORMAL, demo::A_TEXCOORD0}; //Position in time... dude... 
	const unsigned int testAxesAttribs[] = { demo::A_POSITION, demo::A_TEXCOORD0 };

	// SETUP SIMPLE CUBE VAO AND VBO (for skybox)
	const float *allSkyboxAttribData[] = { (float*)(demo::cubeVertBuffer), (float*)(demo::cubeTexcoordBuffer) };
	testSkyboxVAO = demo::createVAO(36, 2, allSkyboxAttribData, testAxesAttribs, &testCubeInterleavedVBO);

//-----------------------------------------------------------------------------

	// SETUP SIMPLE CUBE VAO AND VBO
	const float *allCubeAttribData[] = { (float*)(demo::cubeVertBuffer), (float *)(demo::cubeNormBuffer), (float*)(demo::cubeTexcoordBuffer)};
	testCubeVAO = demo::createVAO(demo::simpleCubeVertexCount, 3, allCubeAttribData, testGeomAttribs, &testCubeInterleavedVBO);

	// WIRE CUBE
	const float *allCubeWireAttribData[] = { (float *)(demo::cubeVertBufferWire) };
	testCubeWireVAO = demo::createVAO(demo::simpleCubeVertexCountWire, 1, allCubeWireAttribData, testGeomAttribs, &testCubeWireInterleavedVBO);
/**/
	// SETUP AXES


	// SETUP SQUARE VAO AND VBO
	const float *allSquareAttribData[] = { (float*)(demo::simpleSquareVertices) , (float*)(demo::simpleSquareNormals), (float*)(demo::simpleSquareTexcoords) };
	testSquareVAO = demo::createVAO(demo::simpleSquareVertexCount, 1, allSquareAttribData, testGeomAttribs, &testSquareInterleavedVBO);

	// WIRE SQUARE
	const float *allSquareWireAttribData[] = { (float *)(demo::simpleSquareVerticesWire) };
	testSquareWireVAO = demo::createVAO(demo::simpleSquareVertexCount, 1, allSquareWireAttribData, testGeomAttribs, &testSquareWireInterleavedVBO);


//-----------------------------------------------------------------------------
}

void deleteGeometry()
{
	// delete renderable data


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

	{
		// program for skybox drawing
		

			fsFile = demo::loadFile("../../../../resource/glsl/450/drawTexture_fs.glsl");
			fsHandle = demo::createShaderfromSource(fsFile.str, GL_FRAGMENT_SHADER);

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
			

			// done with shared shader object
			demo::deleteShader(fsHandle);
			demo::unloadFile(&fsFile);
		
	}

	// uniforms
	drawAttribColor_mvp = glGetUniformLocation(drawAttribColorProgram, "mvp");

	drawSolidColor_mvp = glGetUniformLocation(drawSolidColorProgram, "mvp");
	drawSolidColor_color = glGetUniformLocation(drawSolidColorProgram, "solidColor");
	frameHandle = glGetUniformLocation(drawSolidColorProgram, "frame");


	// done
	demo::activateProgram(0);
}

void deleteShaderPrograms()
{
	demo::deleteProgram(drawAttribColorProgram);
	demo::deleteProgram(drawSolidColorProgram);
}


// load and delete textures
void loadTextures()
{

	skybox = ilutGLLoadImage("../../../../resource/tex/bg/sky_water.png");
	glBindTexture(GL_TEXTURE_2D, skybox);							// activate 2D texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// texture gets large, smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// texture gets small, smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// texture repeats on horiz axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// texture repeats on vert axis



	bob = ilutGLLoadImage("../../../../resource/tex/sprite/bob.png");

	glBindTexture(GL_TEXTURE_2D, bob);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// these two are deliberately different

	badBoi = ilutGLLoadImage("../../../../resource/tex/sprite/bongo.png");

	glBindTexture(GL_TEXTURE_2D, badBoi);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// these two are deliberately different


	glBindTexture(GL_TEXTURE_2D, 0);									// deactivate 2D texture
}

void deleteTextures()
{

	iluDeleteImage(skybox);
	iluDeleteImage(bob);
	iluDeleteImage(badBoi);

}


// physics objects
void resetPhysics(int playFlag)
{
	// **acceleration** due to gravity on earth (not a force!)
	const cbmath::vec3 gravity(0.0f, -9.80665f, 0.0f);

	// set playing flag
	playing = playFlag;




	cube0 = demo::createMover(
		cbmath::vec3(-2.0f, -2.0f, 0.02f), BONGO_ACCEL, cbmath::v3zero, 1.0f, &cubeModelMatrix0);
	
	cube1 = demo::createMover(
		cbmath::vec3(+2.0f, -2.0f, 0.03f), cbmath::v3zero, cbmath::v3zero, 1.0f, &cubeModelMatrix1);

	demo::addAcceleration(&cube0, cbmath::vec3(2.00f, 0.0f, 0.0f));



	demo::updateMoverEuler(&cube0, 0.0f);
	demo::updateMoverEuler(&cube1, 0.0f);



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
	//updateCameraControlled(dt);
	updateCameraOrbit(dt);



	viewMatrix.c3 = cameraPosWorld;
	viewMatrix = cbtk::cbmath::transformInverseNoScale(viewMatrix);
	viewProjMat = projectionMatrix * viewMatrix;

	cubeViewMatrix = viewMatrix;
	cubeViewMatrix.c3.set(0.0f, 0.0f, 0.0f, 1.0f);
	cubeViewMatrix = cubeViewMatrix * cbtk::cbmath::makeScale4(50.0f);




	// update control
	if (playing)
	{
		// update movable objects

		//demo::setVelocity(&cube0,cbmath::vec3(0.01f,0.0f,0.0f));

		//apply drag
		cube1.velocity = cube1.velocity -  BOINGO_DRAG;

		demo::updateMoverEuler(&cube0, dt);
		demo::updateMoverEuler(&cube1, dt);

		
		cameraPosWorld = cube1.position + cbmath::vec4(0.0f, 2.0f, cameraDistance, 1.0f);

		// update colliders


		demo::setCenterBBV(&cubeAABV0, cube0.position.xyz);
		demo::setCenterBBV(&cubeAABV1, cube1.position.xyz);

		demo::updateWorldLimits(&cubeAABV0);
		demo::updateWorldLimits(&cubeAABV1);


		// update collision checks
		demo::Collision ssbvTest, aabvTest;
		aabvTest = demo::testCollisionBBVvsBBV(&cubeAABV0, &cubeAABV1);
		cube0intersect = cube1intersect = (aabvTest.bvtA != demo::BVT_NONE);

		if (cube0intersect) {
			cube0intersect = cube1intersect = false;

			cameraPosWorld = cbmath::vec4(0.0f, 0.0f, cameraDistance, 1.0f);

			printf("\nGame Over, you earned %d punty\n", punty);

			punty = 0;

			resetPhysics(0);
		}


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


	// use simple texturing program
	demo::activateProgram(drawTextureProgram);

	// bind skybox texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skybox);

	// send skybox transform
	cubeViewProjectionMatrix = viewMatrix;
	glUniformMatrix4fv(0, 1, 0, cubeViewProjectionMatrix.m);

	// draw skybox - flip culling
	glCullFace(GL_FRONT);
	demo::drawVAO(36, GL_TRIANGLES, testSkyboxVAO);
	glCullFace(GL_BACK);


	puntyCount[0] = punty % 8; // gigapunty. 
	puntyCount[1] = punty % 8; // gigapunty. 

	//printf("%d\n", punty % 3);
//-----------------------------------------------------------------------------
// test geometry

	// draw objects at center
	// red for solid object, green if colliding, blue for outline
	demo::activateProgram(drawSolidColorProgram);
	
	// cube 0: 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, badBoi);

	// solid
	cubeModelViewProjectionMatrix0 = viewProjMat * cubeModelMatrix0;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, cubeModelViewProjectionMatrix0.m);
	glUniform4fv(drawSolidColor_color, 1, (cube0intersect ? greenTrans : redTrans));
	glUniform2f(frameHandle, 1, (GLfloat)*puntyCount);
	demo::drawVAO(demo::simpleCubeVertexCount, GL_TRIANGLES, testCubeVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCount, GL_TRIANGLES, GL_UNSIGNED_INT, testCubeVAO);

	// outline
	//glUniform4fv(drawSolidColor_color, 1, blueTrans);
	//demo::drawVAO(demo::simpleCubeVertexCountWire, GL_LINES, testCubeWireVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCountWire, GL_LINES, GL_UNSIGNED_INT, testCubeWireVAO);
	
	//glutSolidTeapot(2);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bob);


	// cube 1: 
	cubeModelViewProjectionMatrix1 = viewProjMat * cubeModelMatrix1;
	glUniformMatrix4fv(drawSolidColor_mvp, 1, 0, cubeModelViewProjectionMatrix1.m);
	glUniform4fv(drawSolidColor_color, 1, (cube1intersect ? greenTrans : redTrans));
	glUniform2f(frameHandle, 1, (GLfloat) *puntyCount);
	demo::drawVAO(demo::simpleCubeVertexCount, GL_TRIANGLES, testCubeVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCount, GL_TRIANGLES, GL_UNSIGNED_INT, testCubeVAO);

	//glUniform4fv(drawSolidColor_color, 1, blueTrans);
	//demo::drawVAO(demo::simpleCubeVertexCountWire, GL_LINES, testCubeWireVAO);
//	demo::drawIndexedVAO(demo::simpleCubeIndexCountWire, GL_LINES, GL_UNSIGNED_INT, testCubeWireVAO);



	//glutSolidTeapot(2);


//-----------------------------------------------------------------------------


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
			//deleteShaderPrograms();
			//loadShaderPrograms();
			break;
			// reset physics shapes
		case 'L': 
			//resetPhysics(0);
			break;

			// PAUSE
		case 'p': 
			//playing = (1 - playing);
			break;

			// CAMERA CONTROLS
		case 'A':
		case 'a':
			//keyLeft = -1;
			break;
		case 'D':
		case 'd':
			//keyRight = 1;
			break;
		case 'Q': 
		case 'q': 
			//keyDown = -1;
			break;
		case 'E':
		case 'e':
			//keyUp = 1;
			break;
		case 'W':
		case 'w':
			//keyFwd = -1;
			break;
		case 'S':
		case 's':
			//keyBack = 1;
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
			//keyLeft = 0;
			break;
		case 'D':
		case 'd':
			//keyRight = 0;
			break;
		case 'Q':
		case 'q':
			//keyDown = 0;
			break;
		case 'E':
		case 'e':
			//keyUp = 0;
			break;
		case 'W':
		case 'w':
			//keyFwd = 0;
			break;
		case 'S':
		case 's':
			//keyBack = 0;
			break;
		case 'Z':
		case 'z':
			//move boingo
			if (!playing)
				playing = true;
			demo::addVelocity(&cube1, BOINGO_SPEEDO);
			//cameraPosWorld = cameraPosWorld + cbmath::vec4(1.0f,0.0f, 0.0f, 0.0f);
			punty++;
			break;
		case 'X':
		case 'x':
			//move boingo
			if (!playing)
				playing = true;
			demo::addVelocity(&cube1, BOINGO_SPEEDO);
			//cameraPosWorld = cameraPosWorld + cbmath::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			punty++;
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
	//printf("\n GL version: %d.%d", *(version), *(version + 1));

	//Print Game Rules and Welcome
	printf("\nWelcome to Boingo's Big Bingo Bango\n\nTo play you must make Boingo run away from Bongo by using Z and X\n");

	// initialize extensions
	int init = glewInit();
	if (init == GLEW_OK)
	{
		// check version again to confirm initialization was successful
		// should be higher than the previous check if default is not highest
		glGetIntegerv(GL_MAJOR_VERSION, version);
		glGetIntegerv(GL_MINOR_VERSION, version + 1);
		//printf("\n GL version: %d.%d", *(version), *(version + 1));

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
