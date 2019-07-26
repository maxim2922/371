//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "ParticleEmitter.h"
#include "Model.h"

using namespace glm;

ParticleEmitter::ParticleEmitter(glm::vec3 position, const Model* parent)
: mpParent(parent), mPosition(position)
{
}


// This is a point emitter, nothing is random
// As a small extra task, you could create derived classes
// for more interesting emitters such as line emitters or circular emitters
// In these cases, you would sample a random point on these shapes
glm::vec3 ParticleEmitter::GetPosition()
{
    // @TODO 8 - Position from Parented Emitter
    //
    // Return the position where the particle is emitted.
    // If the emitter is parented, the position is relative to its parent

	glm::vec4 position = vec4(mPosition, 1.0f);

	/*if (mpParent != NULL)
	{
		mPosition = vec4(mpParent->GetPosition(),1);
	}*/

    

	position = mpParent->GetWorldMatrix()*position;
	//vec3 v2 = vec3(position);
	//mpParent->SetPosition(v2);
    
	return vec3(position);
}




