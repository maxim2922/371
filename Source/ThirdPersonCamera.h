#pragma once

#include "Camera.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(glm::vec3 position);
	virtual ~ThirdPersonCamera();
	virtual glm::mat4 GetViewMatrix() const;
	virtual void Update(float dt);

	glm::vec3 GetPosition() const { return mPosition; }
	float GetHorizontalAngle() const { return mHorizontalAngle; }
	float GetVerticalAngle() const { return mVerticalAngle; }
	float GetSpeed() const { return mSpeed; }
	glm::vec3 GetLookAt() const { return mLookAt; }
	glm::vec3 GetAngledPosition() const;
	void setPosition(glm::vec3 position);
	void setLookAt(glm::vec3 newLookAt);
	virtual void toggleMouse(bool b);
private:
	glm::vec3 mPosition;
	float mHorizontalAngle; // horizontal angle
	float mVerticalAngle;   // vertical angle

	float mSpeed;			// World units per second
	float mAngularSpeed;    // Degrees per pixel

	glm::vec3 mLookAt;
};
