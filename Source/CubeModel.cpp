//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "CubeModel.h"
#include "Renderer.h"
#include "World.h"
#include "Camera.h"

using namespace glm;

CubeModel::CubeModel(vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Cube
	vec3 halfSize = size * 0.5f;
	
	Vertex2 vertexBuffer[] = {  // position,                normal,                  color
								{ vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f / 3.0f) }, //left - red
								{ vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 1.0f / 3.0f) },
								{ vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 2.0f / 3.0f) },

								{ vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f / 3.0f) },
								{ vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 2.0f / 3.0f) },
								{ vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 2.0f / 3.0f) },

								{ vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 2.0f / 3.0f) }, // far - blue
								{ vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f / 3.0f) },
								{ vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 2.0f / 3.0f) },

								{ vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 2.0f / 3.0f) },
								{ vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 1.0f / 3.0f) },
								{ vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f / 3.0f) },

								{ vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f / 3.0f) }, // bottom - turquoise
								{ vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 0.0f) },
								{ vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 0.0f) },

								{ vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f / 3.0f) },
								{ vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 1.0f / 3.0f) },
								{ vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 0.0f) },

								/*rrrrrrrrrrrrrrrrrrrrrrrr*/{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 2.0f / 3.0f) }, // near - green
								/*rrrrrrrrrrrrrrrrrrrrrrrr*/{ vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 1.0f / 3.0f) },
								/*rrrrrrrrrrrrrrrrrrrrrrrr*/{ vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f / 3.0f) },

								/*rrrrrrr7rrrrrrrr*/{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 2.0f / 3.0f) },
								/*rrrrrrrrrrrrrrrrrrrrrrrr*/{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 2.0f / 3.0f) },
								/*rrrrrrrrrrrrrrrrrrrrrrrr*/{ vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f / 3.0f) },

															{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 2.0f / 3.0f) }, // right - purple
															{ vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 1.0f / 3.0f) },
															{ vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 2.0f / 3.0f) },

															{ vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.75f, 1.0f / 3.0f) },
															{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 2.0f / 3.0f) },
															{ vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f / 3.0f) },

															{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 2.0f / 3.0f) }, // top - yellow
															{ vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f) },
															{ vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 1.0f) },

															{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 2.0f / 3.0f) },
															{ vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 1.0f) },
															{ vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.25f, 2.0f / 3.0f) }
	};

	numOfVertices = sizeof(vertexBuffer) / sizeof(Vertex);
	vec3 minimum = vec3(0, 0, 0);
	vec3 maximum = vec3(0, 0, 0);

	for (int i = 0; i < numOfVertices; i++) {
		minimum = vec3(std::min(vertexBuffer[i].position.x, minimum.x), std::min(vertexBuffer[i].position.y, minimum.y),
			std::min(vertexBuffer[i].position.z, minimum.z));
		maximum = vec3(std::max(vertexBuffer[i].position.x, maximum.x), std::max(vertexBuffer[i].position.y, maximum.y),
			std::max(vertexBuffer[i].position.z, maximum.z));
		//boundingSphereRadius = distance(minimum, maximum) / 2;
	}
	boundingSphereRadius = std::max(std::max(distance(minimum.x, maximum.x) / 2,
		distance(minimum.y, maximum.y) / 2), distance(minimum.z, maximum.z) / 2);

	
	// Create a vertex array
	glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
    
    
    
    // 1st attribute buffer : vertex Positions
    glVertexAttribPointer(    0,                // attribute. No particular reason for 0, but must match the layout in the shader.
                          3,                // size
                          GL_FLOAT,        // type
                          GL_FALSE,        // normalized?
                          sizeof(Vertex2), // stride
                          (void*)0        // array buffer offset
                          );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : vertex normal
    glVertexAttribPointer(    1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex2),
                          (void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
                          );
    glEnableVertexAttribArray(1);

    
    // 3rd attribute buffer : vertex color
    glVertexAttribPointer(    2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex2),
                          (void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
                          );
    glEnableVertexAttribArray(2);
	//4th attribute buffer : UV
	glVertexAttribPointer(3,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2),
		(void*)(3 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(3);

}

CubeModel::~CubeModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

void CubeModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second

	Model::Update(dt);
}

void CubeModel::Draw()
{
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();

	if (mTextureValid)
	{	
		Renderer::SetShader(SHADER_SKYBOX);
		glUseProgram(Renderer::GetShaderProgramID());
		Renderer::CheckForErrors();


		GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

		// Send the view projection constants to the shader
		const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
		mat4 VP = currentCamera->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		Renderer::CheckForErrors();

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		Renderer::CheckForErrors();
	}
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)
	Renderer::CheckForErrors();
	Renderer::SetShader(oldShader);
}

bool CubeModel::ParseLine(const std::vector<ci_string> &token)
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
