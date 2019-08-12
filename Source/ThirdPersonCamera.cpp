


#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.inl>
#include "World.h"

using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 position) : Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(4.0f), mAngularSpeed(5.0f)
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float dt)
{
	EventManager::DisableMouseCursor();

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

	mLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);

	// A S D W for motion along the spaceship's basis vectors
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		mPosition += mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		mPosition -= mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		mPosition += sideVector * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		mPosition -= sideVector * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mPosition += dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mPosition -= dt * mSpeed;
	}
}

mat4 ThirdPersonCamera::GetViewMatrix() const
{
	float radius = 5.0f;
	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);


	vec3 position = mPosition -
		vec3(radius * cosf(phi) * cosf(theta),
			radius * sinf(phi),
			-radius * cosf(phi) * sinf(theta)
		);

	return lookAt(position, mPosition, vec3(0.0f, 1.0f, 0.0f));
}