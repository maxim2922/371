#include "RingModel.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "World.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.inl>
#include <GLFW/glfw3.h>
#include "TextureLoader.h"

using namespace glm;
using namespace std;

RingModel::RingModel(vec3 size) : Model()
{
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;

	ObjLoader* modelLoader = new ObjLoader();
	modelLoader->loadObj("../Assets/Models/Ring.obj", vertices, uvs, normals);

	vertexCount = vertices.size();

	// Create a vertex array
	glGenVertexArrays(1, &mVAO);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	glGenBuffers(3, &(mVBO[0]));
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);

	//uvs (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);

	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0      // uv is offseted a vec2
	);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0     // normal is offseted a vec3
	);
}

RingModel::~RingModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(3, &mVBO[0]);
	glDeleteVertexArrays(1, &mVAO);
}

void RingModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
	Model::Update(dt);
}

void RingModel::Draw()
{
	if (mTextureValid)
	{
		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glUniform1i(textureLocation, 0);

	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVAO);

	mat4 ringWorldMatrix = translate(mat4(1.0f), mPosition) *
		rotate(mat4(1.0f), radians(mRotationAngleInDegrees), vec3(0.0f, 1.0f, 0.0f)) *
		scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));


	GLuint worldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// DEFAULT
	//===================================================================================================
	// FOR BOTH CAMERA VIEWS

	/*glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVAO);


	GLuint worldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

	const Camera* cam = World::GetInstance()->GetCurrentCamera();
	mat4 viewMatrix = cam->GetViewMatrix();

	mRotationAngleInDegrees = 0;

	bool cameraThirdPerson = false;
	if (dynamic_cast<const ThirdPersonCamera*>(cam))
	{
		cameraThirdPerson = true;
	}

	if (cameraThirdPerson)
	{
		vec3 cameraPosition = vec3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]);
		mat4 spaceshipWorldMatrix = translate(mat4(1.0f), cameraPosition) *
			rotate(mat4(1.0f), radians(mRotationAngleInDegrees), vec3(0.0f, 1.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spaceshipWorldMatrix[0][0]);
	}
	else
	{
		GLuint viewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");

		mat4 spinningCubeWorldMatrix(1.0f);
		mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.0f)) *
			rotate(mat4(1.0f), radians(mRotationAngleInDegrees), vec3(0.0f, 1.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &spinningCubeViewMatrix[0][0]);
	}*/

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

bool RingModel::ParseLine(const std::vector<ci_string> &token)
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