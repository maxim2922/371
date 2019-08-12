#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <SFML/Graphics/Image.hpp>

using namespace std;
using namespace glm;

struct Material {
	char name[100];
	float ns, ni, d, illum;
	float ka[3], kd[3], ks[3];
	unsigned int textureID;
	char mapFileName[100];
	sf::Image image;

	void reset() {
		ns = ni = d = illum = 0;
		memset(name, 0, 100);
		memset(ka, 0, 3);
		memset(kd, 0, 3);
		memset(ks, 0, 3);
		textureID = 0;
		memset(mapFileName, 0, 100);
		image = sf::Image();
	}
};

class ObjLoader {
public:
	bool loadObj(const char* path,
		vector <vec3>& vertices,
		vector <vec2>& uvs,
		vector <vec3>& normals);

	void MtlLoader(const char* path);
	unsigned int LoadMaterial(string imgPath);

private:
	vector<Material>materials;
};