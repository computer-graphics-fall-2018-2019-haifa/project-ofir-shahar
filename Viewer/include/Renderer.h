#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "Camera.h"

typedef enum projection { Perspective, orthgraphic };

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
	Camera camera;
	MeshModel* currentModel;

	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, bool scale);
	//void AddLineBresenhamStyle(float x1, float y1, float x2, float y2, glm::vec3 color);
	//float getScaleNumber();
	void setScaleNumber(float f);
	void setEyeX(float eyex);
	void setFov(float f);
<<<<<<< HEAD
	void setProjection(bool p);

	const bool getProjection();
=======
	void rotateLocalX(float x);
	void rotateLocalY(float y);
	void rotateLocalZ(float z);
	// Add more methods/functionality as needed...
>>>>>>> 2efa6611ae1c885a3a9ce3035dd27b6c29ecb7b3
};
