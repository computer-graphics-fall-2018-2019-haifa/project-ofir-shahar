#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Vertex Utils::VertexFromStream(std::istream& issLine)
{
	int depth = INT32_MAX;
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	//return glm::vec3(x, y, z);
	glm::vec3 vertex(x, y, z);
	glm::vec3 color(0, 0, 0);
	return Vertex(vertex, color, depth);
}

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	int depth = INT32_MAX;
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);

}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	//std::vector<glm::vec3> vertices;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::VertexFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			//normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// Texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//iterate over the faces vector of the model, and calculate the face normals
	for (std::vector<Face>::iterator face = faces.begin(); face != faces.end(); face++)
	{
		//face vertices for fill triangles purpose
		Vertex first, second, third;
		glm::vec3 normal;

		//get the indices of the vertices for each face
		std::vector<int> indices = (*face).GetVertices();
		//iterate over the indices
		for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++)
		{
			first = vertices.at((*face).GetVertexIndex(0) - 1);
			second = vertices.at((*face).GetVertexIndex(1) - 1);
			third = vertices.at((*face).GetVertexIndex(2) - 1);
			normal = glm::normalize(glm::cross(first.getPoint() - second.getPoint(), first.getPoint() - third.getPoint()));

			//add newly created normal to each face vertex
			vertices.at((*face).GetVertexIndex(0) - 1).addNormal(normal);
			vertices.at((*face).GetVertexIndex(1) - 1).addNormal(normal);
			vertices.at((*face).GetVertexIndex(2) - 1).addNormal(normal);
		}		
		//add normal to normal vector
		normals.push_back(normal);
	}
	//normalize all vertex's normals
	for (std::vector<Vertex>::iterator vertex = vertices.begin(); vertex != vertices.end(); vertex++)
	{
		glm::vec3 norm = glm::normalize(vertex->getNormal());
		(*vertex).setNormal(norm);
	}

	return MeshModel(faces, vertices, normals, Utils::GetFileName(filePath), true);
}

void Utils::createGrid()
{
	std::string path = "../Data/obj_examples/grid.obj";
	//FILE* file;

	//if ((file = fopen(path.c_str(), "w")) != nullptr)
	//{
	//	for (float z = -0.050; z <= 0.050; z = z + 0.01)
	//	{
	//		fprintf(file, "v -0.05 0 %f\n", z);
	//		fprintf(file, "v 0.05 0 %f\n", z);
	//	}

	//	for (float x = 0.050; x >= 0; x = x - 0.01)
	//	{
	//		fprintf(file, "v %f 0 -0.05\n", x);
	//		fprintf(file, "v %f 0 0.05\n", x);
	//	}

	//	
	//}

	std::ofstream file;

	file.open(path);


	if (file)
	{
		file << "grid" << std::endl;

		for (float z = -0.05; z <= 0.05; z = (float)(z + 0.01))
		{
			file << "v " << -0.50 << " 0 " << z << std::endl;
			file << "v " << 0.50 << " 0 " << z << std::endl;
		}

		for (float x = 0.05; x >= 0; x = (float)(x - 0.01))
		{
			file << "v " << x << " 0 " << -0.50 << std::endl;
			file << "v " << x << " 0 " << 0.50 << std::endl;
		}

		file.close();
	}
}

glm::vec4 Utils::Centeroid(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
	glm::vec4 result;

	result.x = (1 / 3)*(v1.x + v2.x + v3.x);
	result.y = (1 / 3)*(v1.y + v2.y + v3.y);
	result.z = (1 / 3)*(v1.z + v2.z + v3.z);

	return result;
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}