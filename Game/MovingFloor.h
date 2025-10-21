#pragma once
#include "physics/PhysicsGhostObject.h"
class Player;
class MovingFloor:public IGameObject
{
public:
	MovingFloor();
	~MovingFloor();
	bool Start()override;
	void Update();

	void Render(RenderContext& rc);

	Vector3 m_movingPosition = Vector3::Zero;
	Vector3 m_firstPosition = Vector3::Zero;
	Vector3 m_setPos = Vector3(300.0f, 600.0f, 0.0f);
private:
	void Move();

	ModelRender m_movingFloorRender;
	PhysicsStaticObject m_physicsStaticObj;//静的オブジェクト。
	CollisionObject* m_collisionObj = nullptr;//コリジョンオブジェクト。
	Player* m_player = nullptr;//プレイヤー。

	enum enMovingFloorState {
		enMovingFloorState_MovingForward,
		enMovingFloorState_MovingBack,
	};

	enMovingFloorState m_movingFloorState = enMovingFloorState_MovingForward;
};

