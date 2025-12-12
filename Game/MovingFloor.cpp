#include "stdafx.h"
#include "MovingFloor.h"
#include "Player.h"
#include "Game.h"

namespace {
	float SPEED = 300.0f;
	float LIMITED = 400.0f;
	Vector3 COLLISION_SIZE = Vector3(600.0f, 5.0f, 600.0f);
	Vector3 COLLISION_HEIGHT = Vector3(0.0f, 50.0f, 0.0f);
	const char* MOVINGFLOOR_INIT = "Assets/modelData/MovingFloor.tkm";
}

/// <summary>
/// デスストラクタ。
/// </summary>
MovingFloor::~MovingFloor()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
}
/// <summary>
/// 初期化処理。
/// </summary>
/// <returns></returns>
bool MovingFloor::Start()
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
void MovingFloor::SetModel()
{
	m_movingFloorRender.Init(MOVINGFLOOR_INIT);
	m_movingFloorRender.SetTRS(
		m_position,
		m_movingFloorRotation,
		m_movingSc
	);
	m_movingFloorRender.Update();
}

/// <summary>
/// 静的物理オブジェクトの初期化。
/// </summary>
void MovingFloor::SetPhysicsStaticObj()
{
	//静的オブジェクトの初期化。
	//剛体を破棄する。
	m_physicsStaticObj.Release();
	m_physicsStaticObj.CreateFromModel(
		m_movingFloorRender.GetModel(),
		m_movingFloorRender.GetModel().GetWorldMatrix()
	);
}

/// <summary>
/// コリジョンの初期化。
/// </summary>
void MovingFloor::SetCollisionObj()
{
	//コリジョンを消して作り直す。
	delete m_collisionObj;
	m_collisionObj = new CollisionObject;

	//初期位置 + 高さ分。
	m_position = m_firstPosition + COLLISION_HEIGHT;
	m_collisionObj->CreateBox(
		m_position,
		Quaternion::Identity,
		COLLISION_SIZE
	);
	m_collisionObj->SetPosition(m_position);
	m_collisionObj->SetRotation(m_movingFloorRotation);
	m_collisionObj->Update();
}

/// <summary>
/// 更新処理。
/// </summary>
void MovingFloor::Update()
{
	const bool& isPause = Game::GetIsPause();
	if (isPause) {
		return;
	}
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
		if(!m_player) return;
	}
	
	Vector3 Pos = m_speed;
	Move();
	SetPhysicsStaticObj();
	//床がどれだけ動いたかの計算。
	Vector3 moveDelta = m_speed - Pos;

	//コリジョンオブジェクトとプレイヤーのキャラクターコントローラーが
	////衝突したら(キャラクターが動く床の上に乗ったら)。
	if (!m_collisionObj && !m_player) {
		return;
	}

	if (m_collisionObj && m_player)
	{
		if (m_collisionObj->IsHit(m_player->GetCharacterController() ))
		{
			//動く床の移動速度をキャラクターの移動速度に加算。
			m_player->AddPosition(moveDelta);
		}
	}

	//デバック表示(動く座標)。
	wchar_t movingfont[256];
	Vector3 fontPos = Vector3(m_position);
	swprintf_s(movingfont, 256, L"PosX=%.1f,Y=%.1f,Z=%.1f", fontPos.x, fontPos.y, fontPos.z);
	m_movingFontRender.SetText(movingfont);
	m_movingFontRender.SetPosition(m_fontPos);

	m_movingFloorRender.Update();
	m_collisionObj->SetPosition(m_speed);
	m_movingFloorRender.SetPosition(m_speed);
}

/// <summary>
/// 移動処理。
/// </summary>
void MovingFloor::Move()
{
	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	Vector3 moveDir = Vector3::Zero;
	//動く床を前,後ろ方向に動かす。
	if (m_movingFloorState == enMovingFloorState_MovingForward)
	{
		moveDir.z += -SPEED;
	}
	else if (m_movingFloorState == enMovingFloorState_MovingBack)
	{
		moveDir.z += SPEED;
	}

	m_speed += moveDir * deltaTime;
	//範囲制限。
	if (m_movingFloorState == enMovingFloorState_MovingForward && 
		m_speed.z <= m_firstPosition.z - LIMITED)
	{
		m_movingFloorState = enMovingFloorState_MovingBack;	
	}

	else if (m_movingFloorState == enMovingFloorState_MovingBack && 
		m_speed.z >= m_firstPosition.z + LIMITED)
	{
		m_movingFloorState = enMovingFloorState_MovingForward;
	}

	
	//コリジョンに移動速度を設定。
	m_collisionObj->SetPosition(m_speed);
	//モデルに移動速度を設定。
	m_movingFloorRender.SetPosition(m_speed);
}

/// <summary>
/// 描画処理。
/// </summary>
/// <param name="rc"></param>
void MovingFloor::Render(RenderContext& rc)
{
	//動く床の描画。
	m_movingFloorRender.Draw(rc);

	//動く床のフォント座標。
	//m_movingFontRender.Draw(rc);
}