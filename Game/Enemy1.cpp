#include "stdafx.h"
#include "Enemy1.h"
#include "Player.h"
#include <time.h>

namespace {
	const char* ENEMY1_INIT = "Assets/modelData/Enemy1.tkm";
	const float ENEMY1_RANGE = 120.0f;
	const float ENEMY1_MOVESPEED  = 200.0f;//Eenemy1�̈ړ����x�B
	const float ENEMY1_MOVE_LIMIT = 300.0f;//Enemy1�̍s�������B

	
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
	srand(time(nullptr));//�N�����ɓG1������͈͓̔��������_���ňړ�����B
	return true;
}

void Enemy1::Update()
{
	//�G1�Ɉړ����x����������B
	m_enemy1MoveSpeed.x += ENEMY1_MOVESPEED;

	//�G1�ƃv���C���[�̃x�N�g�����v�Z���ăv���C���[�Ɍ����킹��B
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