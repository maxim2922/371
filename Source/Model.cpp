//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Model.h"
#include "Animation.h"
#include "World.h"
#include "ParticleEmitter.h"
#include "ParticleDescriptor.h"
#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <string>
#include "TextureLoader.h"
#include "EventManager.h"

using namespace std;
using namespace glm;

Model::Model(const Model* parent)
: mName("UNNAMED"), mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(0.0f, 1.0f, 0.0f),
  mHorizontalRotationAngleInDegrees(0.0f), mAnimation(nullptr), mParent(parent)
{
}

Model::~Model()
{
}

void Model::Update(float dt)
{
	mPosition += mVelocity * dt;
}

void Model::Draw()
{
}


void Model::Load(ci_istringstream& iss)
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

bool Model::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty() == false)
	{
		if (token[0].empty() == false && token[0][0] == '#')
		{
			return true;
		}
		else if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			mName = token[2];	
		}
		else if (token[0] == "position")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mPosition.x = static_cast<float>(atof(token[2].c_str()));
			mPosition.y = static_cast<float>(atof(token[3].c_str()));
			mPosition.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "rotation")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mRotationAxis.x = static_cast<float>(atof(token[2].c_str()));
			mRotationAxis.y = static_cast<float>(atof(token[3].c_str()));
			mRotationAxis.z = static_cast<float>(atof(token[4].c_str()));
			mHorizontalRotationAngleInDegrees = static_cast<float>(atof(token[5].c_str()));

			glm::normalize(mRotationAxis);
		}
		else if (token[0] == "scaling")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mScaling.x = static_cast<float>(atof(token[2].c_str()));
			mScaling.y = static_cast<float>(atof(token[3].c_str()));
			mScaling.z = static_cast<float>(atof(token[4].c_str()));
			boundingSphereRadius *= std::max(std::max(mScaling.x, mScaling.y), mScaling.z);
		}
		else if (token[0] == "animation")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string animName = token[2];
            
            mAnimation = World::GetInstance()->FindAnimation(animName);
		}
		else if (token[0] == "texturePath")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");
			ci_string animName = token[2].substr(1, token[2].length() - 2);

			mTextureID = TextureLoader::LoadTexture(animName.c_str());

			//mTextureID = TextureLoader::LoadTexture("../Assets/Textures/Particle.jpg");
			//mTextureID = TextureLoader::LoadTexture(token[2].c_str());
			mTextureValid = true;
		}
        else if (token[0] == "particleemitter")
        {
            assert(token.size() > 2);
            assert(token[1] == "=");

            ParticleDescriptor* desc = World::GetInstance()->FindParticleDescriptor(token[2]);
            assert(desc != nullptr);
            
            ParticleEmitter* emitter = new ParticleEmitter(vec3(0.0f, 0.0f, 0.0f), this);
            
            ParticleSystem* ps = new ParticleSystem(emitter, desc);
            World::GetInstance()->AddParticleSystem(ps);
        }
		else if (token[0] == "parent")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string parentName = token[2];

			mParent = World::GetInstance()->FindParentModel(parentName);
		}
		else if (token[0] == "materialCoefficients")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			materialCoefficients.x = static_cast<float>(atof(token[2].c_str()));
			materialCoefficients.y = static_cast<float>(atof(token[3].c_str()));
			materialCoefficients.z = static_cast<float>(atof(token[4].c_str()));
			materialCoefficients.w = static_cast<float>(atof(token[5].c_str()));
		}
		else if (token[0] == "velocity")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mVelocity.x = static_cast<float>(atof(token[2].c_str()));
			mVelocity.y = static_cast<float>(atof(token[3].c_str()));
			mVelocity.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "mass")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			mass = static_cast<float>(atof(token[2].c_str()));
		}

		else
		{
			return false;
		}
	}

	return true;
}

glm::mat4 Model::GetWorldMatrix() const
{
	// @TODO 2 - You must build the world matrix from the position, scaling and rotation informations
    //           If the model has an animation, get the world transform from the animation.
	mat4 worldMatrix(1.0f);
	mat4 parentMatrix(1.0f);
	mat4 tr(1.0f);
	mat4 r(1.0f);
	mat4 s = glm::scale(mat4(1.0f), mScaling);
    // Solution TRS
#if 1
    if (mAnimation)
   {
        // In this case, TR has both a rotation and translation matrix
		 tr = mAnimation->GetAnimationWorldMatrix();
    }
	else
	{
		// Here, just a translation
		 tr = glm::translate(mat4(1.0f), mPosition);
	}
	if (mParent != NULL)
	{
		parentMatrix = mParent->GetWorldMatrix();
	}
#endif
	r = glm::rotate(mat4(1.0f), glm::radians(mHorizontalRotationAngleInDegrees), mRotationAxis);
	worldMatrix = tr * r * s;

	return parentMatrix * worldMatrix;
}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	mScaling = scaling;
}

void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	mRotationAxis = axis;
	mHorizontalRotationAngleInDegrees = angleDegrees;
}

void Model::SetVelocity(glm::vec3 newVelocity) {
	mVelocity = newVelocity;
}

void Model::SetMaterialCoefficients(glm::vec4 material) {
	materialCoefficients = material;
}