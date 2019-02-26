#pragma once
#include "Scene.h"
#include "Vertex.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <cmath>
#include "Camera.h"
#include "Edge.h"
#include "Definitions.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Camera.h"


/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	float **viewport;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;
	float scaleNumber;
	float diffusive;
	float ambient;
	float ambientIntensity;
	float ambientK;
	float fov;
	bool projection; 
	bool hasModel;
	bool tooDrawaCube, toDrawFaceNormals, toDrawLineNormals, toDrawVertexNormals;
	glm::vec3 ambientColor;
	glm::vec3 diffusivePos;
	glm::mat4x4 worldToCameraTransformation;
	std::shared_ptr<MeshModel> currentModel;
	std::vector<Light> lights;
	void putPixel(int i, int j, const glm::vec3 & color);
	void putPixel(int x, int y, const glm::vec3& color, const float &z);
	void createBuffers(int viewportWidth, int viewportHeight);
	float zDepth(glm::vec3, std::vector<Vertex>);
	glm::vec3 baryCentric(std::vector<glm::vec3> &polygon, glm::vec3 &point);
	float Fab(glm::vec3 &a, glm::vec3 &b, glm::vec3 &point);
	Camera currentCamera;
	Scene scene;
	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

	//drawing routings
	float CalculateColor(glm::vec3 &n1, glm::vec3 &n2, glm::vec3 &n);
	void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale);
	void DrawLine(Vertex p1, Vertex p2, glm::vec3 color, bool scale);
	void drawCube(); 
	void drawBetween2Line(std::vector<Vertex> &points, Edge &e1, Edge &e2);
	void scanLine(std::vector<Vertex>&, int &e1, int &e2, int &y);
	void fillTriangle( Face &face, glm::vec3 color); 
	void fillTriangle(std::vector<Vertex> points, glm::vec3 color);
	void fillTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);


	std::vector<std::string> ExcludeModels;
	static bool sort_asc_x(const Vertex &x, const Vertex &y) { return x.getPoint().x < y.getPoint().x; }
	static bool sort_dec_y(const Vertex &x, const Vertex &y) { return x.getPoint().y > y.getPoint().y; }

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();
	bool isHasModel();
	void setHasModel();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);	
	void rotateLocalX(float x);
	void rotateLocalY(float y);
	void rotateLocalZ(float z);
	void rotateWorldX(float x);
	void rotateWorldY(float y);
	void rotateWorldZ(float z);
	void translate(float x, float y, float z);
	void addLight(const Light &l) { this->lights.push_back(l); }
	
	//getters\setters
	//---------------
	//getters
	const std::vector<std::string> getExcludeModels() const { return this->ExcludeModels; }
	const bool getProjection();
	bool getToDrawaCube() const { return this->tooDrawaCube; }
	bool getToDrawFaceNormals() const { return this->toDrawFaceNormals; }
	const bool getToDrawVertexNormals() { return this->toDrawVertexNormals; }
	const int& getViewPortWidth() const { return this->viewportWidth; }
	const int& getViewPortHeight() const { return this->viewportHeight; }

	//setters
	void setToDrawVertexNormals(const bool &b) { this->toDrawVertexNormals = b; }
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void setExcludeModels(std::vector<std::string> v) { this->ExcludeModels = v; }
	void setScaleNumber(float f);
	void setEyeX(float eyex);
	void setFov(float f);
	void setProjection(bool p);
	void setPerspective(float f, float ar, float n, float fa);
	void setWorldTranslation(float x, float y, float z);
	void setToDrawaCube(const bool b) { this->tooDrawaCube = b; }
	void setToDrawFaceNormals(bool b) { this->toDrawFaceNormals = b; }
	void setCurrentModel(std::shared_ptr<MeshModel> m) { this->currentModel = m; this->currentModel->setIsCurrentModel(true); }
	void setViewPortWidth(const int& w) { this->viewportWidth = w; }
	void setViewPortHeight(const int& h) { this->viewportHeight = h; }
	void setViewPortWidth( int w) { this->viewportWidth = w; }
	void setViewPortHeight( int h) { this->viewportHeight = h; }
	void setAmbientIntensity(const float &c) { this->ambient = this->ambientK  * c; }
	void setAmbientCoefficient(const float &c) { this->ambient = this->ambientIntensity * c; }
	void setAmbientColor(const glm::vec3 &color) { this->ambientColor = color; }
};
