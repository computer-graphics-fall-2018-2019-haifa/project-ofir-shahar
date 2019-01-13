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

//ctor
Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr),
	hasModel(false)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
	this->toDrawFaceNormals = false;
	this->toDrawLineNormals = false;
	this->tooDrawaCube = false;
	//setScaleNumber(50);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

bool Renderer::isHasModel() {
	return hasModel;
}
void Renderer::setHasModel() {
	this->hasModel = true;
}
void Renderer::setEyeX(float eyex) {
	glm::vec3 eye;
	//eye = glm::vec3(1280 * sin(PI*eyex / 180), 0, cos(PI*eyex / 180) * 1280);
	eye = glm::vec3(1280 * sin(PI*eyex / 180), 0, cos(PI*eyex / 180) * 1280);
	//glm::vec3 at = glm::vec3(viewportWidth / 2, viewportHeight / 2, 0);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	currentCamera.SetCameraLookAt(eye, at, up);
}
void Renderer::rotateLocalX(float x) {
	this->currentModel->setRotationTransform(x, 1, 1);
}
void Renderer::rotateLocalY(float y) {
	this->currentModel->setRotationTransform(1, y, 1);
}
void Renderer::rotateLocalZ(float z) {
	this->currentModel->setRotationTransform(1, 1, z);
}

void Renderer::rotateWorldX(float x) {
	this->currentModel->setWorldRotation(x, 1, 1);
}
void Renderer::rotateWorldY(float y) {
	this->currentModel->setWorldRotation(1, y, 1);
}
void Renderer::rotateWorldZ(float z) {
	this->currentModel->setWorldRotation(1, 1, z);
}
void Renderer::setWorldTranslation(float x, float y, float z) {
	if (this->currentModel != NULL)
		this->currentModel->setWorldTranslation(x, y, z);
}

void Renderer::translate(float xt, float yt, float zt) {
	if (this->currentModel != NULL)
		this->currentModel->setTranslationTransform(xt, yt, zt);
}
void Renderer::setPerspective(float f, float ar, float n, float fa) {
	currentCamera.SetPerspectiveProjection(f, ar, n, fa);
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
	//this->scaleNumber = f;
	if (this->currentModel!=NULL)
		this->currentModel->setScaleTransform(f,f,f);
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
		x1 = viewportWidth/2 + (p1.x);
		x2 = viewportWidth/2 + (p2.x);
		y1 = viewportHeight/2 + (p1.y);
		y2 = viewportHeight/2 + (p2.y);
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

void Renderer::drawCube()
{
	Cube c = this->currentModel->getCube(); 
	//draw the cube
	DrawLine(c.cPoints[0], c.cPoints[2], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[1], c.cPoints[3], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[0], c.cPoints[1], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[2], c.cPoints[3], glm::vec3(1, 0, 0), true);

	DrawLine(c.cPoints[4], c.cPoints[6], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[5], c.cPoints[7], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[4], c.cPoints[5], glm::vec3(1, 0, 0), true);
	DrawLine(c.cPoints[6], c.cPoints[7], glm::vec3(1, 0, 0), true);

	DrawLine(c.cPoints[0], c.cPoints[4], glm::vec3(0, 0, 1), true);
	DrawLine(c.cPoints[1], c.cPoints[5], glm::vec3(0, 0, 1), true);
	DrawLine(c.cPoints[2], c.cPoints[6], glm::vec3(0, 0, 1), true);
	DrawLine(c.cPoints[3], c.cPoints[7], glm::vec3(0, 0, 1), true);
}

void Renderer::fillTriangle(Face &face, glm::vec3 color)
{
	glm::vec3 high, low;
	//get the vertices indexes from the face
	std::vector<int> indices = face.GetVertices();
	
	//iterate thru all 
	for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) {

		if ((vindex + 1) == indices.end()) {
			
		}
	}
}

void Renderer::fillTriangle(std::vector<glm::vec3> face, glm::vec3 color)
{
	glm::vec3 max = glm::vec3(INT32_MIN); 
	glm::vec3 min = glm::vec3(INT32_MAX);
	glm::vec3 right = glm::vec3(INT32_MIN);
	glm::vec3 left = glm::vec3(INT32_MAX);

	//find the highest and lowest verices of the face
	for (std::vector<glm::vec3>::iterator vindex = face.begin(); vindex != face.end(); vindex++) {
		if ((*vindex).y <= min.y) min = (*vindex);
		if ((*vindex).y >= max.y) max = (*vindex);
		if ((*vindex).x >= right.x) right = (*vindex);
		if ((*vindex).x <= left.x) left = (*vindex);
	}


}


void Renderer::Render(const Scene& scene)
{
	
	glm::vec3 p1, p2, color;
	p1.x = 0.0;
	p1.y = (int)(viewportHeight/2);
	p1.z = 0.0;
	p2.x = (int)viewportWidth;
	p2.y = (int)(viewportHeight/2);
	p2.z = 0.0;
	color.x = 0.0;
	color.y = 0.0;
	color.z = 0.0;

	
	
	//we get the model list from the scene object that was passed to Render function
	//models = vector of pointers (pointing to a MeshModel) representing this list.
	std::vector<std::shared_ptr<MeshModel>> models = scene.getModels();
	std::shared_ptr<MeshModel> model; 
	
	//we iterate over models vector with an iterator
	for (std::vector<std::shared_ptr<MeshModel>>::iterator it = models.begin(); it != models.end(); it++) {
		//the iterator is pointing to a shared_ptr that points to our MeshModel. 
		model = (*it);

		//this->currentModel = &(*model);
		Cube c = model->getCube();
		glm::mat4 localTransform = model->GetLocalTransform();
		glm::mat4 scaleTransform = model->GetScaleTransform();
		glm::mat4 translateTransform = model->getTranslationTransform();
		glm::mat4 rotationTransform = model->GetRotationTransform();
		glm::mat4 worldTransform = model->GetWorldTransformation();
		glm::mat4 worldTranslate = model->GetWorldTranslate();
		glm::mat4 worldRotate = model->GetWorldRotation();
		glm::vec3 red_color(1, 0, 0);
		glm::vec3 green_color(0, 1, 0);
		glm::vec3 black_color(0, 0, 0);
		glm::vec4 normal_vertex;
		//get the faces from the pointer to the model
		std::vector<Face> faces = (*model).GetFaces();
		//get the vertices from the pointer to the model
		std::vector<glm::vec3> vertices = (*model).GetVertices();
		std::vector<glm::vec3> normals = (*model).GetNormals(); 
		typedef std::vector<glm::vec3>::iterator normal_it;
		typedef std::vector<Face>::iterator faces_it; 
		typedef std::vector<glm::vec4>::iterator center_it;


		for (std::vector<std::string>::iterator name_it = this->ExcludeModels.begin(); name_it != this->ExcludeModels.end(); name_it++)
		{
			//if (name_it::compare(this->currentModel->GetModelName())) break;
		}

		//adjust cube coordinates
		for (int i = 0; i < 8; i++)
		{
			c.cPoints[i] = scaleTransform * c.cPoints[i];
			c.cPoints[i].w = 1;

			c.cPoints[i] = rotationTransform * c.cPoints[i];
			c.cPoints[i].w = 1;

			c.cPoints[i] = translateTransform * c.cPoints[i];
			c.cPoints[i].w = 1;

			c.cPoints[i] = worldRotate * c.cPoints[i];
			c.cPoints[i].w = 1;

			c.cPoints[i] = worldTranslate * c.cPoints[i];
			c.cPoints[i].w = 0;

			c.cPoints[i] = currentCamera.getViewTransformation() * c.cPoints[i];
			c.cPoints[i].w = 0;

			c.cPoints[i] = currentCamera.getProjectionTformation() * c.cPoints[i];
			c.cPoints[i].w = 0; 
		}
		
		//draw the cube
		if (this->tooDrawaCube && (*model).getIsCurrentModel())
		{
			DrawLine(c.cPoints[0], c.cPoints[2], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[1], c.cPoints[3], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[0], c.cPoints[1], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[2], c.cPoints[3], glm::vec3(1, 0, 0), true);

			DrawLine(c.cPoints[4], c.cPoints[6], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[5], c.cPoints[7], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[4], c.cPoints[5], glm::vec3(1, 0, 0), true);
			DrawLine(c.cPoints[6], c.cPoints[7], glm::vec3(1, 0, 0), true);

			DrawLine(c.cPoints[0], c.cPoints[4], glm::vec3(0, 0, 1), true);
			DrawLine(c.cPoints[1], c.cPoints[5], glm::vec3(0, 0, 1), true);
			DrawLine(c.cPoints[2], c.cPoints[6], glm::vec3(0, 0, 1), true);
			DrawLine(c.cPoints[3], c.cPoints[7], glm::vec3(0, 0, 1), true);
		}
		

		//adjust face normals and faces centeroids positions
		if (this->toDrawLineNormals)
		{

		}

		// ############### IMPORTANT CODE HERE ##################
		// in this for loop we iterate over all the vertices of the model 
		// and multiply each vertex by view, projection and viewport transformation.
		// ######################################################
		for (std::vector<glm::vec3>::iterator vertex = vertices.begin(); vertex != vertices.end(); vertex++) {

			glm::vec4 newVertex = glm::vec4((*vertex).x, (*vertex).y, (*vertex).z, 1);
			// set LOCAL tranformations first.
			newVertex = scaleTransform * newVertex;
			newVertex.w = 1;
			newVertex = rotationTransform * newVertex;
			newVertex.w = 1;
			newVertex = translateTransform * newVertex;
			// new set WORLD transformations.
			newVertex.w = 1;
			newVertex = worldRotate * newVertex;
			newVertex.w = 1;
			newVertex = worldTranslate * newVertex;
			//newVertex.w = 0;

			newVertex = currentCamera.getViewTransformation() * newVertex;
			newVertex.w = 0;
			newVertex = currentCamera.getProjectionTformation() * newVertex;
			//normals per face

			//set new cube faces
			if (c.back >= newVertex.z) c.back = newVertex.z; 
			if (c.front < newVertex.z)  c.front = newVertex.z;

			if (c.right <= newVertex.x) c.right = newVertex.x;
			if (c.left > newVertex.x) c.left = newVertex.x;

			if (c.bottom >= newVertex.y) c.top = newVertex.y;
			if (c.top < newVertex.y) c.top = newVertex.y;

			(*vertex) = glm::vec3(newVertex.x, newVertex.y, newVertex.z);
		}
		// ############## END OF IMPORTANT CODE #################
		// ######################################################
	

		//iterate over the faces vector of the model
		for (std::vector<Face>::iterator face = faces.begin(); face != faces.end(); face++) {
			
			//get the indices of the vertices for each face
			std::vector<int> indices = (*face).GetVertices();
			//iterate over the indices
			for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) {
				//here we draw a line between two successive pointes by the indexes from the indices vector
				//if we are at the end of the indices vector we connect the vertex with this index
				//with the vertex with the index from the start of the vector
				if ((vindex+1) == indices.end()) {
					//DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0)-1), glm::vec3(0, 0, 0),true);
					DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0) - 1), glm::vec3(0, 0, 0), true);
				} 
				else //draw a line between the two vertices by their indices from the vector "indices"
					//DrawLine(vertices.at(*(vindex)-1), vertices.at(*(vindex + 1)-1), glm::vec3(0, 0, 0),true);
					DrawLine(vertices.at(*(vindex)-1), vertices.at(*(vindex + 1) - 1), glm::vec3(0, 0, 0), true);

				glm::vec3 first, second, third;
				first = vertices.at((*face).GetVertexIndex(0) - 1);
				second = vertices.at((*face).GetVertexIndex(1) - 1);
				third = vertices.at((*face).GetVertexIndex(2) - 1);
				glm::vec3 centerv = glm::vec3((first.x + second.x + third.x) / 3, (first.y + second.y + third.y) / 3, (first.z + second.z + third.z) / 3);

				//normals
				glm::vec3 normal = glm::normalize(glm::cross(first - second, first - third));
				normal.x *= -50; normal.y *= -50; normal.z *= -50;
				if (this->toDrawFaceNormals && (*model).getIsCurrentModel())
				{
					putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y, red_color);
					putPixel(viewportWidth / 2 + centerv.x + 1, viewportHeight / 2 + centerv.y, red_color);
					putPixel(viewportWidth / 2 + centerv.x - 1, viewportHeight / 2 + centerv.y, red_color);
					putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y + 1, red_color);
					putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y - 1, red_color);
					DrawLine( centerv, glm::vec3(centerv.x + normal.x, centerv.y + normal.y, -(centerv.z + normal.z)), red_color, true);
				}
			}

			fillTriangle(*face, red_color); 
		}
	}

	
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