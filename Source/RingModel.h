#pragma once
#include "Model.h"

class RingModel : public Model
{
public:
	RingModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~RingModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	int vertexCount;

	unsigned int mVAO;
	unsigned int mVBO[3];
};
