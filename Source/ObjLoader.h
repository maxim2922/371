#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace std;
using namespace glm;

class ObjLoader {
public:
	bool loadObj(const char* path,
		vector <vec3>& vertices,
		vector <vec2>& uvs,
		vector <vec3>& normals);

private:
};