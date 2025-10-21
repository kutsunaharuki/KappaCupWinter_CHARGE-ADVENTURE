#include "stdafx.h"
#include "MovingFloor.h"
#include "Player.h"

namespace {
	Vector3 SCALE = Vector3::One * 3.0f;
	float SPEED   = 300.0f;
	float LIMITED = 400.0f;
	Vector3 COLLISION_HEIGHT = Vector3(0.0f, 50.0f, 0.0f);
	Vector3 COLLISION_SIZE = Vector3(365.0f, 5.0f, 225.0f);
	const char* MOVINGFLOOR_INIT = "Assets/modelData/MovingFloor.tkm";
}

MovingFloor::MovingFloor()
{

}

MovingFloor::~MovingFloor()
{

}

bool MovingFloor::Start()
{
	m_movingFloorRender.Init(MOVINGFLOOR_INIT);
	m_movingFloorRender.SetPosition(m_movingPosition);
	m_movingFloorRender.SetScale(SCALE);
	m_movingFloorRender.Update();

	//�����蔻��̒ǉ��B
	m_physicsStaticObj.CreateFromModel(m_movingFloorRender.GetModel(),
		m_movingFloorRender.GetModel().GetWorldMatrix());
	
	m_collisionObj = NewGO<CollisionObject>(0, "collisionObject");
	//�R���W�����I�u�W�F�N�g�𓮂����ɐݒu����B
	//(�L�����N�^�[����ɏ�����甽������悤�ɂ���������)�B
	//�R���W�����I�u�W�F�N�g�̏������B
	m_collisionObj->CreateBox(
		m_movingPosition + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);
	m_collisionObj->SetIsEnableAutoDelete(false);
	m_firstPosition = m_movingPosition;

	m_player = FindGO<Player>("player");
	return true;
}

void MovingFloor::Update()
{
	Move();
	Vector3 Pos = m_movingPosition;

	//�����ǂꂾ�����������v�Z����B
	Vector3 moveCalc = m_movingPosition - Pos;

	//�ÓI�I�u�W�F�N�g�̏������B
	//���̂�j������B
	m_physicsStaticObj.Release();
	m_physicsStaticObj.CreateFromModel(m_movingFloorRender.GetModel(),
		m_movingFloorRender.GetModel().GetWorldMatrix());

	//�R���W�����I�u�W�F�N�g�ƃv���C���[�̃L�����N�^�[�R���g���[���[��
	////�Փ˂�����(�L�����N�^�[���������̏�ɏ������)�B
	//if (m_collisionObj->IsHit(m_player->GetCharacterController()) == true)
	//{
	//	//�������̈ړ����x���L�����N�^�[�̈ړ����x�ɉ��Z�B
	//	m_player->AddPosition(moveCalc);
	//}

	m_movingFloorRender.Update();

	m_collisionObj->SetPosition(m_movingPosition);
}

void MovingFloor::Move()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	Vector3 moveSpeed = Vector3::Zero;
	//��������O,�������ɓ������B
	if (m_movingFloorState == enMovingFloorState_MovingForward)
	{
		moveSpeed.z = -SPEED;
	}
	else if (m_movingFloorState == enMovingFloorState_MovingBack)
	{
		moveSpeed.z = SPEED;
	}

	m_movingPosition += moveSpeed * deltaTime;

	if (m_movingFloorState == enMovingFloorState_MovingForward)
	{
		if (m_firstPosition.z - LIMITED >= m_movingPosition.z)
		{
			m_movingFloorState = enMovingFloorState_MovingBack;
		}
	}

	else if (m_movingFloorState == enMovingFloorState_MovingBack)
	{
		if (m_firstPosition.z + LIMITED <= m_movingPosition.z)
		{
			m_movingFloorState = enMovingFloorState_MovingForward;
		}
	}

	m_movingFloorRender.SetPosition(m_movingPosition);
}

void MovingFloor::Render(RenderContext& rc)
{
	//�������̕`��B
	m_movingFloorRender.Draw(rc);
}