//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update(float dt) = 0;

	virtual glm::vec3 GetPosition() const { return glm::vec3(0.0f); }
	virtual float GetHorizontalAngle() const { return 0; }
	virtual float GetVerticalAngle() const { return 0; }
	virtual float GetSpeed() const { return 0; }
	virtual glm::vec3 GetLookAt() const { return glm::vec3(0.0f); }
	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;
	virtual glm::vec3 GetAngledPosition() const { return glm::vec3(0.0f); }
	virtual void setPosition(glm::vec3 position) {}
private:

};
