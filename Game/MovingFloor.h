#pragma once
#include "physics/PhysicsGhostObject.h"
class Player;
class MovingFloor:public IGameObject
{
public:
	MovingFloor() {};
	~MovingFloor();
	bool Start()override;
	void Update();
	void Move();
	void SetCollisionObj();
	void SetModel();
	void SetPhysicsStaticObj();
	void Render(RenderContext& rc);

	Vector3 m_firstPosition  = Vector3::Zero;            //初期位置。
	Vector3 m_position = Vector3::Zero;                  //現在地。
	Vector3 m_speed = Vector3::Zero;                     //移動速度。
	Vector3 m_movingSc = Vector3::One;                   //大きさ。
	
	Vector3 m_fontPos = Vector3(200.0f,100.0f,0.0f);     //フォントの座標。

	Quaternion m_movingFloorRotation = Quaternion::Identity;
	FontRender m_movingFontRender;

private:
	ModelRender m_movingFloorRender;
	PhysicsStaticObject m_physicsStaticObj;              //静的オブジェクト。
	CollisionObject* m_collisionObj = nullptr;           //コリジョンオブジェクト。
	Player* m_player = nullptr;                          //プレイヤー。

	//uint_fast8_tは良く分からない。
	enum enMovingFloorState : uint_fast8_t{
		enMovingFloorState_MovingForward,
		enMovingFloorState_MovingBack,
	};

	enMovingFloorState m_movingFloorState = enMovingFloorState_MovingForward;
};

