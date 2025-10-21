#pragma once

#include "physics/PhysicsGhostObject.h"

class Warp : public IGameObject
{
public:
	bool Start()override;

	PhysicsGhostObject m_physicsGhostObj;
};

