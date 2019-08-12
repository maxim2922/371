#pragma once

#include "Model.h"

class SpaceshipModel : public Model
{
public:
	SpaceshipModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~SpaceshipModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	glm::vec3 mLookAt;

	int vertexCount;

	unsigned int mVAO;
	unsigned int mVBO[3];
};