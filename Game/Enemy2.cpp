#include "stdafx.h"
#include "Enemy2.h"
#include "Player.h"

namespace {
	const char* ENEMY2_INIT = "Assets/modelData/Enemy2.tkm";
	const float ENEMY2_RANGE = 120.0f;
}

bool Enemy2::Start()
{
	m_enemy2Render.Init(ENEMY2_INIT);
	m_enemy2Render.SetPosition(m_enemy2Pos);
	m_enemy2Render.SetScale(m_enemy2Scale);
	m_enemy2Render.Update();

	//“G2‚Ì“–‚½‚è”»’èB
	m_physicsStaticObj.CreateFromModel(m_enemy2Render.GetModel(),
		m_enemy2Render.GetModel().GetWorldMatrix());

	return true;
}

void Enemy2::Update()
{
	Vector3 diff = m_enemy2Pos - m_player->m_position;
	if (diff.Length() <= ENEMY2_RANGE)
	{

	}
}

void Enemy2::Damage(int damage)
{

}

void Enemy2::Move()
{

}

void Enemy2::Render(RenderContext& rc)
{

}