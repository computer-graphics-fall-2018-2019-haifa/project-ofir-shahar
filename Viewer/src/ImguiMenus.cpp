#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>
#include "Renderer.h"

bool showDemoWindow = false;
bool showAnotherWindow = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 1500.0f;
		static float turnUpDown = 0.0f;
		static int counter = 0;
		static float fov = 50.0f;
		static float rotateLocalX = 0.0f;
		static float rotateLocalY = 0.0f;
		static float rotateLocalZ = 0.0f;
		static float scaleX = 0.0f;
		static float scaleY = 0.0f;
		static float scaleZ = 0.0f;
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		
		if (ImGui::SliderFloat("scale", &f, 0.0f, 2000.0f)) {
			renderer.setScaleNumber(f);
		}// Edit 1 float using a slider from 0.0f to 2000.0f
		if (ImGui::SliderFloat("turn left or right", &turnUpDown, 0.0f, 360.0f)) {
			renderer.setEyeX(turnUpDown);
		}// Edit 1 float using a slider from 0.0f to 2000.0f
		if (ImGui::SliderFloat("FOV", &fov, 0.0f, 90.0f)) {
			renderer.setFov(fov);
		}
		ImGui::Text("Current Object:");
		ImGui::Text("Local Rotations");
		if (ImGui::SliderFloat("Rotate local x", &rotateLocalX, 0.0, 360.0f)) {
			renderer.rotateLocalX(rotateLocalX);
		}
		if (ImGui::SliderFloat("Rotate local y", &rotateLocalY, 0.0, 360.0f)) {
			renderer.rotateLocalY(rotateLocalY);
		}
		if (ImGui::SliderFloat("Rotate local z", &rotateLocalZ, 0.0, 360.0f)) {
			renderer.rotateLocalZ(rotateLocalZ);
		}
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
		ImGui::End();
	}

	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						renderer.setEyeX(0);
						renderer.setScaleNumber(1800);
						renderer.setFov(45);
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}