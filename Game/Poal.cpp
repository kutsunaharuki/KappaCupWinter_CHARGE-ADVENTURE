#include "stdafx.h"
#include "Poal.h"
#include "Player.h"
#include "Game.h"

namespace {
	const char* POAL = "Assets/modelData/Poal.tkm";
	const float RADIUS = 90.0f;//カプセルコライダーの半径。
	const float HEIGHT = 500.0f;//カプセルコライダーの高さ。
	const Vector3 POS_HEIGHT = { 0.0f,250.0f,0.0f };//座標を上に上げる。
}

Poal::~Poal()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
}

bool Poal::Start()
{
	m_poalRender.Init(POAL);
	m_poalRender.SetPosition(m_pos);
	m_poalRender.SetScale(m_scale);
	m_poalRender.Update();

	SetCollisionObj();
	m_collisionObj->SetPosition(m_pos);
	m_collisionObj->SetRotation(m_rot);
	m_collisionObj->Update();
	return true;
}

void Poal::Update()
{
	PoalHit();
	m_poalRender.Update();
}

void Poal::SetCollisionObj()
{
	m_collisionObj = new CollisionObject;
	m_collisionObj->CreateCapsule(
		m_pos,           //座標。
		m_rot,              //回転。
		RADIUS,             //半径。
		HEIGHT              //高さ。
	);
	//自動で削除しない。
	m_collisionObj->SetIsEnableAutoDelete(false);
	m_collisionObj->SetPosition(m_colPos);
	m_collisionObj->SetRotation(m_rot);
	m_collisionObj->Update();
}

/// <summary>
/// ゴールポールのコリジョンに当たった時の処理。
/// </summary>
bool Poal::PoalHit()
{
	if (m_collisionObj != nullptr && m_player != nullptr)
	{
		if (m_collisionObj->IsHit(m_player->GetCharacterController()) == true)
		{
			isHit = true;
			return true;
		}
		return true;
	}
	return true;
}

void Poal::Render(RenderContext& rc)
{
	m_poalRender.Draw(rc);
}