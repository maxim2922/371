//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Animation.h"
#include "Renderer.h"
#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;
using namespace std;

AnimationKey::AnimationKey() : Model()
{
}

AnimationKey::~AnimationKey()
{
}

void AnimationKey::Update(float dt)
{
	Model::Update(dt);
}

void AnimationKey::Draw()
{
	Model::Draw();
}

bool AnimationKey::ParseLine(const std::vector<ci_string> &token)
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

Animation::Animation() 
	: mName(""), mCurrentTime(0.0f), mDuration(0.0f), mVBO(0), mVAO(0)
{
}

Animation::~Animation()
{
}

void Animation::CreateVertexBuffer()
{
	/*
    // This is just to display lines between the animation keys
    for (int i=0; i<(int)mKey.size(); ++i)
    {
        Vertex v;
        v.position = mKey[i].mPosition;
        mVertexBuffer.push_back(v);
    }
    
	// Create a vertex array
	glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mVertexBuffer.size(), &(mVertexBuffer[0]), GL_STATIC_DRAW);
    
    // Create a vertex array
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mVertexBuffer.size(), &(mVertexBuffer[0]), GL_STATIC_DRAW);
    
    // 1st attribute buffer : vertex Positions
    glVertexAttribPointer(0,
                          3,                // size
                          GL_FLOAT,        // type
                          GL_FALSE,        // normalized?
                          sizeof(Vertex), // stride
                          (void*)0        // array buffer offset
                          );
    glEnableVertexAttribArray(0);*/
	for (int i = 0; i < mKey.size() - 1; ++i)
	{
		spline.AddControlPoint(mKey[i].GetPosition());
	}

	spline.CreateVertexBuffer();

}

void Animation::Update(float dt)
{
	mCurrentTime += dt;

    while(mCurrentTime > mDuration)
    {
        mCurrentTime -= mDuration;
    }
}

void Animation::Draw()
{
	// @TODO 3 - Draw lines between animation key positions
	//
	// Steps:
	// - Draw the Vertex Buffer
	// - The Shader used to draw the Animation is PathLines, only requires to send the positions of the key positions
	// - You need to bind the vertex buffer created in CreateVertexBuffer() as the input of the vertex shader
	// - Also set the WorldTransform uniform variable in the vertex shader
	// - You Should use GL_LINE_LOOP as a primitive to draw lines (not triangles)
	// - You can look at CubeModel::Draw() for a good example!
		//
		// Notes:
		// The shader is bound in World.cpp and the ViewProjection Matrix uniform is set there...
	// The Model View Projection transforms are computed in the Vertex Shader
	/*
	glm::mat4 m = glm::mat4(1.0f);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &m[0][0]);

	// Draw the triangles !
	glDrawArrays(GL_LINE_LOOP, 0, mKey.size());*/
	spline.Draw();
}


void Animation::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}


bool Animation::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty())
    {
        return true;
    }
	else if (token[0] == "name")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		mName = token[2];
		return true;
	}
	else if (token[0] == "key")
	{
		assert(token.size() > 5);
        assert(token[1] == "=");
        assert(token[3] == "time");
        assert(token[4] == "=");

		ci_string name = token[2];
		AnimationKey *key = World::GetInstance()->FindAnimationKey(name);
        
		assert(key != nullptr);
		AddKey(key, (float) atof(token[5].c_str()));
		return true;
	}
	return false;
}

void Animation::AddKey(AnimationKey* key, float time)
{
    assert(time >= mDuration);
	mKey.push_back(*key);
    mKeyTime.push_back(time);
    mDuration = time;
}

ci_string Animation::GetName() const
{
	return mName;
}

glm::mat4 Animation::GetAnimationWorldMatrix() const
{
	// @TODO 4 - Find the 2 keys to interpolate the transformation (before and after current time)
	//           Interpolate the position, scaling and rotation separately
	//           Finally concatenate the interpolated transforms into a single
	//           world transform and return it.

	//return mat4(1.0f);

	int t;
	for (int i = 0; i < (mKey.size() - 1); i++)
	{
		if (mCurrentTime > mKeyTime[i] && mCurrentTime < mKeyTime[i + 1])
			t = i;
	}



	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::mix(mKey[t].GetScaling(), mKey[t + 1].GetScaling(), (mCurrentTime - mKeyTime[t]) / (mKeyTime[t + 1] - mKeyTime[t])));

	quat q1 = glm::angleAxis(radians(mKey[t].GetRotationAngle()), mKey[t].GetRotationAxis());
	quat q2 = glm::angleAxis(radians(mKey[t + 1].GetRotationAngle()), mKey[t + 1].GetRotationAxis());
	quat q3 = glm::slerp(q1, q2, (mCurrentTime - mKeyTime[t]) / (mKeyTime[t + 1] - mKeyTime[t]));


	glm::mat4 rotationMatrix = glm::toMat4(q3);

	


	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::mix(mKey[t].GetPosition(), mKey[t + 1].GetPosition(), (mCurrentTime - mKeyTime[t]) / (mKeyTime[t + 1] - mKeyTime[t])));




	mat4 worldMatrix(1.0f);


	return translationMatrix * rotationMatrix * scalingMatrix;
}
