#include "stdafx.h"
#include "FallBox.h"
#include "Player.h"


namespace {
	const char* FALL_BOX_FILE_PATH = "Assets/modelData/FallBox.tkm";
	const Vector3 COLLISION_FALL_SIZE = Vector3::One * 100.0f;
}


FallBox::FallBox()
{
}

FallBox::~FallBox()
{
}

bool FallBox::Start()
{
	//初期位置の保存。
	m_initPos = m_fallPos;

	m_fallBoxRender.Init(FALL_BOX_FILE_PATH);
	m_fallBoxRender.SetPosition(m_fallPos);
	m_fallBoxRender.SetRotation(m_fallRot);
	m_fallBoxRender.SetScale(m_fallScale);
	m_fallBoxRender.Update();

	m_fallPhysicsObj.CreateFromModel(
		m_fallBoxRender.GetModel(),
		m_fallBoxRender.GetModel().GetWorldMatrix()
	);

	SetCollision();
	m_player = FindGO<Player>("player");
	return true;
}

void FallBox::Update()
{
	FallMove();
	m_fallBoxRender.Update();
}


void FallBox::SetCollision()
{
	m_fallCollisionObj = new CollisionObject;
	m_fallCollisionObj->CreateBox(
		m_fallPos,
		m_fallRot,
		COLLISION_FALL_SIZE
	);
	m_fallCollisionObj->SetPosition(m_fallPos);
	m_fallCollisionObj->SetRotation(m_fallRot);
	m_fallCollisionObj->Update();
}


bool FallBox::FallMove()
{
	if (!m_player)return false;
	if (m_player->GetCollisionObj()->IsHit(m_fallCollisionObj)) {
		m_isPlayerOn = true;
	}

	float deltaTime = g_gameTime->GetFrameDeltaTime();

	switch (m_fall) {
	case FallStage::enFall_Idle:
		if (m_isPlayerOn) {
			m_fallTimer += deltaTime;
			if (m_fallTimer > 0.5f) {
				m_fall = FallStage::enFall;
				m_fallTimer = 0.0f;
			}
		}
		break;

	case FallStage::enFall:
		/** 落下処理 */
		m_fallPos.y -= m_sinkSpeed * deltaTime;
		/** 一定の高さまで行くと初期位置に */
		if (m_fallPos.y < m_initPos.y - 500.0f) {
			m_stayTimer += deltaTime;
			if (m_stayTimer > 2.0f) {
				m_fall = FallStage::enFall_Back;
				m_stayTimer = 0.0f;
			}
		}
		break;

	case FallStage::enFall_Back:
		/** 落下後の初期位置 */
		m_fallPos = m_initPos;
		m_isPlayerOn = false;
		/** 待機状態に */
		m_fall = FallStage::enFall_Idle;
		break;
	}


	/** 座標の更新 */
	m_fallBoxRender.SetPosition(m_fallPos);
	/** コリジョンの位置を同期させる */
	m_fallCollisionObj->SetPosition(m_fallPos);
	m_fallCollisionObj->Update();

	return true;
}


void FallBox::Render(RenderContext& rc)
{
	m_fallBoxRender.Draw(rc);
}
