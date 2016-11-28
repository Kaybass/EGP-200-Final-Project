/*
EGP Graphics Starter
Sprite controller structure and function prototypes by Dan Buckstein
*/

#ifndef __DEMO_SPRITECONTROLLER_H
#define __DEMO_SPRITECONTROLLER_H



namespace demo
{


	// structure for describing a sprite sheet
	// tldr it describes how the image is sized and broken up
	struct SpriteSheetDescriptor
	{
		unsigned int imgWidth, imgHeight;		
		unsigned int cols, rows;				
		float frameWidth, frameHeight;			
	};

	// structure for managing an animating sprite, physically in the world
	struct SpriteController
	{
		SpriteSheetDescriptor *spriteInfo;		
		unsigned int currentCol, currentRow;	
		float spriteWidth, spriteHeight;		
		float worldPosX, worldPosY, worldPosZ;	


		////****TO DO: ADD MORE VALUES TO SUIT YOUR NEEDS
		float currentFrameTime, fps, spf;
	};


	// create a descriptor for a sprite sheet
	SpriteSheetDescriptor createSpriteInfo(unsigned int imageWidthPixels, unsigned int imageHeightPixels, unsigned int numCols, unsigned int numRows);


	// send size and frame info as uniforms
	void spriteSendUniforms(SpriteController *sprite, int frameSizeLoc, int currentFrameLoc);


	// prepare transformations for sprite and send to GPU as uniform
	void spriteUpdateWorldTransform(SpriteController *sprite, float *modelMat);


	// update based on time and check if we need to move to the next frame
	void spriteUpdate(SpriteController *sprite, float dt);



}	// namespace demo


#endif	// __DEMO_SPRITECONTROLLER_H