//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 position);
	virtual ~FirstPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;
	virtual void toggleMouse(bool b);
	glm::vec3 GetPosition() const { return mPosition; }
	glm::vec3 GetAngledPosition() { return glm::vec3(0.0f); }
	void setPosition(glm::vec3 position);
	void setLookAt(glm::vec3 newLookAt);
private:
	glm::vec3 mPosition;	
	float mHorizontalAngle; // horizontal angle
	float mVerticalAngle;   // vertical angle

	float mSpeed;			// World units per second
	float mAngularSpeed;    // Degrees per pixel

	glm::vec3 mLookAt;
};
