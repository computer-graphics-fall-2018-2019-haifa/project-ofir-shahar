#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "Camera.h"

#define PI 3.14159265358
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
	setScaleNumber(50);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}
void Renderer::setEyeX(float eyex) {
	glm::vec3 eye;
	eye = glm::vec3(1280 * sin(PI*eyex / 180), 0, cos(PI*eyex / 180) * 1280);
	glm::vec3 at = glm::vec3(viewportWidth / 2, viewportHeight / 2, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	camera.SetCameraLookAt(eye, at, up);
}
void Renderer::rotateLocalX(float x) {
	currentModel->setRotationTransform(x, 1, 1);
}
void Renderer::rotateLocalY(float y) {
	currentModel->setRotationTransform(1, y, 1);
}
void Renderer::rotateLocalZ(float z) {
	currentModel->setRotationTransform(1, 1, z);
}
void Renderer::setFov(float f) {
	camera.SetPerspectiveProjection(f, 1, 1, 10);
}
void Renderer::setProjection(bool p)
{
	this->projection = p;
}
const bool Renderer::getProjection()
{
	return this->projection;
}
void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}
void Renderer::setScaleNumber(float f) {
	this->scaleNumber = f;
}
void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

//Draw a  line between 2 vertices, represented by p1, p2 which are 2 glm:vec3 vectors
//baesd on the Bresenham algorithem
void Renderer::DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale) 
{
	float x1, x2, y1, y2;
	// order the points so one is left and one is right depending on x1 and x2 values.
	if (scale) {
		x1 = viewportWidth/2 + (p1.x * this->scaleNumber);
		x2 = viewportWidth/2 + (p2.x * this->scaleNumber);
		y1 = viewportHeight/2 + (p1.y * this->scaleNumber);
		y2 = viewportHeight/2 + (p2.y * this->scaleNumber);
	}
	else {
		x1 = p1.x;
		x2 = p2.x;
		y1 = p1.y;
		y2 = p2.y;
	}

	float xLeft = x1 >= x2 ? x2 : x1;
	float xRight = x1 >= x2 ? x1 : x2;
	// set the y accordingly
	float yLeft = 0;
	float yRight = 0;
	if (x1 >= x2) {
		yLeft = y2;
		yRight = y1;
	}
	else {
		yLeft = y1;
		yRight = y2;
	}
	float a = (y2 - y1) / (x2 - x1);
	float c = yLeft + a * xLeft;

	if (yLeft == yRight) {
		while (xLeft <= xRight) {
			putPixel((int)xLeft, (int)yLeft, color);
			xLeft = xLeft + 1;
		}
	}
	else if (xLeft == xRight) {
		if (yLeft <= yRight)
			while (yLeft <= yRight) {
				putPixel((int)xLeft, (int)yLeft, color);
				yLeft = yLeft + 1;
			}
		else
			while (yLeft >= yRight) {
				putPixel((int)xLeft, (int)yLeft, color);
				yLeft = yLeft - 1;
			}
	}
	else if (a > 0 && a <= 1) {
		float e = 0 - 2*(xRight - xLeft);
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft + 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color);
			xLeft = xLeft + 1;
			e = e + 2 * (yRight - yLeft);
		}
	}
	else if (a > 1) {
		float e = 0 - 2*(yRight - yLeft);
		while (yLeft <= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e - 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color);
			yLeft = yLeft + 1;
			e = e + 2 * (xRight - xLeft);
		}
	}
	else if (a >= -1 && a < 0) {
		float e = 0 - 2*(xRight - xLeft);
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft - 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color);
			xLeft = xLeft + 1;
			e = e - 2 * (yRight - yLeft);
		}
	}
	else if (a < -1) {
		float e = 2*(yRight - yLeft);
		while (yLeft >= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e + 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color);
			yLeft = yLeft - 1;
			e = e + 2 * (xRight - xLeft);
		}
	}
}



void Renderer::Render(const Scene& scene)
{
	
	glm::vec3 p1, p2, color;
	p1.x = 0.0;
	p1.y = viewportHeight/2;
	p1.z = 0.0;
	p2.x = viewportWidth;
	p2.y = viewportHeight/2;
	p2.z = 0.0;
	color.x = 0.0;
	color.y = 0.0;
	color.z = 0.0;

	// Draw a chess board in the middle of the screen
	/*for (int i = 100; i < viewportWidth - 100; i++)
	{
		for (int j = 100; j < viewportHeight - 100; j++)
		{
			int mod_i = i / 50;
			int mod_j = j / 50;

			int odd = (mod_i + mod_j) % 2;
			if (odd)
			{
				putPixel(i, j, glm::vec3(0, 1, 0));
			}
			else
			{
				putPixel(i, j, glm::vec3(1, 0, 0));
			}
		}
	}*/
	
	//we get the model list from the scene object that was passed to Render function
	//models = vector of pointers (pointing to a MeshModel) representing this list.
	std::vector<std::shared_ptr<MeshModel>> models = scene.getModels();
	std::shared_ptr<MeshModel> model;
	//we iterate over models vector with an iterator
	for (std::vector<std::shared_ptr<MeshModel>>::iterator it = models.begin(); it != models.end(); it++) {
		//the iterator is pointing to a shared_ptr that points to our MeshModel. 
		model = (*it);
		currentModel = &(*model);
		glm::mat4 localTransform = currentModel->GetLocalTransform();
		//get the faces from the pointer to the model
		std::vector<Face> faces = (*model).GetFaces();
		//get the vertices from the pointer to the model
		std::vector<glm::vec3> vertices = (*model).GetVertices();


		// ############### IMPORTANT CODE HERE ##################
		// in this for loop we iterate over all the vertices of the model
		// and multiply each vertex by view, projection and viewport transformation.
		// ######################################################
		for (std::vector<glm::vec3>::iterator vertex = vertices.begin(); vertex != vertices.end(); vertex++) {
			glm::vec4 newVertex = glm::vec4((*vertex).x, (*vertex).y, (*vertex).z, 0);
			//std::cout << "<"<<newVertex.x <<","<<newVertex.y<<","<<newVertex.z<< ">" << std::endl;
			newVertex = (this->projection) ? camera.getOrthographicTransformation() * camera.getViewTransformation()*newVertex : camera.getProjectionTformation() * camera.getViewTransformation()*newVertex;
			newVertex = localTransform * newVertex;
			newVertex = camera.getViewTransformation()*newVertex;
			newVertex = camera.getProjectionTformation()*newVertex;
			/*std::cout << "<" << newVertex.x << "," << newVertex.y << "," << newVertex.z <<">"<< std::endl;
			std::cout << "end here"<<std::endl;*/
			(*vertex) = glm::vec3(newVertex.x, newVertex.y, newVertex.z);
		}
		// ############## END OF IMPORTANT CODE #################
		// ######################################################


		//iterate over the faces vector of the model
		for (std::vector<Face>::iterator faceIndex = faces.begin(); faceIndex != faces.end(); faceIndex++) {
			//get the indices of the vertices for each face
			std::vector<int> indices = (*faceIndex).GetVertices();
			//iterate over the indices
			for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) {
				//here we draw a line between two successive pointes by the indexes from the indices vector
				//if we are at the end of the indices vector we connect the vertex with this index
				//with the vertex with the index from the start of the vector
				if ((vindex+1) == indices.end()) {
					//std::cout << "index at start of face: "<<vertices.at(indices.at(0) - 1).x<< std::endl;
					/*std::cout << "xEnd=" << vertices.at(*(vindex)-1).x << " " << "yEnd=" << vertices.at(*(vindex)-1).y << " "
						<< "xStart=" << vertices.at(indices.at(0)-1).x << " yStart=" << vertices.at(indices.at(0) - 1).y << std::endl;
*/
					DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0)-1), glm::vec3(0, 0, 0),true);
				} else //draw a line between the two vertices by their indices from the vector "indices"
					DrawLine(vertices.at(*(vindex)-1), vertices.at(*(vindex + 1)-1), glm::vec3(0, 0, 0),true);
			}
		}
	}
	/*glm::vec4 p14 = glm::vec4(p1.x, p1.y, p1.z, 0);
	glm::vec4 p24 = glm::vec4(p2.x, p2.y, p2.z, 0);
=======
	/*glm::vec4 p24 = glm::vec4(p2.x, p2.y, p2.z, 0);
>>>>>>> c382393077ff6dace273740e3d655e003e1f6cdb
	p14 = camera.getViewTransformation()*p14;
	p24 = camera.getViewTransformation()*p24;
	p1 = glm::vec3(p14.x, p14.y, p14.z);
	p2 = glm::vec3(p24.x, p24.y, p24.z);*/
	//draw y and x axis at the center of the screen
	DrawLine(p1, p2, glm::vec3(0,0,0),false);
	DrawLine(glm::vec3(viewportWidth / 2, 0, 0), glm::vec3(viewportWidth / 2, viewportHeight, 0), glm::vec3(0, 0, 0),false);
	
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}