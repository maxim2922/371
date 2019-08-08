#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace std;
using namespace glm;

class ObjLoader {
public:
	bool loadObj(const char* path,
		vector <vec3>& out_vertices,
		vector <vec2>& out_uvs,
		vector <vec3>& out_normals);
};
