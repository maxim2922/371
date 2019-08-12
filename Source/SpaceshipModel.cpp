#include "SpaceshipModel.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "World.h"
#include "Camera.h"
#include "ThirdPersonCamera.h"
#include <glm/ext/matrix_transform.inl>
#include "EventManager.h"
#include <GLFW/glfw3.h>
#include "TextureLoader.h"

using namespace glm;
using namespace std;

SpaceshipModel::SpaceshipModel(vec3 size) : Model(), mLookAt(0.0f, 0.0f, -1.0f)
{
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;

	ObjLoader* modelLoader = new ObjLoader();
	modelLoader->loadObj("../Assets/Models/f.obj", vertices, uvs, normals);

	vertexCount = vertices.size();

	// Create a vertex array
	glGenVertexArrays(1, &mVAO);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	glGenBuffers(3, &(mVBO[0]));
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//vertices

	glVertexAttribPointer(0,
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(vec3),		 // stride
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0     // normal is a vec3
	);
	glEnableVertexAttribArray(2);

	//uvs (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);

	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0      // uv is a vec2
	);
	glEnableVertexAttribArray(3);
}

SpaceshipModel::~SpaceshipModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(3, &mVBO[0]);
	glDeleteVertexArrays(1, &mVAO);
}

bool IsCameraThirdPerson()
{
	const Camera* cam = World::GetInstance()->GetCurrentCamera();
	return dynamic_cast<const ThirdPersonCamera*>(cam) != nullptr;
}

void SpaceshipModel::Update(float dt)
{

	bool cameraThirdPerson = IsCameraThirdPerson();

	if (cameraThirdPerson) {
		const Camera* cam = World::GetInstance()->GetCurrentCamera();

		/*if (mPosition == vec3(0.0f)) {
			mPosition = cam->GetPosition();
		}

		if (mPosition != cam->GetPosition()) {
			mPosition = cam->GetPosition() * dt;
		}*/


		mPosition = cam->GetPosition();

		SetRotation(vec3(0.0f, 1.0f, 0.0f), cam->GetHorizontalAngle() + 90);
	}
}

void SpaceshipModel::Draw()
{

	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");
	glBindTexture(GL_TEXTURE_2D, mTextureID);





	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVAO);

	bool cameraThirdPerson = IsCameraThirdPerson();

	if (cameraThirdPerson)
	{
		mat4 spaceshipWorldMatrix = translate(mat4(1.0f), mPosition) *
			rotate(mat4(1.0f), radians(mRotationAngleInDegrees), vec3(0.0f, 1.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

		GLuint worldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spaceshipWorldMatrix[0][0]);
	}
	else
	{
		mat4 spaceshipWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.0f)) *
			rotate(mat4(1.0f), radians(mRotationAngleInDegrees), vec3(0.0f, 1.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

		GLuint worldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spaceshipWorldMatrix[0][0]);
	}

	glDrawArrays(GL_QUADS, 0, vertexCount);
}

bool SpaceshipModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
