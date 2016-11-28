/*
	EGP Graphics Starter Utilities
	Function prototypes by Dan Buckstein
*/

#ifndef __DEMO_UTILS_H
#define __DEMO_UTILS_H



namespace demo
{

//-----------------------------------------------------------------------------
// PURE EVIL IMMEDIATE MODE FUNCTIONS

	void drawTestTriangle();
	void drawTestRect();
	void drawTestAxes();



//-----------------------------------------------------------------------------
// SEMI-EVIL SEMI-RETAINED MODE FUNCTIONS

	// PART 1A: start using draw arrays
	void drawTriangleAttribs();
	void drawSquareAttribs();



//-----------------------------------------------------------------------------
// NICE RETAINED MODE FUNCTIONS

	// PART 1B: general vbo
	unsigned int createMultipleVBOs(unsigned int numVerts, unsigned int attribCount, 
		const float **data, unsigned int *handlesOut);


	void drawMultipleVBOs(unsigned int numVerts, unsigned int attribCount, 
		unsigned int primType, const unsigned int *handles, const unsigned int *attribIndex);


	void deleteMultipleVBOs(unsigned int attribCount, unsigned int *handles);



	// PART 2A: create interleaved VBO
	unsigned int createInterleavedVBO(unsigned int numVerts, unsigned int attribCount, 
		const float **data);


	void drawInterleavedVBO(unsigned int numVerts, unsigned int attribCount, 
		unsigned int primType, unsigned int handle, const unsigned int *attribIndex);


	void deleteBufferObject(unsigned int handle);


	// PART 2B: tie interleaved VBO to VAO and draw VAO
	unsigned int createVAO(unsigned int numVerts, unsigned int attribCount, 
		const float **data, const unsigned int *attribIndex, unsigned int *vboHandleOut);


	void drawVAO(unsigned int numVerts, unsigned int primType, unsigned int vaoHandle);


	void deleteVAO(unsigned int vaoHandle);


	// PART 2C: create VAO with IBO
	unsigned int createIndexedVAO(unsigned int numVerts, unsigned int numIndices, 
		unsigned int attribCount, const float **data, const unsigned int *attribIndex, 
		const unsigned int *indexData, unsigned int *vboHandleOut, unsigned int *iboHandleOut);


	void drawIndexedVAO(unsigned int numIndices, unsigned int primType, 
		unsigned int indexType, unsigned int vaoHandle);



//-----------------------------------------------------------------------------

	// PART 3A: shaders
	unsigned int createShaderfromSource(const char *src, unsigned int shaderType);

	void deleteShader(unsigned int shaderHandle);

	unsigned int createProgram();

	int attachShaderToProgram(unsigned int programHandle, unsigned int shaderHandle);

	int linkProgram(unsigned int programHandle);

	int validateProgram(unsigned int programHandle);

	void activateProgram(unsigned int programHandle);

	void deleteProgram(unsigned int programHandle);



//-----------------------------------------------------------------------------
// general utilities

	// file loading
	struct FileInfo
	{
		char *str;
		unsigned int len;
	};

	FileInfo loadFile(const char *path);
	int unloadFile(FileInfo *fileInfo);



//-----------------------------------------------------------------------------
	// default indices for attributes
	// these are what OpenGL uses for built-in attributes
	// if you align your data correctly with these, you 
	//	will not need to use a shader (programmed pipe)!!!
	enum ATTRIB_INDEX
	{
		A_POSITION,		// vertex location, point in space
		A_BLWT,			// irrelevant for now (bl. wt)
		A_NORMAL,		// normal vector
		A_COLOR0,		// colour
		A_COLOR1,		// alt colour
		A_FOG,			// irrelevant for now (fog)
		A_PTSZ,			// point size
		A_BLIND,		// irrelevant for now (bl. wt)
		A_TEXCOORD0,	// texcoord
		A_TEXCOORD1,	// "
		A_TEXCOORD2,	// "
		A_TEXCOORD3,	// "
		A_TEXCOORD4,	// "
		A_TEXCOORD5,	// "
		A_TEXCOORD6,	// "
		A_TEXCOORD7,	// "

		ATTRIB_COUNT	// 16
	};



//-----------------------------------------------------------------------------

}



#endif	// __DEMO_UTILS_H