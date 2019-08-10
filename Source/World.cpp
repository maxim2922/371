//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"
#include "UI_elements.hpp"

#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#include "BSpline.h"
#include "BSplineCamera.h"
#include <glm/gtx/transform.hpp>


using namespace std;
using namespace glm;

World* World::instance;
FirstPersonCamera* fp = new FirstPersonCamera(vec3(3.0f, 5.0f, 20.0f));


World::World()
{
    instance = this;

	// Setup Camera
	mCamera.push_back(fp);
	mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	mCurrentCamera = 0;

    
#if defined(PLATFORM_OSX)
//    int billboardTextureID = TextureLoader::LoadTexture("Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("Textures/Particle.png");
#else
//    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/Particle.png");
#endif
    assert(billboardTextureID != 0);

    mpBillboardList = new BillboardList(2048, billboardTextureID);

    
    
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		delete *it;
	}

	mAnimation.clear();

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		delete *it;
	}

	mAnimationKey.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();
    
    for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it < mParticleSystemList.end(); ++it)
    {
        delete *it;
    }
    mParticleSystemList.clear();
    
    for (vector<ParticleDescriptor*>::iterator it = mParticleDescriptorList.begin(); it < mParticleDescriptorList.end(); ++it)
    {
        delete *it;
    }
    mParticleDescriptorList.clear();

    
	delete mpBillboardList;
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{
	float dt2 = speed*dt;
	// Read mouse button. Toggle first person if RIGHT click is detected.
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		fp->toggleMouse(true);
	}
	else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		fp->toggleMouse(false);
	}
	
	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}

	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_SOLID_COLOR);
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_BLUE);
	}

    // Update animation and keys
    for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        (*it)->Update(dt2);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt2);
    }


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    
    // Update billboards
    
    for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it != mParticleSystemList.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    mpBillboardList->Update(dt);

}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 View = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &View[0][0]);

	GLuint ProjectionMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectionTransform");
	mat4 Projection = mCamera[mCurrentCamera]->GetProjectionMatrix();
	glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, &Projection[0][0]);

	// Get a handle for Light Attributes uniform
	GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPosition");
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");

	// Light Coefficients
	const vec3 lightColor(1.0f, 1.0f, 1.0f);
	const float lightKc = 0.05f;
	const float lightKl = 0.02f;
	const float lightKq = 0.002f;
	const vec4 lightPosition(0.0f, 0.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light
	//const vec4 lightPosition(0.0f, 10.0f, 20.0f, 0.0f); // If w = 0.0f, we have a directional light

	glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
	glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

	// Draw models

	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{

		(*it)->Draw();
	}

	for (vector<UI_elements*>::iterator it = mUI.begin(); it < mUI.end(); ++it)
	{

		(*it)->Draw();
	}




	


	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	//// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		(*it)->Draw();
	}

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		(*it)->Draw();
	}

    Renderer::CheckForErrors();

	/*mat4 identity(1.0f);
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &identity[0][0]);
	GLuint ViewProjectionLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 viewMatrix(1.0f);
	mat4 projectionMatrix = glm::ortho(0.0, 1024.0, 768.0, 0.0, -1.0, 1.0);
	mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(ViewProjectionLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);


	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		if ((*it)->isUI() == true)
			(*it)->Draw();
	}
*/
    
    // Draw Billboards
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mpBillboardList->Draw();
    glDisable(GL_BLEND);




	
/*
	GLfloat array[3];
	glReadPixels(EventManager::GetMousePositionX(), EventManager::GetMousePositionY(), 1, 1, GL_RGB, GL_FLOAT, array);*/


	///*	int count = 0;
	//	for (int i = 0; i < 3; i++) {
	//		if (array[i] >= 0)
	//			count++;
	//	}
	//	if (count >= 3)
	//		buttonState = 5;*/

	getButtonInteraction();




	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				cube->setUI(false);
				mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
				sphere->setUI(false);
                mModel.push_back(sphere);
            }
			else if ( result == "animationkey" )
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				key->setUI(false);
				mAnimationKey.push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				mAnimation.push_back(anim);
			}
			else if (result == "ui")
			{
				UI_elements* ui = new UI_elements();
				ui->Load(iss);
				ui->setUI(true);
				mUI.push_back(ui);
			}

			else if (result == "spline")
			{
				BSpline* spline = new BSpline();
				spline->Load(iss);
				spline->setUI(false);
				spline->CreateVertexBuffer();

				// FIXME: This is hardcoded: replace last camera with spline camera
				mSpline.push_back(spline);
				mCamera.pop_back();
				mCamera.push_back(new BSplineCamera(spline, 10.0f));
			}
            else if (result == "particledescriptor")
            {
                ParticleDescriptor* psd = new ParticleDescriptor();
                psd->Load(iss);
                AddParticleDescriptor(psd);
            }
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set Animation vertex buffers
	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
}

Animation* World::FindAnimation(ci_string animName)
{
    for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        if((*it)->GetName() == animName)
        {
            return *it;
        }
    }
    return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
    for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        if((*it)->GetName() == keyName)
        {
            return *it;
        }
    }
    return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
     return mCamera[mCurrentCamera];
}

void World::AddBillboard(Billboard* b)
{
    mpBillboardList->AddBillboard(b);
}

void World::RemoveBillboard(Billboard* b)
{
    mpBillboardList->RemoveBillboard(b);
}

void World::AddParticleSystem(ParticleSystem* particleSystem)
{
    mParticleSystemList.push_back(particleSystem);
}

void World::RemoveParticleSystem(ParticleSystem* particleSystem)
{
    vector<ParticleSystem*>::iterator it = std::find(mParticleSystemList.begin(), mParticleSystemList.end(), particleSystem);
    mParticleSystemList.erase(it);
}

void World::AddParticleDescriptor(ParticleDescriptor* particleDescriptor)
{
    mParticleDescriptorList.push_back(particleDescriptor);
}

ParticleDescriptor* World::FindParticleDescriptor(ci_string name)
{
    for(std::vector<ParticleDescriptor*>::iterator it = mParticleDescriptorList.begin(); it < mParticleDescriptorList.end(); ++it)
    {
        if((*it)->GetName() == name)
        {
            return *it;
        }
    }
    return nullptr;
}

void World::getButtonInteraction() {
	EventManager::GetMouseButton();
	if (EventManager::GetMousePositionX()<402
		&& EventManager::GetMousePositionX()>342 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 5;
	}
	if (EventManager::GetMousePositionX()<462
		&& EventManager::GetMousePositionX()>402 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 4;
	}
	if (EventManager::GetMousePositionX()<522
		&& EventManager::GetMousePositionX()>462 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 3;
	}

	if (EventManager::GetMousePositionX()<582
		&& EventManager::GetMousePositionX()>522 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 2;
	}

	if (EventManager::GetMousePositionX()<642
		&& EventManager::GetMousePositionX()>582 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 1;
	}

	if (EventManager::GetMousePositionX()<702
		&& EventManager::GetMousePositionX()>642 &&
		EventManager::GetMousePositionY()<60
		&& EventManager::GetMousePositionY()>20 &&
		EventManager::isClicked()) {
		buttonState = 0;
	}


	switch (buttonState) {
	case 0:
		speed = 10;
		break;
	case 1:
		speed = 1;
		break;
	case 2:
		speed = 0;
		break;

	case 3: // bird view
		mCurrentCamera = 0;
		break;
	case 4: // bird view
		mCurrentCamera = 1;
		break;
	case 5: // bird view
		mCurrentCamera = 2;
		break;

	default:
		speed = 1;
	}


}
