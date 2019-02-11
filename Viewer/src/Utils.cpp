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
	//return glm::vec3(x, y, z);
	glm::vec3 vertex(x, y, z);
	glm::vec3 color(0, 0, 0);
	return vertex;
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	MeshModel *result_ptr = new MeshModel();
	MeshModel result;
	std::vector<Face> faces;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());
	std::string name = Utils::GetFileName(filePath);

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
			normals.push_back(Utils::Vec3fFromStream(issLine));
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
	//iterate over the faces vector of the model
	for (std::vector<Face>::iterator face = faces.begin(); face != faces.end(); face++)
	{
		std::vector<int> indices = (*face).GetVertices();
		//iterate over the face's indices
		for (std::vector<int>::iterator vindex = indices.begin(); vindex != indices.end(); vindex++) 
		{

		}
	}
	return MeshModel(faces, vertices, normals, true, name);
	
}

const std::shared_ptr<MeshModel> Utils::LoadMeshModelPtr(const std::string& filePath)
{
	//MeshModel *result_ptr = new MeshModel();
	std::vector<Face> faces;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());
	std::string name = Utils::GetFileName(filePath);
	MeshModel *result_ptr = new MeshModel(faces, vertices, normals, true, name);

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
			result_ptr->addVertex(Utils::VertexFromStream(issLine));
			//vertices.push_back(Utils::VertexFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			//normals.push_back(Utils::Vec3fFromStream(issLine));
			result_ptr->addNormal(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// Texture coordinates
		}
		else if (lineType == "f")
		{
			//faces.push_back(Face(issLine));
			result_ptr->addFace(Face(issLine));
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
	/*result_ptr->setFaces(faces);
	result_ptr->setVertexs(vertices);
	result_ptr->setNormals(normals);*/
	result_ptr->setIsCurrentModel(true);
	//return result_ptr;
	return std::make_shared<MeshModel>(*result_ptr);

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