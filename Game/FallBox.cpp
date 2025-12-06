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
	m_fallBoxRender.Init(FALL_BOX_FILE_PATH);
	m_fallBoxRender.SetPosition(m_fallPos);
	m_fallBoxRender.SetRotation(m_fallRot);
	m_fallBoxRender.SetScale(m_fallScale);
	m_fallBoxRender.Update();

	m_fallPhysicsObj.CreateFromModel(
		m_fallBoxRender.GetModel(),
		m_fallBoxRender.GetModel().GetWorldMatrix()
	);

	m_player = FindGO<Player>("player");
	return true;
}

void FallBox::Update()
{
	SetCollision();
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

}


bool FallBox::FallMove()
{
	m_player = FindGO<Player>("player");
	if (!m_player || !m_fallCollisionObj)return false;

	if (m_fallCollisionObj->IsHit(m_player->GetCharacterController()) 
		&& m_player->GetPosition().y > m_fallPos.y + 30.0f) {
		m_isPlayerOn = true;
	}
	else {
		m_isPlayerOn = false;
	}

	if (m_isPlayerOn) {
		Vector3 pos = m_fallPos;
		pos.y -= m_sinkSpeed * m_deltaTime;
	}

	return false;
}


void FallBox::Render(RenderContext& rc)
{
	m_fallBoxRender.Draw(rc);
}
