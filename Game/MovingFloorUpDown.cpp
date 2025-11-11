#include "stdafx.h"
#include "MovingFloorUpDown.h"
#include "Player.h"

namespace {
	const float UP_SPEED = 280.0f;
	const float LIMITED  = 400.0f;
	const Vector3 COLLISION_SIZE = { 365.0f,6.0f,220.0f }; //コリジョンの大きさ。
	const Vector3 COLLISION_HEIGHT = { 0.0f,50.0f,0.0f };    //コリジョンの高さ。
	const char* MOVING_UPDOWN = "Assets/modelData/MovingFloorUpDown.tkm";
}

MovingFloorUpDown::~MovingFloorUpDown()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
}

bool MovingFloorUpDown::Start()
{
	SetModel();
	SetPhysicsStaticObj();
	SetCollisionObj();

	m_player = FindGO<Player>("player");
	return true;
}

/// <summary>
/// モデルの設定。
/// </summary>
void MovingFloorUpDown::SetModel()
{
	m_movingFloorUpDownRender.Init(MOVING_UPDOWN);
	m_movingFloorUpDownRender.SetTRS(
		m_movingPos,
		m_movingRot,
		m_movingScale
	);
	m_movingFloorUpDownRender.Update();
}

/// <summary>
/// 静的物理オブジェクトの設定。
/// </summary>
void MovingFloorUpDown::SetPhysicsStaticObj()
{
	//静的オブジェクトの初期化。
	//剛体を破棄する。
	//再作成する。
	m_physicsStaticObj.Release();
	m_physicsStaticObj.CreateFromModel(
		m_movingFloorUpDownRender.GetModel(),
		m_movingFloorUpDownRender.GetModel().GetWorldMatrix()
	);
}

/// <summary>
/// コリジョンの設定。
/// </summary>
void MovingFloorUpDown::SetCollisionObj()
{
	//コリジョンを消して再作成する。
	delete m_collisionObj;
	m_collisionObj = new CollisionObject;
	Vector3 colP = Vector3::Zero;
	m_collisionObj->CreateBox(
		colP,
		m_movingRot,
		COLLISION_SIZE
	);
	//コリジョンを自動で消すかどうかの判定をとる。
	m_collisionObj->SetIsEnableAutoDelete(false);
	m_collisionObj->SetPosition(colP);
	m_collisionObj->SetRotation(m_movingRot);
	m_collisionObj->Update();
}

void MovingFloorUpDown::Update()
{
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
		return;
	}

	Move();
	SetPhysicsStaticObj();
	m_movingFloorUpDownRender.Update();
}

void MovingFloorUpDown::Move()
{
	const float distance = g_gameTime->GetFrameDeltaTime();
	Vector3 moveDir = Vector3::Zero;

	if (movingSpeedState == enMoving_Up)
	{
		moveDir.y += -UP_SPEED;
	}
	else if (movingSpeedState == enMoving_Down)
	{
		moveDir.y += UP_SPEED;
	}
	
	m_movingSpeed += moveDir * distance;
	//範囲制限での移動距離。
	if (movingSpeedState == enMoving_Down && 
		m_movingSpeed.y >= m_firstPos.y + LIMITED)
	{
		movingSpeedState = enMoving_Up;
	}
	else if (movingSpeedState == enMoving_Up &&
		m_movingSpeed.y <= m_firstPos.y - LIMITED)
	{
		movingSpeedState = enMoving_Down;
	}
	//コリジョンに移動速度を設定する。
	m_collisionObj->SetPosition(m_movingSpeed);
	//モデルに移動速度を設定する。
	m_movingFloorUpDownRender.SetPosition(m_movingSpeed);
}

void MovingFloorUpDown::Render(RenderContext& rc)
{
	m_movingFloorUpDownRender.Draw(rc);
}