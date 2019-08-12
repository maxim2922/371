#pragma once
#include "Model.h"

class AsteroidModel : public Model
{
public:
	AsteroidModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	AsteroidModel(glm::vec3 location, glm::vec3 velocity, glm::vec3 size );
	virtual ~AsteroidModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);
	virtual void randomArray();

private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	struct Vertex2
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 UV;
	};

	std::vector<bool> tf;


	const float random2 = 1.0f / (rand() % 4 + 4.0f);
	float random1 = rand() % 30;
	
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int numOfVertices;
};
