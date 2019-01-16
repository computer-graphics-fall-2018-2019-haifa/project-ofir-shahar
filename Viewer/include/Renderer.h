#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "Camera.h"


/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;
	float scaleNumber;
	float fov;
	bool projection; 
	Camera currentCamera;
	bool hasModel;
	bool tooDrawaCube, toDrawFaceNormals, toDrawLineNormals;
	std::shared_ptr<MeshModel> currentModel;
	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

	//drawing routings
	void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale);
	void drawCube(); 
	void drawPolygon(std::vector<Vertex>)
	void fillTriangle( Face &face, glm::vec3 color); 
	void fillTriangle(std::vector<glm::vec3> points, glm::vec3 color);
	void fillTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	static inline glm::vec2 interpolate(glm::vec2 p1, glm::vec2 p2, float alpha) { return alpha * p1 + (1 - alpha)*p2; }

	std::vector<std::string> ExcludeModels;
	static bool sort_asc_x(const glm::vec2 &x, const glm::vec2 &y) { return x.x < y.x; }
	static bool sort_dec_y(const glm::vec2 &x, const glm::vec2 &y) { return x.y > y.y; }
	//static bool sort(const glm::vec3 &x, const glm::vec3 &y) { return x.y > y.y; }

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();
	bool isHasModel();
	

	void setHasModel();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	
	//getters\setters
	//---------------
	const std::vector<std::string> getExcludeModels() const { return this->ExcludeModels; }
	void setExcludeModels(std::vector<std::string> v) { this->ExcludeModels = v; }

	void setScaleNumber(float f);
	void setEyeX(float eyex);

	void setFov(float f);

	void setProjection(bool p);

	const bool getProjection();


	void setPerspective(float f, float ar, float n, float fa);

	void rotateLocalX(float x);
	void rotateLocalY(float y);
	void rotateLocalZ(float z);
	void rotateWorldX(float x);
	void rotateWorldY(float y);
	void rotateWorldZ(float z);
	void translate(float x, float y, float z);
	void setWorldTranslation(float x, float y, float z);
	const void setToDrawaCube(const bool b) { this->tooDrawaCube = b; }
	bool getToDrawaCube() const { return this->tooDrawaCube; }
	const void setToDrawFaceNormals(bool b) { this->toDrawFaceNormals = b; }
	bool getToDrawFaceNormals() const { return this->toDrawFaceNormals; }

	void setCurrentModel(std::shared_ptr<MeshModel> m) { this->currentModel = m; }
	
	void setViewPortWidth( int w) { this->viewportWidth = w; }
	void setViewPortHeight( int h) { this->viewportHeight = h; }
	
	const int& setViewPortWidth() const { return this->viewportWidth; }
	const int& setViewPortHeight() const { return this->viewportHeight; }



};
