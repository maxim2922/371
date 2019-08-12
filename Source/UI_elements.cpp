//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "UI_elements.hpp"
#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include "TextureLoader.h"

using namespace glm;

UI_elements::UI_elements(vec3 size) : Model()
{
    // Create Vertex Buffer for all the verices of the Cube
    vec3 halfSize = size * 100.f;

    vec3 center = vec3(0);
    
   
    
    std::vector <Vertex2> vertexBuffer2 = {  // position,   normal,   color,     uv


        
        { vec3(342, 20, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)}, //1
        { vec3(342,60, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) }, //2
        { vec3( 382,60, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f),vec2(1.0f, 0.0f) }, //3
        
        { vec3(382,20, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f) }, //4
        { vec3(342,20, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) }, //5
        { vec3(382,60, center.z), vec3( 0.0f, 0.0f, 1.0f),vec3(0.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },//6
        

    };
        numOfVertices = sizeof(vertexBuffer2) / sizeof(Vertex);

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2)*vertexBuffer2.size(), vertexBuffer2.data(), GL_STATIC_DRAW);

		// 1st attribute buffer : vertex Positions
		glVertexAttribPointer(0,              // attribute. No particular reason for 0, but must match the layout in the shader.
			3,              // size
			GL_FLOAT,       // type
			GL_FALSE,       // normalized?
			sizeof(Vertex2), // stride
			(void*)0        // array buffer offset
		);
		glEnableVertexAttribArray(0);

		// 2nd attribute buffer : vertex normal
		glVertexAttribPointer(1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex2),
			(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);
		glEnableVertexAttribArray(1);


		// 3rd attribute buffer : vertex color
		glVertexAttribPointer(2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex2),
			(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
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





UI_elements::~UI_elements()
{
    // Free the GPU from the Vertex Buffer
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}

void UI_elements::Update(float dt)
{
    // If you are curious, un-comment this line to have spinning cubes!
    // That will only work if your world transform is correct...
    // mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second

    
 
   Model::Update(dt);
}

void UI_elements::Draw()
{
    // Draw the Vertex Buffer
    // Note this draws a unit Cube
    // The Model View Projection transforms are computed in the Vertex Shader
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();

	if (mTextureValid)
	{
		Renderer::SetShader(SHADER_SKYBOX);
		glUseProgram(Renderer::GetShaderProgramID());
		Renderer::CheckForErrors();
		GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	mat4 identity(1.0f);
    GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &identity[0][0]);

    GLuint ViewProjectionLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 viewMatrix = translate(mat4(1.0f), mPosition);
    mat4 projectionMatrix = glm::ortho(0.0, 1024.0, 768.0, 0.0, -1.0, 1.0);
    mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
    // Draw TRIANGLE #1
    glDrawArrays(GL_TRIANGLES, 0, 6); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)
	Renderer::SetShader(oldShader);
    
 //   viewMatrix= translate(mat4(1.0f), vec3(60.0f,0.0f,0.0f));
 //   viewProjectionMatrix = projectionMatrix * viewMatrix;
 //   glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]); 
 //      //  Draw TRIANGLE #2
 //   glDrawArrays(GL_TRIANGLES, 0, 6);
 //   
 //   
 //   viewMatrix= translate(mat4(1.0f), vec3(120.0f,0.0f,0.0f));
 //   viewProjectionMatrix = projectionMatrix * viewMatrix;
 //   glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
 //       // Draw TRIANGLE #3
 //   glDrawArrays(GL_TRIANGLES, 0, 6);

 //   
 //   viewMatrix= translate(mat4(1.0f), vec3(180.0f,0.0f,0.0f));
 //   viewProjectionMatrix = projectionMatrix * viewMatrix;
 //   glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
 //       // Draw TRIANGLE #4
 //   glDrawArrays(GL_TRIANGLES, 0, 6);
 //   
 //   viewMatrix= translate(mat4(1.0f), vec3(240.0f,0.0f,0.0f));
 //   viewProjectionMatrix = projectionMatrix * viewMatrix;
 //   glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
 //   // Draw TRIANGLE #5
 //   glDrawArrays(GL_TRIANGLES, 0, 6);

	//viewMatrix = translate(mat4(1.0f), vec3(300.0f, 0.0f, 0.0f));
	//viewProjectionMatrix = projectionMatrix * viewMatrix;
	//glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
	//// Draw TRIANGLE #6
	//glDrawArrays(GL_TRIANGLES, 0, 6);
 // 
}

bool UI_elements::ParseLine(const std::vector<ci_string> &token)
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

void UI_elements::setTexture(const char * imagepath) {
	mTextureID = TextureLoader::LoadTexture(imagepath);
}

