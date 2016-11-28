/*
Dan Buckstein - Sprite Controller
*/


#include "demo_spritecontroller.h"

#include "GL/glew.h"

#include "cbmath/cbtkMatrix.h"

#include <memory.h>



////****TO DO: 
// create a descriptor for a sprite sheet
demo::SpriteSheetDescriptor demo::createSpriteInfo(unsigned int imageWidthPixels,
	unsigned int imageHeightPixels, unsigned int numCols, unsigned int numRows)
{
	// TO DO: 
	//	- set variables in structure
	//	- calculate frame size within 0-1 range

	// start with a dummy instance of our sprite sheet descriptor
	//	with all values hard-set to 1
	SpriteSheetDescriptor result = { 1,1,1,1,1,1 };

	result.imgWidth    = imageWidthPixels;
	result.imgHeight   = imageHeightPixels;
	result.cols        = numCols;
	result.rows        = numRows;
	result.frameWidth  = 1.0f / (float) numCols;
	result.frameHeight = 1.0f / (float) numRows;

	// done
	return result;
}


// send size and frame info as uniforms
void demo::spriteSendUniforms(SpriteController *sprite,
	int frameSizeLoc, int currentFrameLoc)
{
	////****TO DO: 
	//	- send 2 float values for frame size to whatever program is currently active
	//	- send 2 float values for our current col and row indices

	glUniform2f(frameSizeLoc, sprite->spriteInfo->frameWidth,
							  sprite->spriteInfo->frameHeight);

	glUniform2f(currentFrameLoc, (float) sprite->currentCol,
								 (float) sprite->currentRow);

}


// prepare transformations for sprite and send to GPU as uniform
void demo::spriteUpdateWorldTransform(SpriteController *sprite, float *modelMat)
{
	// we are assuming "modelMat" represents the values in a 4x4 matrix 
	//	...so really it is an array of 16 floats
	// first write zeros to all values in the matrix
	memset(modelMat, 0, sizeof(float) * 16);

	// set scale
	modelMat[0] = sprite->spriteWidth;
	modelMat[5] = sprite->spriteHeight;
	modelMat[10] = 1.0f;
	modelMat[15] = 1.0f;

	// set translate
	modelMat[12] = sprite->worldPosX;
	modelMat[13] = sprite->worldPosY;
	modelMat[14] = sprite->worldPosZ;
}


////****TO DO: 
// update based on time and check if we need to move to the next frame
void demo::spriteUpdate(SpriteController *sprite, float dt)
{
	// TO DO: 
	//	- update sprite's frame time
	//	- if it is time to change frames, figure out how this should be handled: 
	//		-> are we going forward? reverse? looping? ping-ponging? stop at end? up to you...

	sprite->currentFrameTime += dt;
	if (sprite->currentFrameTime >= sprite->spf) {
		sprite->currentFrameTime -= sprite->spf;
		sprite->currentCol = (sprite->currentCol + 1) % sprite->spriteInfo->cols;
	}


	// TEMPORARY HAX: 
	// ALWAYS CHANGE FRAME
//	
}

