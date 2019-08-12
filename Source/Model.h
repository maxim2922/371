//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"

#include <vector>

#include <glm/glm.hpp>

class Animation;

class Model
{
public:
	Model(const Model* parent = nullptr);
	virtual ~Model();

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

	void Load(ci_istringstream& iss);

	virtual glm::mat4 GetWorldMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angleDegrees);
	void SetVelocity(glm::vec3 newVelocity);
	void SetMaterialCoefficients(glm::vec4 material);
	virtual bool IntersectsRay(glm::vec3 source, glm::vec3 direction) { return false; }


	glm::vec3 GetPosition() const		{ return mPosition; }
	glm::vec3 GetScaling() const		{ return mScaling; }
	glm::vec3 GetRotationAxis() const	{ return mRotationAxis; }
	float     GetRotationAngle() const	{ return mRotationAngleInDegrees; }
    ci_string GetName()                 { return mName; }
	glm::vec4 GetMaterialCoefficients() const { return materialCoefficients; }
	glm::vec3 GetVelocity() const { return mVelocity; }
	float GetRadius() const { return boundingSphereRadius; }
	float GetMass() const { return mass; }

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

	ci_string mName; // The model name is mainly for debugging
	glm::vec3 mPosition;
	glm::vec3 mScaling;
	glm::vec3 mRotationAxis;
	float     mRotationAngleInDegrees;
	glm::vec4 materialCoefficients;
	glm::vec3 mVelocity = glm::vec3(0, 0, 0);
	float boundingSphereRadius = 0;
	float mass = 1.0f;
	const Model* mParent;

    // Makes the model follow a list of Animation Keys so it's world transform changes over time
    Animation* mAnimation;

	int mTextureID;
	bool mTextureValid = false;

    friend class Animation;
};
