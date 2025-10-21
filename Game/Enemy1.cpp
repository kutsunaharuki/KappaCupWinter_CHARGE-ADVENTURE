#include "stdafx.h"
#include "Enemy1.h"
#include "Player.h"
#include <time.h>

namespace {
	const char* ENEMY1_INIT = "Assets/modelData/Enemy1.tkm";
	const float ENEMY1_RANGE = 120.0f;
	const float ENEMY1_MOVESPEED  = 200.0f;//Eenemy1の移動速度。
	const float ENEMY1_MOVE_LIMIT = 300.0f;//Enemy1の行動距離。

	
}

bool Enemy1::Start()
{
	m_enemy1Render.Init(ENEMY1_INIT);
	m_enemy1Render.SetPosition(m_enemy1Pos);
	m_enemy1Render.SetScale(m_enemy1Scale);
	m_enemy1Render.Update();

	//m_physicsStaticObj.CreateFromModel(m_enemy1Render.GetModel(),
	//	m_enemy1Render.GetModel().GetWorldMatrix());

	m_player = FindGO<Player>("player");
	srand(time(nullptr));//起動時に敵1が特定の範囲内をランダムで移動する。
	return true;
}

void Enemy1::Update()
{
	//敵1に移動速度を持たせる。
	m_enemy1MoveSpeed.x += ENEMY1_MOVESPEED;

	//敵1とプレイヤーのベクトルを計算してプレイヤーに向かわせる。
	Vector3 diff = m_enemy1Pos - m_player->m_position;
	if (diff.Length() <= ENEMY1_RANGE)
	{
		
	}
}

void Enemy1::Damage(int damage)
{

}

void Enemy1::Move()
{

}

void Enemy1::Render(RenderContext& rc)
{

}