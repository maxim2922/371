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
#include "ThirdPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "SpaceshipModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"

#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#include "BSpline.h"
#include "BSplineCamera.h"
#include "AsteroidModel.h"

using namespace std;
using namespace glm;

World* World::instance;
FirstPersonCamera* fp = new FirstPersonCamera(vec3(3.0f, 5.0f, 20.0f));

World::World()
{
    instance = this;

	// Setup Camera
	mCamera.push_back(fp);
	mCamera.push_back(new ThirdPersonCamera(vec3(3.0f, 5.0f, 20.0f)));
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

	for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		delete *it;
	}
	mSpline.clear();
    
	delete mpBillboardList;
}

World* World::GetInstance()
{
    return instance;
}
int lastMouseButtonState = GLFW_RELEASE;
void World::Update(float dt)
{
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
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
	{
		if (mCamera.size() > 3)
		{
			mCurrentCamera = 3;
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
        (*it)->Update(dt);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt);
    }


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);
	if (lastMouseButtonState == GLFW_RELEASE && glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		mModel.push_back(new AsteroidModel(fp->GetPosition() + vec3(0, 0, -0.1f), -inverse(fp->GetViewMatrix())[2], vec3(0.1f, 0.1f, 0.1f)));
	}
	lastMouseButtonState = glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT);

	//Check collisions
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{

		//Intersphere collisions
		//complexity: O(n^2)
		for (vector<Model*>::iterator it2 = it; it2 < mModel.end(); ++it2)
		{

			//Models can't collide with themselves, and both models cant be projectiles
			if (it != it2)
			{
				Model* m1 = *it;
				Model* m2 = *it2;

				float distance = glm::distance(m1->GetPosition(), m2->GetPosition());
				float r1 = m1->GetRadius();
				float r2 = m2->GetRadius();
				float totalRadii = r1 + r2;

				//TODO 2 - Collisions between Models

				if (distance <= totalRadii) //Collision
				{
					glm::vec3 collisionNormal = glm::normalize(m1->GetPosition() - m2->GetPosition());
					glm::vec3 collisionPoint = m2->GetPosition() + r2 * collisionNormal;
					glm::vec3 normalVelocity1 = glm::dot(m1->GetVelocity(), collisionNormal) * collisionNormal;
					glm::vec3 normalVelocity2 = glm::dot(m2->GetVelocity(), collisionNormal) * collisionNormal;

					glm::vec3 tangentMomentum1 = m1->GetVelocity() - normalVelocity1;
					glm::vec3 tangentMomentum2 = m2->GetVelocity() - normalVelocity2;

					float mass1 = m1->GetMass();
					float mass2 = m2->GetMass();
					glm::vec3 newNormalVelocity1 = ((mass1 - mass2) / (mass1 + mass2)) * normalVelocity1 + ((2 * mass2) / (mass1 + mass2) * normalVelocity2);
					glm::vec3 newNormalVelocity2 = ((2 * mass1) / (mass1 + mass2)) * normalVelocity1 + ((mass2 - mass1) / (mass1 + mass2) * normalVelocity2);

					m1->SetVelocity(newNormalVelocity1 + tangentMomentum1);
					m2->SetVelocity(newNormalVelocity2 + tangentMomentum2);

				}


			}
		}
	}

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

	// Draw Path Lines
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
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
    
    // Draw Billboards
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mpBillboardList->Draw();
    glDisable(GL_BLEND);

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
				mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            }
			else if (result == "asteroid")
			{
				AsteroidModel* asteroid = new AsteroidModel();
				asteroid->Load(iss);
				mModel.push_back(asteroid);
			}
			else if (result == "spaceship")
			{
				SpaceshipModel* spaceship = new SpaceshipModel();
				spaceship->Load(iss);
				mModel.push_back(spaceship);
			}
			else if ( result == "animationkey" )
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				mAnimationKey.push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				mAnimation.push_back(anim);
			}
			else if (result == "spline")
			{
				BSpline* spline = new BSpline();
				spline->Load(iss);
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
