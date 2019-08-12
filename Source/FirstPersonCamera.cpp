//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;
bool freeWill = false;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position) :  Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(2.5f)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update(float dt)
{	
	if (freeWill) {
		// Prevent from having the camera move only when the cursor is within the windows
		EventManager::DisableMouseCursor();


		// The Camera moves based on the User inputs
		// - You can access the mouse motion with EventManager::GetMouseMotionXY()
		// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
		// - Don't forget to use dt to control the speed of the camera motion

		// Mouse motion to get the variation in angle
		mHorizontalAngle -= EventManager::GetMouseMotionX() * mAngularSpeed * dt;
		mVerticalAngle -= EventManager::GetMouseMotionY() * mAngularSpeed * dt;

		// Clamp vertical angle to [-85, 85] degrees
		mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
		if (mHorizontalAngle > 360)
		{
			mHorizontalAngle -= 360;
		}
		else if (mHorizontalAngle < -360)
		{
			mHorizontalAngle += 360;
		}

		float theta = radians(mHorizontalAngle);
		float phi = radians(mVerticalAngle);

		mLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));

		vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
		glm::normalize(sideVector);

		// A S D W for motion along the camera basis vectors
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			mPosition += mLookAt * dt * (mSpeed + (length(mPosition)*0.2f));
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			mPosition -= mLookAt * dt * (mSpeed + (length(mPosition)*0.2f));
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			mPosition += sideVector * dt * (mSpeed + (length(mPosition)*0.2f));
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			mPosition -= sideVector * dt * (mSpeed + (length(mPosition)*0.2f));
		}
	}
	else {
		EventManager::EnableMouseCursor();

		// Default W S D A commands. 
		// TODO
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			mPosition += dt * mSpeed * vec3(0.0f, 0.0f, -1.0f);
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			mPosition -= dt * mSpeed * vec3(0.0f, 0.0f, -1.0f);
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			mPosition += dt * mSpeed* vec3(1.0f, 0.0f, 0.0f);
		}

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			mPosition -= dt * mSpeed* vec3(1.0f, 0.0f, 0.0f);
		}
	}
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}

void FirstPersonCamera::toggleMouse(bool value) {
	freeWill = value;
}

void FirstPersonCamera::setPosition(vec3 position) {
	mPosition = position;
}

void FirstPersonCamera::setLookAt(vec3 newLookAt) {
	mLookAt = newLookAt;
}