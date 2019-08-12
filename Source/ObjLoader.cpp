#include <fstream>
#include "ObjLoader.h"
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include "Renderer.h"

using namespace glm;

bool ObjLoader::loadObj(const char* path, vector<vec3>& out_vertices, vector<vec2>& out_uvs, vector<vec3>& out_normals)
{
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> temp_vertices;
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;
	int i = 0;
	while (1) {
		i++;
		char headline[150];	// here i'm assuming there isn't more than 100 characters in a sentence.

								// read the first word of the line
		int res = fscanf(file, "%s", headline);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(headline, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(headline, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(headline, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(headline, "f") == 0) {
			//std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches != 9 && matches != 12) {
				printf("can't parse f values \n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			if (matches == 12) {
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[3]);
				normalIndices.push_back(normalIndex[3]);
			}
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		out_uvs.push_back(vertex);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		out_normals.push_back(vertex);
	}

	//MtlLoader("../Assets/Models/fighter.mtl");
}

void ObjLoader::MtlLoader(const char* path) {

	vector<string>tmp;
	string imageName;
	ifstream fin;
	char line[256];

	fin.open(path);
	while (!fin.eof())
	{
		fin.getline(line, 200);
		tmp.push_back(line);
	}
	fin.close();

	//READ MATERIALS
	bool matExists = false;
	string lastMapFileName;
	Material m;

	for (int i = 0; i < tmp.size() + 1; i++)
	{

		// To register the last material file before exiting for loop
		if (i == tmp.size()) {
			materials.push_back(m);
			break;
		}

		if (tmp[i][0] == '\n' || tmp[i][0] == '#')
			continue;

		if (tmp[i][0] == 'n' && tmp[i][1] == 'e' && tmp[i][2] == 'w')
		{
			//This is a new material, let's save the previous one first
			if (matExists) {
				materials.push_back(m);
				m.reset();
				matExists = false;
			}
			sscanf(tmp[i].c_str(), "newmtl %s", &m.name);
		}
		else if (tmp[i][0] == 'N' && tmp[i][1] == 's') {
			sscanf(tmp[i].c_str(), "Ns %f", &m.ns);
			matExists = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 'a') {
			sscanf(tmp[i].c_str(), "Ka %f %f %f", &m.ka[0], &m.ka[1], &m.ka[2]);
			matExists = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 'd') {
			sscanf(tmp[i].c_str(), "Kd %f %f %f", &m.kd[0], &m.kd[1], &m.kd[2]);
			matExists = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 's') {
			sscanf(tmp[i].c_str(), "Ks %f %f %f", &m.ks[0], &m.ks[1], &m.ks[2]);
			matExists = true;
		}
		else if (tmp[i][0] == 'N' && tmp[i][1] == 'i') {
			sscanf(tmp[i].c_str(), "Ni %f", &m.ni);
			matExists = true;
		}
		else if (tmp[i][0] == 'd' && tmp[i][1] == ' ') {
			sscanf(tmp[i].c_str(), "d %f", &m.d);
			matExists = true;
		}
		else if (tmp[i][0] == 'i' && tmp[i][1] == 'l') {
			sscanf(tmp[i].c_str(), "illum %f", &m.illum);
			matExists = true;
		}
		else if (tmp[i][0] == 'm' && tmp[i][1] == 'a')
		{
			sscanf(tmp[i].c_str(), "map_Kd %s", &m.mapFileName);
			m.textureID = LoadMaterial(m.mapFileName);
			matExists = true;
		}
	}
}

unsigned int ObjLoader::LoadMaterial(string imgPath)
{
	unsigned int id;
	sf::Image img;
	img.loadFromFile(imgPath);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glEnable(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

	GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");
	glBindTexture(GL_TEXTURE_2D, id);

	return id;
}