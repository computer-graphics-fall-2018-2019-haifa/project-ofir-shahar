#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "Camera.h"


#define PI 3.14159265358
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define RED glm::vec3(1, 0 , 0) 
//#define INTERPOLATE(z1,z2,alpha) z1*alpha + (1-alpha)*z2 

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

void Renderer::fillTriangle(Face &face, glm::vec3 color) {}

void Renderer::drawBetween2Line(Edge & e1, Edge & e2)
{
	int  dy;
	float ratio = 0.0f;
	float edge1ydiff = (float)((e1.getP2().y - e1.getP1().y));
	float edge2ydiff = (float)((e2.getP2().y - e2.getP1().y));

	//check for zero height
	if (edge1ydiff == 0 || edge2ydiff == 0) return;

	float edge1xdiff = (float)((e1.getP1().x - e1.getP2().x));
	float edge2xdiff = (float)((e2.getP1().x - e2.getP2().x));

	//calc factors
	float alpha_1 = (float)((e2.getP1().y - e1.getP1().y) / edge1ydiff);
	float alpha_2 = 0;
	float factorStep1 = 1.0f / edge1ydiff;
	float factorStep2 = 1.0f / edge2ydiff;
	for (int y = e2.getP1().y, dy = 0; y < e2.getP2().y; y++, dy++)
	{
		ratio = (float)(edge2xdiff / edge2ydiff);
		int ex1 = e1.getP1().x - (int)(edge1xdiff * alpha_1);
		int ex2 = e2.getP1().x - (int)( (ratio) * dy );

		scanLine(ex1, ex2, y);

		alpha_1 += factorStep1;
	}

}


void Renderer::scanLine(int &e1, int &e2, int &y)
{
	int x1 = e1, x2 = e2;
	float factor = 0.0f;
	float factorStep = 0.0f;
	int xdiff;

	if (e1 > e2)
	{
		x1 = e2;
		x2 = e1;
	}
	
	if ((xdiff = x2 - x1) == 0) return;

	factorStep = 1.0f / (float)xdiff;

	for (int x = x1; x < x2; x++)
	{
		putPixel(x + this->viewportWidth / 2, y + this->viewportHeight / 2, GREEN);
		factor += factorStep; 
	}
}


void Renderer::fillTriangle(std::vector<glm::vec3> points, glm::vec3 color)
{
	float alpha_1 = 0, alpha_2 = 0, alpha_3 = 0;
	float edge1ydiff, edge2ydiff, edge3ydiff;
	glm::vec2 z, z1, z2, z3, z4, z5;
	glm::vec2 x_left, y_top, y_down;
		
	std::sort(points.begin(), points.end(), sort_dec_y); 
	z1 = points.at(0);
	z2 = points.at(1);
	z3 = points.at(2);
	
	Edge longEdge(z1, z3, GREEN);
	Edge shortEdge1(z1, z2, GREEN);
	Edge shortEdge2(z2, z3, GREEN);

	drawBetween2Line(longEdge, shortEdge1);
	drawBetween2Line(longEdge, shortEdge2);


}

void Renderer::fillTriangle(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 color) {}

glm::vec4 Renderer::transform(std::shared_ptr<MeshModel> model, glm::vec3 v)
{
	glm::vec4 result = glm::vec4(v, 0);
	glm::mat4 localTransform = (*model).GetLocalTransform();
	glm::mat4 scaleTransform = (*model).GetScaleTransform();
	glm::mat4 translateTransform = (*model).getTranslationTransform();
	glm::mat4 rotationTransform = (*model).GetRotationTransform();
	glm::mat4 worldTransform = (*model).GetWorldTransformation();
	glm::mat4 worldTranslate = (*model).GetWorldTranslate();
	glm::mat4 worldRotate = (*model).GetWorldRotation();

	//glm::vec4 newVertex = glm::vec4((*vertex).getPoint().x, (*vertex).getPoint().y, (*vertex).getPoint().z, 1);
	//glm::vec4 newVertex = glm::vec4(v, 1);
	/*for (unsigned int index = 0; index < model->getVertexs().size(); index++)
	{
		model->getVertexs().at(index).setPoint( scaleTransform * result ); 
		model->getVertexs().at(index).setPoint( scaleTransform * result );
		result.w = 1;
		model->getVertexs().at(index).setPoint( rotationTransform * result );
		result.w = 1;
		model->getVertexs().at(index).setPoint( translateTransform * result );
		 new set WORLD transformations.
		result.w = 1;
		model->getVertexs().at(index).setPoint( worldRotate * result );
		result.w = 1;
		model->getVertexs().at(index).setPoint( worldTranslate * result );

		model->getVertexs().at(index).setPoint( currentCamera.getViewTransformation() * result );
		result.w = 0;
		model->getVertexs().at(index).setPoint( currentCamera.getProjectionTformation() * result );
	}*/
	// set LOCAL tranformations first.
	result = scaleTransform * result;
	//result.w = 1;
	result = rotationTransform * result;
	//result.w = 1;
	result = translateTransform * result;
	//// new set WORLD transformations.
	//result.w = 1;
	result = worldRotate * result;
	//result.w = 1;
	result = worldTranslate * result;
	//
	result = currentCamera.getViewTransformation() * result;
	//result.w = 0;
	result = currentCamera.getProjectionTformation() * result;

	return result;
}

void Renderer::drawPolygon(std::shared_ptr<MeshModel> model, Face &face)
{
	//face vertices for fill triangles purpose
	
	glm::vec3 first, second, third;
	std::vector<Vertex> vertexs = model->getVertexs(); 

	//get the indices of the vertices for each face
	std::vector<int> indices = (face).GetVertices();
	//iterate over the indices
	for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) {
		//here we draw a line between two successive pointes by the indexes from the indices vector
		//if we are at the end of the indices vector we connect the vertex with this index
		//with the vertex with the index from the start of the vector
		if ((vindex + 1) == indices.end()) {
			//DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0)-1), glm::vec3(0, 0, 0),true);
			first = vertexs.at(*(vindex) - 1).getPoint();
			second = vertexs.at(indices.at(0) - 1).getPoint();
			
			//DrawLine(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(indices.at(0) - 1).getPoint(), glm::vec3(0, 0, 0), true);
		}
		else { //draw a line between the two vertices by their indices from the vector "indices"
			//DrawLine(vertices.at(*(vindex)-1), vertices.at(*(vindex + 1)-1), glm::vec3(0, 0, 0),true);
			//DrawLine(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(*(vindex + 1) - 1).getPoint(), glm::vec3(0, 0, 0), true);
			first = vertexs.at(*(vindex) - 1).getPoint();
			second = vertexs.at(*(vindex + 1) - 1).getPoint();
		}
		/*std::cout << "first x:" << first.x << " y:" << first.y << " z:" << first.z << std::endl;
		std::cout << "second x:" << second.x << " y:" << second.y << " z:" << second.z << std::endl << std::endl;*/
		DrawLine(first, second, glm::vec3(0, 0, 0), true);

		first = vertexs.at((face).GetVertexIndex(0) - 1).getPoint();
		second = vertexs.at((face).GetVertexIndex(1) - 1).getPoint();
		third = vertexs.at((face).GetVertexIndex(2) - 1).getPoint();
		glm::vec3 centerv = glm::vec3((first.x + second.x + third.x) / 3, (first.y + second.y + third.y) / 3, (first.z + second.z + third.z) / 3);

		//normals
		glm::vec3 normal = glm::normalize(glm::cross(first - second, first - third));
		normal.x *= -50; normal.y *= -50; normal.z *= -50;
		if (this->toDrawFaceNormals && (*model).getIsCurrentModel())
		{
			putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y, RED); 
			putPixel(viewportWidth / 2 + centerv.x + 1, viewportHeight / 2 + centerv.y, RED);
			putPixel(viewportWidth / 2 + centerv.x - 1, viewportHeight / 2 + centerv.y, RED);
			putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y + 1, RED);
			putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y - 1, RED);
			DrawLine(centerv, glm::vec3(centerv.x + normal.x, centerv.y + normal.y, -(centerv.z + normal.z)), RED, true);
		}
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
		/*glm::mat4 localTransform = model->GetLocalTransform();
		glm::mat4 scaleTransform = model->GetScaleTransform();
		glm::mat4 translateTransform = model->getTranslationTransform();
		glm::mat4 rotationTransform = model->GetRotationTransform();
		glm::mat4 worldTransform = model->GetWorldTransformation();
		glm::mat4 worldTranslate = model->GetWorldTranslate();
		glm::mat4 worldRotate = model->GetWorldRotation();*/
		glm::vec3 red_color(1, 0, 0);
		glm::vec3 green_color(0, 1, 0);
		glm::vec3 black_color(0, 0, 0);
		glm::vec4 normal_vertex;
		//get the faces from the pointer to the model
		std::vector<Face> faces = (*model).GetFaces();
		//get the vertices from the pointer to the model
		std::vector<Vertex> vertexs = (*model).getVertexs();
		std::vector<Vertex> *vertexsptr = (*model).getVertexsptr();
		//TODO: connect vertexptr to vertex
		//std::vector<glm::vec3> vertices = (*model).getVertexs();
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
			c.cPoints[i] = transform(model, c.cPoints[i]);
			/*c.cPoints[i] = scaleTransform * c.cPoints[i];
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
			c.cPoints[i].w = 0; */
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
		unsigned int counter = 0;
		for (std::vector<Vertex>::iterator vertex = vertexs.begin(); vertex != vertexs.end(); vertex++) {

			glm::vec4 temp = glm::vec4(vertex->getPoint().x, vertex->getPoint().y, vertex->getPoint().z, 0); 
			//glm::vec4 temp;// = transform(model, (*vertex).getPoint());
			/*vertex->setPoint(glm::vec3(temp.x, temp.y, temp.z)); */
			//glm::vec4 newVertex = glm::vec4((*vertex).getPoint().x, (*vertex).getPoint().y, (*vertex).getPoint().z, 1);

			////debug: erase later
			//std::cout << "before render transform" << std::endl;
			//std::cout << "vertex x:" << newVertex.x << " y:" << newVertex.y << " z:" << newVertex.z << std::endl;

			//// set LOCAL tranformations first.
			//newVertex = scaleTransform * newVertex;
			//newVertex.w = 1;
			//newVertex = rotationTransform * newVertex;
			//newVertex.w = 1;
			//newVertex = translateTransform * newVertex;
			//// new set WORLD transformations.
			//newVertex.w = 1;
			//newVertex = worldRotate * newVertex;
			//newVertex.w = 1;
			//newVertex = worldTranslate * newVertex;
			//newVertex.w = 0;

			//newVertex = currentCamera.getViewTransformation() * newVertex;
			//newVertex.w = 0;
			//newVertex = currentCamera.getProjectionTformation() * newVertex;

			//std::cout << "after render transform" << std::endl;
			//std::cout << "vertex x:" << newVertex.x << " y:" << newVertex.y << " z:" << newVertex.z << std::endl << std::endl;


			//std::cout << "before transform transform" << std::endl;
			//std::cout << "temp x:" << temp.x << " y:" << temp.y << " z:" << temp.z << std::endl;
			//
			//
			

			//std::cout << "after transform transform" << std::endl;
			temp = transform(model, temp);
			//std::cout << "temp x:" << temp.x << " y:" << temp.y << " z:" << temp.z << std::endl << std::endl;
			//normals per face

			//set new cube faces
			/*if (c.back >= newVertex.z) c.back = newVertex.z;
			if (c.front < newVertex.z)  c.front = newVertex.z;

			if (c.right <= newVertex.x) c.right = newVertex.x;
			if (c.left > newVertex.x) c.left = newVertex.x;

			if (c.bottom >= newVertex.y) c.top = newVertex.y;
			if (c.top < newVertex.y) c.top = newVertex.y;
			*/
			//vertex->setPoint(glm::vec3(temp.x, temp.y, temp.z));
			//(*vertexsptr).at(counter++).setPoint(glm::vec3(temp.x, temp.y, temp.z)); 
			
			
			//(*vertexsptr).
			vertex->setPoint(glm::vec3(temp.x, temp.y, temp.z));
		}
		//std::cout << "after transform transform" << std::endl;
		//model->setVertexs(vertexs); 
		//(*model).setVertexs(vertexs); 
		// ############## END OF IMPORTANT CODE #################
		// ######################################################
	

		//iterate over the faces vector of the model
		for (std::vector<Face>::iterator face = faces.begin(); face != faces.end(); face++) 
		{

			//drawPolygon(model, *face); 
			//face vertices for fill triangles purpose
			glm::vec3 first, second, third;

			//get the indices of the vertices for each face
			std::vector<int> indices = (*face).GetVertices();
			//iterate over the indices
			for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) {
				//here we draw a line between two successive pointes by the indexes from the indices vector
				//if we are at the end of the indices vector we connect the vertex with this index
				//with the vertex with the index from the start of the vector
				if ((vindex+1) == indices.end()) {
					//DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0)-1), glm::vec3(0, 0, 0),true);
					DrawLine(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(indices.at(0) - 1).getPoint(), glm::vec3(0, 0, 0), true);
				} 
				else //draw a line between the two vertices by their indices from the vector "indices"
					//DrawLine(vertices.at(*(vindex)-1), vertices.at(*(vindex + 1)-1), glm::vec3(0, 0, 0),true);
					DrawLine(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(*(vindex + 1) - 1).getPoint(), glm::vec3(0, 0, 0), true);

				
				first = vertexs.at((*face).GetVertexIndex(0) - 1).getPoint();
				second = vertexs.at((*face).GetVertexIndex(1) - 1).getPoint();
				third = vertexs.at((*face).GetVertexIndex(2) - 1).getPoint();
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
			std::vector<glm::vec3> points;
			
			points.push_back(first); 
			points.push_back(second);
			points.push_back(third);
			
			fillTriangle(points, green_color); 
		}


	}
	/*std::vector<glm::vec3> points;
	points.push_back(glm::vec3(200, 100, 300));
	points.push_back(glm::vec3(300, 200, 300));
	points.push_back(glm::vec3(50, 50, 300));
	fillTriangle(points, GREEN);
	putPixel(this->viewportWidth / 2 + 200, this->viewportHeight / 2 + 100, BLACK);
	putPixel(this->viewportWidth / 2 + 300, this->viewportHeight / 2 + 200, BLACK);
	putPixel(this->viewportWidth / 2 + 50, this->viewportHeight / 2 + 50, BLACK);*/
	
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