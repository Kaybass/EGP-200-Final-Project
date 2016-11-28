#include "demo_utils.h"


#include "GL/glew.h"



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>




void demo::drawTestTriangle()
{
	// immediate mode coloured triangle... DO NOT USE IMMEDIATE MODE!!!!
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.4f, -0.5f, -0.5f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.4f, -0.5f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.5f, 1.0f);
	glEnd();
}

void demo::drawTestRect()
{
	// immediate mode coloured rectangle... DO NOT USE IMMEDIATE MODE!!!!
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
	glEnd();
}

void demo::drawTestAxes()
{
	// immediate mode axes; r = x, g = y, b = z... DO NOT USE IMMEDIATE MODE!!!!
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// PART 1A: start using draw arrays
void demo::drawTriangleAttribs()
{
	// vertices in (x,y,z)
	static const float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,		// left, down
		0.2f, -0.5f, 0.0f,		// right, down
		0.5f,  0.5f, 0.0f,		// right, up
	};

	// colours in r-g-b
	static const float colours[] =
	{
		1.0f, 0.5f, 0.0f,		// orange
		0.0f, 1.0f, 0.5f,		// lime
		0.5f, 0.0f, 1.0f,		// grape
	};


	// enable vertex arrays - one for each attribute
	// ****USE CORRECT INDICES SO WE CAN DO THIS WITHOUT SHADERS!!!
	// like turning the lights on
	glEnableVertexAttribArray(A_POSITION);		// vertices ON
	glEnableVertexAttribArray(A_COLOR0);		// colours ON

	// send raw, unconfigured data
	// PROBLEM: data is sent every time we call this function... still sortof immediate
	glVertexAttribPointer(A_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(A_COLOR0, 3, GL_FLOAT, GL_FALSE, 0, colours);


	// draw
	glDrawArrays(GL_TRIANGLES, 0, 3);


	// disable
	// ****GOOD HABIT: turn stuff off when not in use
	// be responsible and turn the lights off when you're done with em!!!
	glDisableVertexAttribArray(A_POSITION);		// vertices OFF
	glDisableVertexAttribArray(A_COLOR0);		// colours OFF
}

void demo::drawSquareAttribs()
{
	static const float vertices[] =
	{
		-1.0f, -1.0f, -1.0f,	// bottom left
		 1.0f, -1.0f, -1.0f,	// bottom right
		-1.0f,  1.0f, -1.0f,	// top left
		 1.0f,  1.0f, -1.0f,	// top right
	};

	static const float colours[] =
	{
		0.0f, 0.0f, 0.0f,		// black
		1.0f, 0.0f, 0.0f,		// red
		0.0f, 1.0f, 0.0f,		// green
		1.0f, 1.0f, 0.0f,		// yellow
	};


	// enable vertex arrays - one for each attribute
	// like turning light switches on
	glEnableVertexAttribArray(A_POSITION);
	glEnableVertexAttribArray(A_COLOR0);
												
	// send raw, unconfigured data
	glVertexAttribPointer(A_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(A_COLOR0, 3, GL_FLOAT, GL_FALSE, 0, colours);


	// draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	// disable... be responsible and turn the lights off!!!
	glDisableVertexAttribArray(A_POSITION);
	glDisableVertexAttribArray(A_COLOR0);
}



//-----------------------------------------------------------------------------
// PART 1B: general vbo
unsigned int demo::createMultipleVBOs(unsigned int numVerts, unsigned int attribCount, 
	const float **data, unsigned int *handlesOut)
{
	const unsigned int sizeoffloat = sizeof(float);
	const unsigned int floatsPerAttrib = 3;	// <-- this may not be constant!!!

	// validate params
	if ((numVerts > 0) && (attribCount > 0) && (attribCount <= ATTRIB_COUNT) && 
		data && handlesOut)
	{
		// ALGORITHM: 
		//	- create enough buffers (VBOs) for all attributes in-use
		//	- for each attribute...
		//		- bind correct buffer to VBO TARGET
		//		- stuff buffer with data
		//	- disable using VBOs

		// EXAMPLE (not always the case!!!): 
		// if we have raw data arrays (data) holding the attribute values...
		//	positionsCPU = |pos0|pos1|pos2|...|
		//	normalsCPU   = |nrm0|nrm1|nrm2|...|
		//	texcoordsCPU = |tex0|tex1|tex2|...|
		// we want to transfer the data to different arrays that live on the GPU... called VBOS: 
		//	positionsCPU -> |VBO_0 (GPU)|
		//	normalsCPU ---> |VBO_1 (GPU)|
		//	texcoordsCPU -> |VBO_2 (GPU)|

		// create enough buffers to hold all the data
		glGenBuffers(attribCount, handlesOut);
		for (unsigned int i = 0; i < attribCount; ++i)
		{
			// bind buffer we want to send data to, one at a time
			// GL_ARRAY_BUFFER: target used for vertex attribute arrays
			// this is what makes our buffer mean it's being used as a VBO
			glBindBuffer(GL_ARRAY_BUFFER, handlesOut[i]);

			// ****SEND DATA TO GPU
			// size = sizeof(float) * numElementsPerVertex * numVerts
			glBufferData(GL_ARRAY_BUFFER, 
				(sizeoffloat * floatsPerAttrib * numVerts),
				data[i], GL_STATIC_DRAW);

			// EXAMPLE: if this attribute is positions...
			//	positionsCPU -> |VBO_0 (GPU)| = |pos0|pos1|pos2|pos3|pos4|...|
			// ...the buffer gets filled with data!!!
			// it basically copies our CPU-side array (data[i]) to 
			//	an array that lives on the GPU (***VBO_i***)
		}

		// DISABLE when done
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// done
		return attribCount;
	}

	// failed
	return 0;
}

void demo::drawMultipleVBOs(unsigned int numVerts, unsigned int attribCount, 
	unsigned int primType, const unsigned int *handles, const unsigned int *attribIndex)
{
	const unsigned int floatsPerAttrib = 3;	// unsafe assumption

	if ((numVerts > 0) && (attribCount > 0) && (attribCount <= ATTRIB_COUNT) && 
		handles && attribIndex)
	{
		// ALGORITHM: 
		//	- for each attribute...
		//		- bind correct buffer to VBO TARGET
		//		- enable the attribute (turn on the correct light switch, there are 16)
		//		- tell OpenGL where the data starts *IN THE CURRENTLY BOUND BUFFER* 
		//			(relative to the beginning of the VBO currently active)
		//	- DRAW
		//	- for each attribute...
		//		- disable the attribute (turn off the light)
		//	- disable using VBOs


		// configure attributes like we did before, assign data
		for (unsigned int i = 0; i < attribCount; ++i)
		{
			// bind appropriate buffer
			glBindBuffer(GL_ARRAY_BUFFER, handles[i]);

			// enable attribute (turn light switch on)
			glEnableVertexAttribArray(attribIndex[i]);

			// ****START DATA AT (NULL) because we are grabbing it from 
			//	the active buffer!
			glVertexAttribPointer(attribIndex[i], floatsPerAttrib,
				GL_FLOAT, GL_FALSE, 0, 0);
		}


		// ****DRAW in the same fashion as before
		glDrawArrays(primType, 0, numVerts);


		// disable attributes in the same fashion
		for (unsigned int i = 0; i < attribCount; ++i)
			glDisableVertexAttribArray(attribIndex[i]);

		// finally, disable use of buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void demo::deleteMultipleVBOs(unsigned int attribCount, unsigned int *handles)
{
	// opposite of 'glGen' but for several things at once
	// frees GPU-side buffer allocation (for each VBO in the list)
	if ((attribCount > 0) && (attribCount <= ATTRIB_COUNT) && handles)
	{
		// EXAMPLE: 
		//	positions -> NULL (no more data)
		//	normals ---> NULL
		//	texcoords -> NULL
		glDeleteBuffers(attribCount, handles);
	}
}



//-----------------------------------------------------------------------------
// PART 2A: create interleaved VBO
unsigned int demo::createInterleavedVBO(unsigned int numVerts, unsigned int attribCount, 
	const float **data)
{
	const unsigned int sizeoffloat = sizeof(float);
	const unsigned int floatsPerAttrib = 3;	// unsafe assumption

	// validate
	if ((numVerts > 0) && (attribCount > 0) && (attribCount <= ATTRIB_COUNT) && data)
	{
		// ALGORITHM: 
		//	- generate ONE VBO to hold ALL DATA for ALL ATTRIBUTES
		//	- bind VBO for changes
		//	- generate CPU-side array to hold the data before shipping to GPU
		//	- for each attribute...
		//		- for each element in this attribute...
		//			- insert into our giant array
		//			***e.g. if element is a vector, insert 2/3/4 items (we are assuming 3)
		//	- stuff GPU-side VBO with raw data in our array!
		//	- disable VBO usage
		//	- free our CPU-side memory allocation


		// create one buffer and bind it
		unsigned int handle;
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);


		// allocate some memory to create interleaved array
		// ****THE AMOUNT OF SPACE CHANGES IF THE SIZES 
		//	WHEN ATTRIBUTES ARE DIFFERENT!!!!
		// this example assumes each attribute has exactly 3 elements 
		//	and all are floats: 
		// num floats = numElementsPerVertex * numVerts * numAttribs
		unsigned int totalFloats = floatsPerAttrib * numVerts * attribCount;
		float *interleavedData = new float[totalFloats];


		// we'll also need to know the gap between 
		//	the start of each vertex and the next: 
		// how many elements are in a single vertex in total?
		// ****ASSUMING EACH ATTRIBUTE HAS 3 ELEMENTS; 
		//	THIS MAY NOT ALWAYS BE TRUE!!!
		unsigned int totalElemsPerVertex = attribCount * 3;


		// iterate through attributes
		for (unsigned int i = 0, attribStart = 0; 
				i < attribCount; 
				++i, attribStart += floatsPerAttrib)
		{
			// the data array corresponding to this attribute
			const float *dataPtr = data[i];

			// now, for each vertex, add the data to the main array
			for (unsigned int j = 0, 
				rawDataIndex = 0, interleavedIndex = attribStart; 
				j < numVerts; 
				++j, interleavedIndex += (totalElemsPerVertex - floatsPerAttrib))
			{
				// copy 3 elements
				interleavedData[interleavedIndex++] = dataPtr[rawDataIndex++];
				interleavedData[interleavedIndex++] = dataPtr[rawDataIndex++];
				interleavedData[interleavedIndex++] = dataPtr[rawDataIndex++];
			}
		}


		// here's what just happened
		// AGAIN these are not definitely the attributes in-use, but we'll say 
		//	we are using positions, normals, texcoords: 
		//	positions(cpu) = |pos0|pos1|pos2|pos3|...|
		//	normals(cpu)   = |nrm0|nrm1|nrm2|nrm3|...|
		//	texcoords(cpu) = |tex0|tex1|tex2|tex3|...|
		// 
		// take this data and interleave it so attributes with the same number
		//	are side-by-side: 
		//	interleavedData(cpu) = |pos0|nrm0|tex0|pos1|nrm1|tex1|pos2|nrm2|tex2|pos3|nrm3|tex3|...|...|...|...|


		// ship to GPU
		// total memory = sizeof(float) * num floats
		glBufferData(GL_ARRAY_BUFFER, (sizeoffloat * totalFloats), 
			interleavedData, GL_STATIC_DRAW);
		// this copies our CPU interleaved array to a VBO that lives on the GPU
		//	interleavedData(cpu) -> | VBO |


		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// delete interleaved data; we don't need it any more because
		//	it lives on the GPU now
		delete[] interleavedData;


		// done
		return handle;
	}

	// fail
	return 0;
}

void demo::drawInterleavedVBO(unsigned int numVerts, unsigned int attribCount, 
	unsigned int primType, unsigned int handle, const unsigned int *attribIndex)
{
	const unsigned int sizeoffloat = sizeof(float);
	const unsigned int floatsPerAttrib = 3;

	if ((numVerts > 0) && (attribCount > 0) && (attribCount <= ATTRIB_COUNT) && 
		handle && attribIndex)
	{
		// ALGORITHM: 
		//	- bind our ONE VBO
		//	- for each attribute...
		//		- enable the attribute
		//		- tell renderer where the date for THIS attribute begins in our ONE VBO
		//		***it may not be right at the beginning any more!
		//	- DRAW
		//	- for each attribute...
		//		- turn off attribute
		//	- stop using VBOs

		// IMPORTANT: DATA ALREADY LIVES ON GPU, NO DATA TRANSFER HERE!!!


		// MINOR DIFF here: only one handle input!
		glBindBuffer(GL_ARRAY_BUFFER, handle);

		for (unsigned int i = 0; i < attribCount; ++i)
		{
			glEnableVertexAttribArray(attribIndex[i]);

			// similar to before, but now we have STRIDE: 
			//	the size of ONE WHOLE VERTEX IN BYTES
			// stride = num attribs * num elements per attrib * size of element
			// also need to offset the start of each attrib in the buffer itself: 
			// attrib start = attrib index * num elements per attrib * size of element
			glVertexAttribPointer(attribIndex[i], floatsPerAttrib, 
				GL_FLOAT, GL_FALSE,
				(sizeoffloat * floatsPerAttrib * attribCount), 
				(char*)((sizeoffloat * floatsPerAttrib) * i));
		}


		// draw!!!
		glDrawArrays(primType, 0, numVerts);

		// disable everything when we are done with it
		for (unsigned int i = 0; i < attribCount; ++i)
			glDisableVertexAttribArray(attribIndex[i]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void demo::deleteBufferObject(unsigned int handle)
{
	if (handle)
	{
		// ****this applies to single VBOs and IBOs, *NOT VAOs*
		glDeleteBuffers(1, &handle);
	}
}



//-----------------------------------------------------------------------------
// PART 2B: tie interleaved VBO to VAO and draw VAO
unsigned int demo::createVAO(unsigned int numVerts, unsigned int attribCount, 
	const float **data, const unsigned int *attribIndex, unsigned int *vboHandleOut)
{
	const unsigned int sizeoffloat = sizeof(float);
	const unsigned int floatsPerAttrib = 3;

	if (vboHandleOut)
	{
		// create interleaved VBO first
		unsigned int vboHandle = createInterleavedVBO(numVerts, attribCount, data);
		*vboHandleOut = vboHandle;

		if (vboHandle)
		{
			// instead of waiting until draw-time to figure out where 
			//	the data is, we do it now... using a VAO: state machine!
			// ALGORITHM: 
			//	- create Vertex Array Object: NOT A VBO (already have one)!!!
			//	- bind VAO \
			//	- bind VBO / now they are associated
			//	- for each attribute...
			//		- turn on attribute
			//		- tell VAO where data begins within CURRENTLY BOUND VBO
			//	- disable VAO FIRST
			//	- disable VBO
			//	***DO NOT TURN OFF THE LIGHTS or VAO will forget they were on!


			// create and bind vao
			unsigned int vaoHandle;
			glGenVertexArrays(1, &vaoHandle);
			glBindVertexArray(vaoHandle);

			// bind vbo that we want to manage
			glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

			// set states that we have been setting in DRAW up until now
			// (exactly the same code as in previous function)
			for (unsigned int i = 0; i < attribCount; ++i)
			{
				glEnableVertexAttribArray(attribIndex[i]);
				glVertexAttribPointer(attribIndex[i], floatsPerAttrib,
					GL_FLOAT, GL_FALSE,
					(sizeoffloat * floatsPerAttrib * attribCount),
					(char*)((sizeoffloat * floatsPerAttrib) * i));
			}

			// DISABLE VAO BEFORE VBO!!!!
			// otherwise the vao will reference nothing!
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// done
			return vaoHandle;
		}
	}

	// fail
	return 0;
}

void demo::drawVAO(unsigned int numVerts, unsigned int primType, unsigned int vaoHandle)
{
	// ALGORITHM: 
	//	- bind VAO (master switch)
	//	- DRAW
	//	- unbind

	// the VAO is already configured, so just bind it and call draw!
	if (numVerts && vaoHandle)
	{
		// bind VAO (attributes are already enabled!)
		glBindVertexArray(vaoHandle);

		// DRAW
		glDrawArrays(primType, 0, numVerts);

		// stop using VAOs
		glBindVertexArray(0);
	}
}

void demo::deleteVAO(unsigned int handle)
{
	// delete state machine... DOES NOT DELETE VBO, we have to do that separately
	if (handle)
	{
		glDeleteVertexArrays(1, &handle);
	}
}



//-----------------------------------------------------------------------------
// PART 2C: create VAO with IBO
unsigned int demo::createIndexedVAO(unsigned int numVerts, unsigned int numIndices, 
	unsigned int attribCount, const float **data, const unsigned int *attribIndex, 
	const unsigned int *indexData, unsigned int *vboHandleOut, unsigned int *iboHandleOut)
{
	// ALGORITHM: 
	//	- create VAO and VBO using existing function that already does just that!!!
	//	- bind VAO we just created so we can continue configuring it
	//	- create new BUFFER OBJECT (not a VBO this time... remember, context is everything!)
	//	- bind buffer to IBO/EBO TARGET (NOT A VBO!!!)
	//	- stuff new buffer (IBO) with index data (NOT ATTRIBUTE DATA)
	//	- turn off VAO first (so we don't forget we were using IBO)
	//	- turn off IBO


	// first create the VAO using the above function
	// ****(we have a process, why not use it?)
	unsigned int vaoHandle = createVAO(numVerts, attribCount, data, attribIndex, vboHandleOut);
	
	// if we have all we need to do this, proceed with ibo
	// otherwise just return vao handle
	if ((numIndices > 0) && vaoHandle && indexData && iboHandleOut)
	{



	}

	// exit with vao handle (could be success or fail)
	return vaoHandle;
}

void demo::drawIndexedVAO(unsigned int numIndices, unsigned int primType, 
	unsigned int indexType, unsigned int vaoHandle)
{
	// ALGORITHM (same as drawing regular VAO but we need to use a different draw call...): 
	//	- bind VAO (master switch)
	//	- DRAW
	//	- unbind

	if (numIndices && vaoHandle)
	{



	}
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// PART 3A: shaders
unsigned int demo::createShaderfromSource(const char *src, unsigned int shaderType)
{
	// validate pointer (make sure it is not null)
	if (src)
	{
		// ALGORITHM: 
		//	- create shader object on GPU
		//	- send shader source to GPU
		//	- compile source
		//	- if compile succeeds, return shader handle
		//	- else, print error message

		// create shader internally and check if successful
		unsigned int handle = glCreateShader(shaderType);
		if (handle)
		{
			int status;

			// attach sources, compile
			glShaderSource(handle, 1, &src, 0);
			glCompileShader(handle);
			glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
			if (status)
			{
				// finished
				return handle;
			}

			// compilation failed
			// print log
			int logLength;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				char *log = (char *)malloc(logLength);
				glGetShaderInfoLog(handle, logLength, &logLength, log);
				printf("\n Shader compilation failed: \n%s", log);
				free(log);
			}

			// reset shader and exit 
			glDeleteShader(handle);
		}
	}

	// fail
	return 0;
}

void demo::deleteShader(unsigned int shaderHandle)
{
	// check if handle is not zero
	if (shaderHandle)
	{
		// delete shader object
		// also safe to do if the shader is already "installed" in the pipeline (see below)
		glDeleteShader(shaderHandle);
	}
}

unsigned int demo::createProgram()
{
	// just return a handle to a PROGRAM OBJECT (not the same as a shader)
	// think of this as a pipeline for attribute data to flow through 
	// by default, OpenGL uses fixed-function shaders... which are gross
	return glCreateProgram();
}

int demo::attachShaderToProgram(unsigned int programHandle, unsigned int shaderHandle)
{
	// like building our own custom pipeline: 
	//			   ________                            ________
	// attribs -> |________|FIXEDTCS|FIXEDTES|FIXED GS|________| -> frame
	//				^ ^	^								^ ^	^
	// EXAMPLE: attaching a vertex or fragment shader: 
	//				^ ^ ^								^ ^	^
	//	install vertex shader here			...or fragment shader here
	//				^ ^ ^								^ ^	^
	//			  | NEW VS |						  | NEW FS |
	//	

	// check if handles are not zero
	if (programHandle && shaderHandle)
	{
		// attach shader to program object
		glAttachShader(programHandle, shaderHandle);

		// yep, that's all
		return 1;
	}

	// fail
	return 0;
}

int demo::linkProgram(unsigned int programHandle)
{
	// check if handle is not zero
	if (programHandle)
	{
		// ALGORITHM: 
		//	- link program (consolidate pipeline)
		//	- if link succeeds, return some indication of success
		//	- else, print error message
		//	***DO NOT DELETE PROGRAM!!! we may have a contingency plan!

		int status;
		glLinkProgram(programHandle);
		glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
		if (status)
		{
			// finished
			return 1;
		}

		// link failed
		// print log
		int logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char *log = (char *)malloc(logLength);
			glGetProgramInfoLog(programHandle, logLength, &logLength, log);
			printf("\n PROGRAM (%u): Link failed: \n%s", programHandle, log);
			free(log);
		}
	}

	// fail
	return 0;
}

int demo::validateProgram(unsigned int programHandle)
{
	// OPTIONAL STEP: validate program
	// basically a second link step, checks if it's good for your graphics card

	// check if handle is not zero
	if (programHandle)
	{
		// ALGORITHM: 
		//	- link program (consolidate pipeline)
		//	- if link succeeds, return some indication of success
		//	- else, print error message
		//	***DO NOT DELETE PROGRAM!!! we may have a contingency plan!

		int status;
		glValidateProgram(programHandle);
		glGetProgramiv(programHandle, GL_VALIDATE_STATUS, &status);
		if (status)
		{
			// finished
			return 1;
		}

		// validation failed
		// print log
		int logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char *log = (char *)malloc(logLength);
			glGetProgramInfoLog(programHandle, logLength, &logLength, log);
			printf("\n PROGRAM (%u): Validate failed: \n%s \n", programHandle, log);
			free(log);
		}
	}

	// fail
	return 0;
}

void demo::activateProgram(unsigned int programHandle)
{
	// don't need to validate this one... dual purpose function :)
	// turn on pipeline
	// instead of data flowing through fixed-function pipeline (old)...
	// attribs -> |FIXED VS|FIXEDTCS|FIXEDTES|FIXED GS|FIXED FS| -> frame is super gross and lame
	// 
	//	...we want to use our own PROGRAMMABLE pipeline (new and whatever we want to do... the possibilities are ENDLESS!)
	// attribs -> | OUR VS | OUR TCS| OUR TES| OUR GS | OUR FS | -> frame is F*CKING AWESOME.  AND SO IS YOUR PROF.
	
	glUseProgram(programHandle);
}

void demo::deleteProgram(unsigned int programHandle)
{
	// check if handle is not zero
	if (programHandle)
	{
		// delete our custom pipeline
		glDeleteProgram(programHandle);
	}
}



//-----------------------------------------------------------------------------

// file loading
demo::FileInfo demo::loadFile(const char *path)
{
	// default result is null
	FileInfo result = { 0, 0 };

	// validate path
	if (path)
	{
		// open file
		FILE *fp = fopen(path, "rb");
		if (fp)
		{
			// get character count
			fseek(fp, 0, SEEK_END);
			unsigned int newCount = (unsigned)ftell(fp);
			if (newCount)
			{
				// allocate the character string and copy data
				char *newContents = (char *)malloc(newCount + 1);

				// read from beginning and close file
				rewind(fp);
				newCount = fread(newContents, 1, newCount, fp);
				*(newContents + newCount) = 0;
				fclose(fp);

				// release previous data
				result.str = newContents;
				result.len = newCount;
			}
		}
	}

	// exit pass/fail
	return result;
}

int demo::unloadFile(FileInfo *fileInfo)
{
	// check for actual data
	if (fileInfo->str && fileInfo->len)
	{
		free(fileInfo->str);
		fileInfo->str = 0;
		fileInfo->len = 0;

		// done
		return 1;
	}

	// fail
	return 0;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
