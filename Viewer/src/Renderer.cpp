#define _USE_MATH_DEFINES

#include "Renderer.h"


//ctor
Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr),
	hasModel(false),
	viewportHeight(viewportHeight),
	viewportWidth(viewportWidth)
{

	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
	//initViewport();
	this->toDrawFaceNormals = false;
	this->toDrawLineNormals = false;
	this->toDrawVertexNormals = false;
	this->tooDrawaCube = false;
	this->fillTriangles = false;
	this->drawLines = false;
	this->diffusive = 0.5;
	this->ambient = 0.5f;
	this->ambientColor = PURPLE;
	this->ambientIntensity = 1;
	this->worldToCameraTransformation = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(viewportWidth / 2, viewportHeight / 2, 0, 1));
	this->proj = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
	if (zBuffer)
		delete[] zBuffer;
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
	//eye = glm::vec3(1280 * sin(PI*eyex / 180), 0, cos(PI*eyex / 180) * 1280);
	glm::vec3 at = glm::vec3(0, 0, -1);

	//glm::vec3 at = glm::vec3(viewportWidth / 2, viewportHeight / 2, 0);
	//glm::vec3 at = glm::vec3(0, 0, 0);

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
	this->currentCamera.SetPerspectiveProjection(f, ar, n, fa);
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

void Renderer::putPixel(int i, int j, const glm::vec3& color, const float &z)
{
	float depth = 0.0f;
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;

	depth = zBuffer[Z_INDEX(viewportWidth, i, j)]; 
	if (z < depth)
	{
		colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
		colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
		colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
		zBuffer[Z_INDEX(viewportWidth, i, j)] = z;
	}
	
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
	if (zBuffer)
	{
		delete[] zBuffer; 
	}
	zBuffer = new float[viewportWidth * viewportHeight]; 
	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
			zBuffer[Z_INDEX(viewportWidth, x, y)] = INT32_MAX; 
		}
	}
}

float Renderer::zDepth(glm::vec3 p, std::vector<Vertex> points)
{
	float result = 0;
	float z1, z2, z3;
	float d1 = 1.0f, d2 = 1.0f, d3 = 1.0f;
	glm::vec3 baryZ = glm::vec3(0);
	std::vector<glm::vec3> pointsb;

	z1 = points.at(0).getPoint().z;
	z2 = points.at(1).getPoint().z;
	z3 = points.at(2).getPoint().z;

	pointsb.push_back(points.at(0).getPoint());
	pointsb.push_back(points.at(1).getPoint());
	pointsb.push_back(points.at(2).getPoint());
	
	if (glm::all(glm::equal(p, points.at(0).getPoint())) ) result = points.at(0).getPoint().z;
	else if (glm::all(glm::equal(p, points.at(0).getPoint()))) result = points.at(1).getPoint().z;
	else if (glm::all(glm::equal(p, points.at(0).getPoint()))) result = points.at(2).getPoint().z;

	else
	{
		baryZ = baryCentric(pointsb, p); 
	}

	result = z1*baryZ.x + z2*baryZ.y + z3*baryZ.z;
	return result;

}

glm::vec3 Renderer::baryCentric(std::vector<glm::vec3> &polygon, glm::vec3 &point)
{
	float alpha = 0.0f, beta = 0.0f, gama = 0.0f;
	alpha = Fab(polygon.at(1), polygon.at(2), point) / Fab(polygon.at(1), polygon.at(2), glm::vec3(polygon.at(0).x, polygon.at(0).y, 0));
	beta = Fab(polygon.at(2), polygon.at(0), point) / Fab(polygon.at(2), polygon.at(0), glm::vec3(polygon.at(1).x, polygon.at(1).y, 0));
	gama = Fab(polygon.at(0), polygon.at(1), point) / Fab(polygon.at(0), polygon.at(1), glm::vec3(polygon.at(2).x, polygon.at(2).y, 0));
	//gama = 1 - alpha - beta;
	return glm::vec3(alpha, beta, gama);
	
}

float Renderer::Fab(glm::vec3 & a, glm::vec3 & b, glm::vec3 & point)
{
	return (a.y - b.y)*point.x + (b.x - a.x)*point.y + a.x*b.y - a.y*b.x;
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, glm::vec3(1,1,1));
			zBuffer[Z_INDEX(viewportWidth, i, j)] = INT32_MAX;
		}
	}
}

void Renderer::setProj(float & fovy,  float & aspectRatio,  float & _near,  float & _far)
{
	float angle = _CMATH_::tanf(0.5*fovy * PI / 180);
	float nearHeight =  angle*2*_near; 
	//float nearHeight = 2 * near * cmath::tan(0.5*fovy * PI / 180);
	//float nearHeight = 2*near * tan(0.5*fovy * PI / 180);
	float nearWidth = aspectRatio * nearHeight;
	float t = 0.5 * nearHeight;
	float b = -0.5 * nearHeight;
	float l = -0.5 * nearWidth;
	float r = 0.5 * nearWidth;
	glm::vec4 v1(_near / r, 0, 0, 0);
	glm::vec4 v2(0, _near / t, 0, 0);
	glm::vec4 v3(0, 0, -1 * (_far + _near) / (_far - _near), -1);
	glm::vec4 v4(0, 0, -2 * (_far * _near) / (_far - _near), 0);
	this->proj = glm::mat4(v1, v2, v3, v4);
	this->currentCamera.setAspectRatio(aspectRatio);
	this->currentCamera.setFOV(fovy);
	this->currentCamera.setNear(_near);
	this->currentCamera.setFar(_far); 
	this->currentCamera.SetPerspectiveProjection(fovy, aspectRatio, _near, _far); 
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

float Renderer::CalculateColor(glm::vec3 &n1, glm::vec3 &n2, glm::vec3 &n)
{
	float ambient, diffusive, specular;
	std::vector<Light> lights = this->scene.getLights();
	return 0.0f;
}

//Draw a  line between 2 vertices, represented by p1, p2 which are 2 glm:vec3 vectors
//baesd on the Bresenham algorithem
void Renderer::DrawLine(Vertex p1, Vertex p2, glm::vec3 color, bool scale) 
{
	float x1, x2, y1, y2, z = 0;
	float dx = 0, dy = 0;
	double x_theta = (glm::dot(glm::normalize(p2.getPoint() - p1.getPoint()), glm::vec3(1, 0, 0)));	//cosine between p1-p2 and x axis
	double y_theta = (glm::dot(glm::normalize(p2.getPoint() - p1.getPoint()), glm::vec3(0, 1, 0)));	//cosine between p1-p2 and y axis
	glm::vec3 normal1 = p1.getNormal(); 
	glm::vec3 normal2 = p2.getNormal();
	
	// order the points so one is left and one is right depending on x1 and x2 values.
	if (scale) {
		x1 = viewportWidth/2 + (p1.getPoint().x);
		x2 = viewportWidth/2 + (p2.getPoint().x);
		y1 = viewportHeight/2 + (p1.getPoint().y);
		y2 = viewportHeight/2 + (p2.getPoint().y);
	}
	else {
		x1 = p1.getPoint().x;
		x2 = p2.getPoint().x;
		y1 = p1.getPoint().y;
		y2 = p2.getPoint().y;
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
		dx = 0;
		while (xLeft <= xRight) {
			putPixel((int)xLeft, (int)yLeft, color, dx * x_theta);
			xLeft = xLeft + 1;
			dx++;
		}
	}
	else if (xLeft == xRight) {
		dy = 0;
		if (yLeft <= yRight)			
			while (yLeft <= yRight) {
				putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
				yLeft = yLeft + 1;
				dy++;
			}
		else
			while (yLeft >= yRight) {
				putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
				yLeft = yLeft - 1;
				dy++;
			}
	}
	else if (a > 0 && a <= 1) {
		float e = 0 - 2*(xRight - xLeft);
		dx = 0;
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft + 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color/*, dx * x_theta*/);
			xLeft = xLeft + 1;
			dx++;
			e = e + 2 * (yRight - yLeft);
		}
	}
	else if (a > 1) {
		float e = 0 - 2*(yRight - yLeft);
		dy = 0;
		while (yLeft <= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e - 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color/*, dy * y_theta*/);
			yLeft = yLeft + 1;
			dy++;
			e = e + 2 * (xRight - xLeft);
		}
	}
	else if (a >= -1 && a < 0) {
		float e = 0 - 2*(xRight - xLeft);
		dx = 0;
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft - 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color/*, dx * x_theta*/);
			xLeft = xLeft + 1;
			dx++;
			e = e - 2 * (yRight - yLeft);
		}
	}
	else if (a < -1) {
		float e = 2*(yRight - yLeft);
		dy = 0;
		while (yLeft >= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e + 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color/*, dy * y_theta*/);
			yLeft = yLeft - 1;
			dy++;
			e = e + 2 * (xRight - xLeft);
		}
	}
}
void Renderer::DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale)
{
	float x1, x2, y1, y2, z = 0;
	float dx = 0, dy = 0;
	double x_theta = (glm::dot(glm::normalize(p2 - p1), glm::vec3(1, 0, 0)));	//cosine between p1-p2 and x axis
	double y_theta = (glm::dot(glm::normalize(p2 - p1), glm::vec3(0, 1, 0)));	//cosine between p1-p2 and y axis

	// order the points so one is left and one is right depending on x1 and x2 values.
	if (scale) {
		x1 = viewportWidth / 2 + (p1.x);
		x2 = viewportWidth / 2 + (p2.x);
		y1 = viewportHeight / 2 + (p1.y);
		y2 = viewportHeight / 2 + (p2.y);
	}
	else {
		x1 = p1.x;
		x2 = p2.x;
		y1 = p1.y;
		y2 = p2.y;
	}

	float xLeft = x1 > x2 ? x2 : x1;
	float xRight = x1 > x2 ? x1 : x2;
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
		dx = 0;
		while (xLeft <= xRight) {
			putPixel((int)xLeft, (int)yLeft, color, dx * x_theta);
			xLeft = xLeft + 1;
			dx++;
		}
	}
	else if (xLeft == xRight) {
		dy = 0;
		if (yLeft <= yRight)
			while (yLeft <= yRight) {
				putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
				yLeft = yLeft + 1;
				dy++;
			}
		else
			while (yLeft >= yRight) {
				putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
				yLeft = yLeft - 1;
				dy++;
			}
	}
	else if (a > 0 && a <= 1) {
		float e = 0 - 2 * (xRight - xLeft);
		dx = 0;
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft + 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color, dx * x_theta);
			xLeft = xLeft + 1;
			dx++;
			e = e + 2 * (yRight - yLeft);
		}
	}
	else if (a > 1) {
		float e = 0 - 2 * (yRight - yLeft);
		dy = 0;
		while (yLeft <= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e - 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
			yLeft = yLeft + 1;
			dy++;
			e = e + 2 * (xRight - xLeft);
		}
	}
	else if (a >= -1 && a < 0) {
		float e = 0 - 2 * (xRight - xLeft);
		dx = 0;
		while (xLeft <= xRight) {
			if (e > 0) {
				yLeft = yLeft - 1;
				e = e - 2 * (xRight - xLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color, dx * x_theta);
			xLeft = xLeft + 1;
			dx++;
			e = e - 2 * (yRight - yLeft);
		}
	}
	else if (a < -1) {
		float e = 2 * (yRight - yLeft);
		dy = 0;
		while (yLeft >= yRight) {
			if (e > 0) {
				xLeft = xLeft + 1;
				e = e + 2 * (yRight - yLeft);
			}
			// turn on pixel at point (x,y) with a black color
			putPixel((int)xLeft, (int)yLeft, color, dy * y_theta);
			yLeft = yLeft - 1;
			dy++;
			e = e + 2 * (xRight - xLeft);
		}
	}
}

void Renderer::DrawLine1(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale)
{
	int x1, x2, y1, y2;
	int z1, z2;
	
	// Bresenham's line algorithm
	// order the points so one is left and one is right depending on x1 and x2 values.
	if (scale) {
		x1 = viewportWidth / 2 + (p1.x);
		x2 = viewportWidth / 2 + (p2.x);
		y1 = viewportHeight / 2 + (p1.y);
		y2 = viewportHeight / 2 + (p2.y);
		z1 = p1.z;
		z2 = p2.z;
	}
	else {
		x1 = p1.x;
		x2 = p2.x;
		y1 = p1.y;
		y2 = p2.y;
		z1 = p1.z;
		z2 = p2.z;
	}

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(z1, z2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
	const float dz = (z1 - z2) / dx;
	float z = z1;

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	//start from left x-coordinate to the right x-coordinate
	for (int x = (int)x1; x < maxX; x++)
	{
		z += x * dz;
		if (steep)
		{
			putPixel(y, x, color, z);
		}
		else
		{
			putPixel(x, y, color, z);
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
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


//void Renderer::drawGrid(std::shared_ptr<MeshModel> m)
//{
//	std::vector<glm::vec3> ver = (*m).GetVertices();
//	for (int it = 0; it <= 10; it++)
//	{
//		if (it + 1 == 10)
//			break;
//		DrawLine(ver.at(it), ver.at(it + 1), glm::vec3(0, 0, 0), true);
//	}
//}



void Renderer::drawBetween2Edges(std::vector<Vertex> &points, Edge & e1, Edge & e2, const glm::vec3 &color)
{
	int  dy;
	float ratio = 0.0f;
	float longEdgeSlope = e1.getP2().y - e1.getP2().y / (e1.getP1().x - e1.getP2().x);
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
		int ex2 = e2.getP1().x - (int)((ratio)* dy);

		scanLine(points, ex1, ex2, y, color);

		alpha_1 += factorStep1;
	}

}

void Renderer::scanLine(std::vector<Vertex> &points, int &e1, int &e2, int &y, const glm::vec3 &color)
{
	int dx = 0;
	int delta = 0;
	int xdiff, x1 = e1, x2 = e2;
	int newx = 0, newy = 0;
	float factor = 0.0f;
	float factorStep = 0.0f;
	float z = 1.0f, oldz = 0.0f;
	float light = 0.0f;
	glm::vec3 baryCoor, normal;
	std::vector<glm::vec3> _points;
	_points.push_back(points.at(0).getPoint());
	_points.push_back(points.at(1).getPoint());
	_points.push_back(points.at(2).getPoint());

	if (e1 > e2)
	{
		x1 = e2;
		x2 = e1;
	}

	if ((xdiff = abs(x2 - x1)) == 0) return;

	factorStep = 1.0f / (float)xdiff;
	light = this->ambient + this->diffusive;

//	for (int x = x1 + 2; x < x2 - delta; x++)
	for (int x = x1; x < x2 ; x++)
	{
		{
			z = zDepth(glm::vec3(x, y, z), points);
			baryCoor = baryCentric(_points, glm::vec3(x, y, z)); 

			normal.x = (points.at(0).getNormal().x * baryCoor.x + points.at(1).getNormal().x * baryCoor.y + points.at(2).getNormal().x * baryCoor.z);
			normal.y = (points.at(0).getNormal().y * baryCoor.x + points.at(1).getNormal().y * baryCoor.y + points.at(2).getNormal().y * baryCoor.z);
			normal.z = (points.at(0).getNormal().z * baryCoor.x + points.at(1).getNormal().z * baryCoor.y + points.at(2).getNormal().z * baryCoor.z);
			//color
			if (scene.getLights().size() != 0)
			{
				float combinedColor = 0.0f;
				for (int i = 0; i < scene.getLights().size(); i++)
				{
					//combinedColor += scene.getLights().at(i).
				}
				light = this->ambient * this->diffusive * glm::dot(glm::normalize(this->diffusivePos), normal);
			}
			if (this->fillTriangles)
			{
				dx = 1;
				light = 1.0f;
			}
			else
			{
				light = this->ambient;
				dx = 0;
			}

			//putting the pixel with the adjusted light
			putPixel((x + this->viewportWidth / 2) , (y + this->viewportHeight / 2), light * color, z);

		}
		factor += factorStep;
	}
}

void Renderer::scanLine1(std::vector<Vertex>&points, int & e1, int & e2, int & y, const glm::vec3 & color)
{
	int dx = 0;
	int delta = 0;
	int xdiff, x1 = e1, x2 = e2;
	int newx = 0, newy = 0;
	float factor = 0.0f;
	float factorStep = 0.0f;
	float z = 1.0f, oldz = 0.0f;
	float light = 0.0f;
	glm::vec3 baryCoor, normal;
	std::vector<glm::vec3> _points;
	_points.push_back(points.at(0).getPoint());
	_points.push_back(points.at(1).getPoint());
	_points.push_back(points.at(2).getPoint());

	if (e1 > e2)
	{
		x1 = e2;
		x2 = e1;
	}

	if ((xdiff = abs(x2 - x1)) == 0) return;

	factorStep = 1.0f / (float)xdiff;
	light = this->ambient + this->diffusive;

	//	for (int x = x1 + 2; x < x2 - delta; x++)
	for (int x = x1 ; x <= x2; x++)
	{
		z = zDepth(glm::vec3(x, y, z), points);
		putPixel((x + this->viewportWidth / 2), (y + this->viewportHeight / 2), light * color, z);
		//{
		//	z = zDepth(glm::vec3(x, y, z), points);
		//	baryCoor = baryCentric(_points, glm::vec3(x, y, z));

		//	normal.x = (points.at(0).getNormal().x * baryCoor.x + points.at(1).getNormal().x * baryCoor.y + points.at(2).getNormal().x * baryCoor.z);
		//	normal.y = (points.at(0).getNormal().y * baryCoor.x + points.at(1).getNormal().y * baryCoor.y + points.at(2).getNormal().y * baryCoor.z);
		//	normal.z = (points.at(0).getNormal().z * baryCoor.x + points.at(1).getNormal().z * baryCoor.y + points.at(2).getNormal().z * baryCoor.z);
		//	//color
		//	if (scene.getLights().size() != 0)
		//	{
		//		float combinedColor = 0.0f;
		//		for (int i = 0; i < scene.getLights().size(); i++)
		//		{
		//			//combinedColor += scene.getLights().at(i).
		//		}
		//		light = this->ambient * this->diffusive * glm::dot(glm::normalize(this->diffusivePos), normal);
		//	}
		//	if (this->fillTriangles)
		//	{
		//		dx = 1;
		//		light = 1.0f;
		//	}
		//	else
		//	{
		//		light = this->ambient;
		//		dx = 0;
		//	}

		//	//putting the pixel with the adjusted light
		//	putPixel((x + this->viewportWidth / 2), (y + this->viewportHeight / 2), light * color, z);

		//}
		//factor += factorStep;
	}
}

void Renderer::fillTriangle2(std::vector<Vertex> points, const glm::vec3 & color)
{
	int dy = 0, y;
	int ex1, ex2;
	int delta1 = 0, delta2 = 0, delta3 = 0;
	float slope1, slope2, slope3;
	glm::vec2 z, P, P1, P2, P3;

	std::sort(points.begin(), points.end(), sort_dec_y);
	P1 = glm::vec2(points.at(0).getPoint().x, points.at(0).getPoint().y);
	P2 = glm::vec2(points.at(1).getPoint().x, points.at(1).getPoint().y);
	P3 = glm::vec2(points.at(2).getPoint().x, points.at(2).getPoint().y);

	slope1 = (float)(P1.y - P3.y) / (P1.x - P3.x);
	slope2 = (float)(P1.y - P2.y) / (P1.x - P2.x);
	slope3 = (float)(P2.y - P3.y) / (P2.x - P3.x);

	delta1 = (std::abs(slope1) < 0.5) ? 1 : 0;
	delta2 = (std::abs(slope2) < 0.5) ? 1 : 0;
	delta3 = (std::abs(slope3) < 0.5) ? 1 : 0;

	//flat bottom trianlge
	for (int y = P1.y - 1, dy = 1; y > P2.y; y--, dy++)
	{
		ex1 = P1.x - (int)dy/slope2 + delta2;
		ex2 = P1.x - (int)dy/slope1 + delta1;

		scanLine1(points, ex1, ex2, y, color);
	}
	//flat top trianlge
	for (int y = P3.y + 1, dy = 1; y <= P2.y; y++, dy++)
	{
		ex1 = P3.x + (int)dy / slope3 - delta3;
		ex2 = P3.x + (int)dy / slope1 - delta1;

		scanLine1(points, ex1, ex2, y, color);
	}

}

void Renderer::fillTriangle1(std::vector<Vertex> points, const glm::vec3 & color)
{
	int dy = 0, y;
	int ex1, ex2;
	float slope1, slope2, slope3;
	glm::vec2 z, P, P1, P2, P3;

	std::sort(points.begin(), points.end(), sort_dec_y);
	P1 = glm::vec2(points.at(0).getPoint().x, points.at(0).getPoint().y);
	P2 = glm::vec2(points.at(1).getPoint().x, points.at(1).getPoint().y);
	P3 = glm::vec2(points.at(2).getPoint().x, points.at(2).getPoint().y);

	Edge longEdge(P1, P3, color);		//highest y coordinate
	Edge shortEdge1(P1, P2, color);		//second highest y coordinate
	Edge shortEdge2(P2, P3, color);		//lowest y coordinate

	slope1 = (float)(longEdge.getP2().y - longEdge.getP1().y) / (longEdge.getP2().x - longEdge.getP1().x);
	slope2 = (float)(shortEdge1.getP2().y - shortEdge1.getP1().y) / (shortEdge1.getP2().x - shortEdge1.getP1().x);
	slope3 = (float)(shortEdge2.getP2().y - shortEdge2.getP1().y) / (shortEdge2.getP2().x - shortEdge2.getP1().x);


	for (int dy = 1, y = P1.y - 1; y > P3.y; dy++, y--)
	{
		if (P1.y == P2.y)
		{
			
		}
		// slopes are "/\"
		if (slope1 < 0 && slope2 > 0)
		{
			ex1 = (int)(P1.x - (dy) / slope2);
			ex2 = (int)(P1.x - (dy) / slope1);
		}
		else // slopes are "/\" (mirror)
		if (slope1 < 0 && slope2 > 0)
		{
			ex1 = (int)(P1.x - (dy) / slope1);
			ex2 = (int)(P1.x - (dy) / slope2);
		}
		else // slopes are "\\"
		if (slope1 < 0 && slope2 < 0 && (slope1 < slope2))
		{
			ex1 = (int)(P1.x - (dy) / slope1);
			ex2 = (int)(P1.x - (dy) / slope2);
		}
		else // slopes are "\\"
		if (slope1 < 0 && slope2 < 0 && (slope1 > slope2))
		{
			ex1 = (int)(P1.x - (dy) / slope2);
			ex2 = (int)(P1.x - (dy) / slope1);
		}
		else // slopes are "//"
		if (slope1 > 0 && slope2 > 0 && (slope1 < slope2))
		{
			ex1 = ceil(P1.x - (dy) / slope1);
			ex2 = (int)(P1.x - (dy) / slope2);
		}
		else // slopes are "//"
		if (slope1 > 0 && slope2 > 0 && (slope1 > slope2))
		{
			ex1 = ceil(P1.x - (dy) / slope2);
			ex2 = floor(P1.x - (dy) / slope1);
		}
		
		scanLine1(points, ex1, ex2, y, color);
	}

}


void Renderer::fillTriangle(std::vector<Vertex> points, const glm::vec3 &color)
{
	float alpha_1 = 0, alpha_2 = 0, alpha_3 = 0;
	float edge1ydiff, edge2ydiff, edge3ydiff;
	std::vector<Edge> edges;
	glm::vec2 z, z1, z2, z3, z4, z5;
	glm::vec2 x_left, y_top, y_down;

	std::sort(points.begin(), points.end(), sort_dec_y);
	z1 = glm::vec2(points.at(0).getPoint().x, points.at(0).getPoint().y);
	z2 = glm::vec2(points.at(1).getPoint().x, points.at(1).getPoint().y);
	z3 = glm::vec2(points.at(2).getPoint().x, points.at(2).getPoint().y);

	Edge longEdge(z1, z3, color);		//highest y coordinate
	Edge shortEdge1(z1, z2, color);		//second highest y coordinate
	Edge shortEdge2(z2, z3, color);		//lowest y coordinate

	drawBetween2Edges(points, longEdge, shortEdge1, color);
	drawBetween2Edges(points, longEdge, shortEdge2, color);
}

void Renderer::drawTriangle(std::vector<Vertex>&points, glm::vec3 &color)
{
	DrawLine1(points.at(0).getPoint(), points.at(1).getPoint(), color, true);
	DrawLine1(points.at(1).getPoint(), points.at(2).getPoint(), color, true);
	DrawLine1(points.at(2).getPoint(), points.at(0).getPoint(), color, true);
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
	this->currentCamera = scene.getCameras().at(0);
	
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
		glm::vec4 normal_vertex;
		//get the faces from the pointer to the model
		std::vector<Face> faces = (*model).GetFaces();
		std::string name = model->GetModelName(); 
		//get the vertices from the pointer to the model
		std::vector<Vertex> vertexs = model->getVertexs();
		//std::vector<glm::vec3> vertices = (*model).getVertexs();
		std::vector<glm::vec3> normals = (*model).GetNormals(); 
		typedef std::vector<glm::vec3>::iterator normal_it;
		typedef std::vector<Face>::iterator faces_it; 
		typedef std::vector<glm::vec4>::iterator center_it;
		//glm::mat4 t = this->currentCamera.getViewWorldTransform();

		//for debug use earse afterwards
		int count = 0;


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

			c.cPoints[i] = currentCamera.getProjectionTransformation() * c.cPoints[i];
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
		for (std::vector<Vertex>::iterator vertex = vertexs.begin(); vertex != vertexs.end(); vertex++) 
		{

			
			glm::vec3 normal = vertex->getNormal();
			glm::vec4 newVertex = glm::vec4((*vertex).getPoint().x, (*vertex).getPoint().y, (*vertex).getPoint().z, 1);
			newVertex = scaleTransform * newVertex;
			newVertex = rotationTransform * newVertex;
			newVertex = translateTransform * newVertex;
			// new set WORLD transformations.
			newVertex = worldRotate * newVertex;
			newVertex = worldTranslate * newVertex;

			/* right now the  problem is with this transformation !!*/
			newVertex = this->currentCamera.getViewTransformation() * newVertex;
			//newVertex.w = 1 ;
			newVertex = this->currentCamera.getProjectionTransformation() * newVertex;

			//newVertex = this->proj * newVertex;

			//newVertex = this->currentCamera.getViewTransformation() * newVertex;
			//newVertex.w = 0;

			//set new cube faces
			if (c.back >= newVertex.z) c.back = newVertex.z; 
			if (c.front < newVertex.z)  c.front = newVertex.z;

			if (c.right <= newVertex.x) c.right = newVertex.x;
			if (c.left > newVertex.x) c.left = newVertex.x;

			if (c.bottom >= newVertex.y) c.top = newVertex.y;
			if (c.top < newVertex.y) c.top = newVertex.y;

			(*vertex).setPoint(glm::vec3(newVertex.x, newVertex.y, newVertex.z)); 
		}
		// ############## END OF IMPORTANT CODE #################
		// ######################################################
	
		//draw vertex norals
		if (this->toDrawVertexNormals && (*model).getIsCurrentModel())
		{
			glm::vec3 normal;
			glm::vec4 newVertex;
			for (std::vector<Vertex>::iterator vertex = vertexs.begin(); vertex != vertexs.end(); vertex++)
			{
				normal = vertex->getNormal();
				newVertex = glm::vec4((*vertex).getPoint().x, (*vertex).getPoint().y, (*vertex).getPoint().z, 1);
				normal.x *= -V_NORMAL_SCALE; normal.y *= -V_NORMAL_SCALE; normal.z *= -V_NORMAL_SCALE;
				DrawLine(newVertex, glm::vec3(newVertex.x + normal.x, newVertex.y + normal.y, (newVertex.z + normal.z)), PURPLE, true);
			}

		}
		
		//iterate over the faces vector of the model
		for (std::vector<Face>::iterator face = faces.begin(); face != faces.end(); face++) 
		{
			//face vertices for fill triangles purpose
			Vertex first, second, third;
			std::vector<Vertex> points;
			first = vertexs.at((*face).GetVertexIndex(0) - 1);
			second = vertexs.at((*face).GetVertexIndex(1) - 1);
			third = vertexs.at((*face).GetVertexIndex(2) - 1);

			////get the indices of the vertices for each face
			//std::vector<int> indices = (*face).GetVertices();
			////iterate over the indices
			//for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) 
			//{
			//	//here we draw a line between two successive pointes by the indexes from the indices vector
			//	//if we are at the end of the indices vector we connect the vertex with this index
			//	//with the vertex with the index from the start of the vector
			//	if ((vindex+1) == indices.end()) {
			//		//DrawLine(vertices.at(*(vindex)-1), vertices.at(indices.at(0)-1), glm::vec3(0, 0, 0),true);
			//		DrawLine1(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(indices.at(0) - 1).getPoint(), BLACK, true);
			//	} 
			//	else //draw a line between the two vertices by their indices from the vector "indices"
			//		DrawLine1(vertexs.at(*(vindex)-1).getPoint(), vertexs.at(*(vindex + 1) - 1).getPoint(), BLACK, true);


			//	//calculate face normal and centeroid
			//	first = vertexs.at((*face).GetVertexIndex(0) - 1);
			//	second = vertexs.at((*face).GetVertexIndex(1) - 1);
			//	third = vertexs.at((*face).GetVertexIndex(2) - 1);
			//	glm::vec3 centerv = glm::vec3((first.getPoint().x + second.getPoint().x + third.getPoint().x) / 3,(first.getPoint().y + second.getPoint().y + third.getPoint().y) / 3, (first.getPoint().z + second.getPoint().z + third.getPoint().z) / 3);

			//	//normals
			//	glm::vec3 normal = glm::normalize(glm::cross(first.getPoint() - second.getPoint(), first.getPoint() - third.getPoint()));
			//	first.addNormal(normal);
			//	second.addNormal(normal);
			//	third.addNormal(normal);
			//	normal.x *= -F_NORMAL_SCALE; normal.y *= -F_NORMAL_SCALE; normal.z *= -F_NORMAL_SCALE;
			//	//draw face normals
			//	if (this->toDrawFaceNormals && (*model).getIsCurrentModel())
			//	{
			//		putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y, RED);
			//		putPixel(viewportWidth / 2 + centerv.x + 1, viewportHeight / 2 + centerv.y, RED);
			//		putPixel(viewportWidth / 2 + centerv.x - 1, viewportHeight / 2 + centerv.y, RED);
			//		putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y + 1, RED);
			//		putPixel(viewportWidth / 2 + centerv.x, viewportHeight / 2 + centerv.y - 1, RED);
			//		DrawLine( centerv, glm::vec3(centerv.x + normal.x, centerv.y + normal.y, -(centerv.z + normal.z)), RED, true);
			//	}
			//}

			
			points.push_back(first);
			points.push_back(second);
			points.push_back(third);
			drawTriangle(points, BLACK);
			glm::vec3 finalColor = (this->fillTriangles) ? glm::vec3(0.8, 0.8, 0.8) : this->ambientColor;
			if (this->fillTriangles)
				fillTriangle2(points, GREEN);

		}
/*
		Vertex first(glm::vec3(150, 120, 0), glm::vec3(0, 0, 0));
		Vertex second(glm::vec3(250, 70, 0), glm::vec3(0, 0, 0));
		Vertex third(glm::vec3(20, 20, 0), glm::vec3(0, 0, 0));

		Vertex first1(glm::vec3(100, 20, 30), glm::vec3(1, 0, 0));
		Vertex second1(glm::vec3(150, 100, 20), glm::vec3(1, 0, 0));
		Vertex third1(glm::vec3(70, 70, -30), glm::vec3(1, 0, 0));

		Vertex first2(glm::vec3(200, 20, 30), glm::vec3(1, 0, 0));
		Vertex second2(glm::vec3(300, 20, 80), glm::vec3(1, 0, 0));
		Vertex third2(glm::vec3(300, 120, -30), glm::vec3(1, 0, 0));

		std::vector<Vertex> points;
		std::vector<Vertex> points1;
		std::vector<Vertex> points2;
		points.push_back(first);
		points.push_back(second);
		points.push_back(third);
		points1.push_back(first1);
		points1.push_back(second1);
		points1.push_back(third1);

		points2.push_back(first2);
		points2.push_back(second2);
		points2.push_back(third2);

		if (this->drawLines)
		{
			drawTriangle(points, BLACK);
			drawTriangle(points1, BLACK);
			drawTriangle(points2, BLACK);
		}
		if (this->fillTriangles)
		{
			fillTriangle2(points, GREEN);
			fillTriangle2(points1, RED);
			fillTriangle2(points2, PURPLE);
		}*/
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