#include "stdafx.h"
#include "WarpBox.h"
#include "Player.h"
#include "Game.h"

namespace {
	const char* WARP_BOX_MODEL_FILE_PATH = "Assets/modelData/WarpBox.tkm";
	const Vector3 COLLISION_SIZE = Vector3::One * 10.0f;
}

WarpBox::~WarpBox()
{
	if (!m_warpCollisionObj) {
		delete m_warpCollisionObj;
		m_warpCollisionObj = nullptr;
	}
}

bool WarpBox::Start()
{
	m_warpBoxModelRender.Init(WARP_BOX_MODEL_FILE_PATH);
	m_warpBoxModelRender.SetPosition(m_warpBoxPos);
	m_warpBoxModelRender.SetRotation(m_warpBoxRot);
	m_warpBoxModelRender.SetScale(m_warpBoxScale);
	m_warpBoxModelRender.Update();
	SetCollisionObj();
	return true;
}

void WarpBox::Update()
{
	const bool& isPause = Game::GetIsPause();
	if (isPause) {
		return;
	}

	if (m_warpCollisionObj) {
		m_warpCollisionObj->SetPosition(m_warpBoxPos);
		m_warpCollisionObj->SetRotation(m_warpBoxRot);
		m_warpCollisionObj->Update();
	}
	m_warpBoxModelRender.Update();
}


void WarpBox::SetCollisionObj()
{
	m_warpCollisionObj = new CollisionObject;
	m_warpCollisionObj->CreateBox(
		m_warpBoxPos,
		m_warpBoxRot,
		COLLISION_SIZE
	);
	m_warpCollisionObj->SetPosition(m_warpBoxPos);
	m_warpCollisionObj->SetRotation(m_warpBoxRot);
}


void WarpBox::Warp()
{
	if (!m_player) {
		return;
	}
	if (m_warpCollisionObj->IsHit(m_player->GetCharacterController())) {
		
	}
}


void WarpBox::WarpDestination()
{

}


void WarpBox::Render(RenderContext& rc)
{
	m_warpBoxModelRender.Draw(rc);
}
