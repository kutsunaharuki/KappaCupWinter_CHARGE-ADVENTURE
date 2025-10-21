#include "stdafx.h"
#include "Player.h"
#include "Warp.h"//�S�[�X�g�I�u�W�F�N�g�B
#include "MovingFloor.h"
#include "SkyGround.h"

namespace
{
	float JUMP_FRAME_TIME          = 0.52f; //���W�����v���Ă锻��̎��ԁB
	const float SMAL_JUMP_POWER    = 500.0f;//���W�����v�B
	const float BIG_JUMP_POWER     = 590.0f;//��W�����v�B

	const float GRAVITY            = -9.4f * 2.8f;//�d�́B

	const char* ANIM_PATH[Player::enAnimationClip_Num] = {
		"Assets/animData/idle.tka",
		"Assets/animData/jump.tka",
		"Assets/animData/walk.tka",
		"Assets/animData/run.tka",
	};
	const char* PLAYER_1 = "Assets/modelData/unityChan.tkm";
}

Player::Player()
{

}

bool Player::Start()
{
	//�A�j���[�V������ǂݍ���
	for (int i = 0; i < enAnimationClip_Num; i++) {
		animationClips[i].Load(ANIM_PATH[i]);
		if (i != enAnimationClip_Jump) {
			animationClips[i].SetLoopFlag(true);
			continue;
		}
		animationClips[i].SetLoopFlag(false);
	}

	//���j�e�B�����̓ǂݍ��݁B
	m_modelRender.Init(PLAYER_1, animationClips, enAnimationClip_Num, enModelUpAxisY);

	//�L�����R���̏������B
	m_charaCon.Init(25.0f, 75.0f, m_position);
	
	m_skyGround = FindGO<SkyGround>("skyGround");

	m_warp = FindGO<Warp>("warp");
	return true;
}

//���ɓ����Ƃ��͂�����g���B(���ړ��p)
//void Player::AddPosition(const Vector3& delta)
//{
//	m_position += delta;
//	m_charaCon.SetPosition(m_position);
//}

Player::~Player()
{

}

void Player::Update()
{
	//���[�v������w�肵�����W�ɔ�ԁB
	if (CanWarp())
	{
		m_modelRender.SetPosition(m_setPos);
		isHit = false;
		m_modelRender.Update();
	}

	//�֐��Ăяo���B
	Move();
	Rotation();
	PlayAnimation();
	ManageState();

	//�v���C���[�̍��W�̕`�揀���B
	wchar_t playerText[256];
	Vector3 Pos = Vector3(m_position);
	swprintf_s(playerText, 256, L"PosX=%.1f,Y=%.1f,Z=%.1f", Pos.x,Pos.y,Pos.z);
	m_posFontRender.SetText(playerText);
	m_posFontRender.SetPosition(m_fontPos);
	m_posFontRender.SetScale(1.2f);
	m_modelRender.Update();
}

void Player::Move()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//�ړ��̐���B
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right   = g_camera3D->GetRight();

	forward.y = 0.0f;
	forward.Normalize();
	
	right.y = 0.0f;
	right.Normalize();

	//�����̏����B
	moveSpeed += right   * stickL.x * 200.0f;
	moveSpeed += forward * stickL.z * 200.0f;

	//�_�b�V���̐���B
	if (g_pad[0]->IsPress(enButtonB))
	{
		isDash = true;
	}
	if (isDash)
	{
		moveSpeed.x *= 2.0f;
		moveSpeed.z *= 2.0f;
	}

	//�n�ʂɐڂ��Ă���Ȃ�
	if (m_charaCon.IsOnGround())
	{
		canJump = false;
		m_jumpTime = 0.0f;
		//�������u�Ԃɏ��W�����v�B
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			moveSpeed.y = SMAL_JUMP_POWER;
			canJump = true;
		}
	}

	else
	{
		if (canJump)
		{
		    if (g_pad[0]->IsPress(enButtonA))
			{
				m_jumpTime += deltaTime;
				if (m_jumpTime <= JUMP_FRAME_TIME)
				{
					moveSpeed.y = BIG_JUMP_POWER;
				}
			}
			else
			{
				//��������W�����v�I������B
				canJump = false;
			}
		}
		//�d�́B
		moveSpeed.y += GRAVITY;
	}
	
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charaCon.Execute(moveSpeed, deltaTime);
	m_modelRender.SetPosition(m_position);
}

/// <summary>
/// �v���C���[�̕���(��])���s���B
/// </summary>

void Player::Rotation()
{
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		m_rot.SetRotationYFromDirectionXZ(moveSpeed);
		m_modelRender.SetRotation(m_rot);
	}
}

/// <summary>
///�v���C���[�̃A�j���[�V�����̃t���O�Ǘ��B
/// </summary>

void Player::PlayAnimation()
{
	switch (playerState)
	{
	case 0:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case 1:
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
	case 2:
		m_modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	case 3:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
}

/// <summary>
/// �v���C���[�̃X�e�[�g�Ǘ��B
/// </summary>

void Player::ManageState()
{
	//�L�����N�^�[�R���g���[���[��
	// �n�ʂɐڂ��Ă��Ȃ���΃X�e�[�g��1�ɂ���B
	if (m_charaCon.IsOnGround() == false)
	{
		playerState = 1;
		return;
	}

	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		playerState = 2;
		if (g_pad[0]->IsPress(enButtonB))
		{
			playerState = 3;
		}
	}
	else
	{
		playerState = 0;
	}
}

/// <summary>
/// �`�揈���B
/// </summary>

void Player::Render(RenderContext& rc)
{
	//�v���C���[���f���̕`��B
	m_modelRender.Draw(rc);

	//���W�̕`��B
	m_posFontRender.Draw(rc);
}

bool Player::CanWarp() {
	//�L�����N�^�[�ƃS�[�X�g�I�u�W�F�N�g�̂����蔻����s���B
	//CanWarp�̊֐����Q��([&])���ăR�s�[���Ēl��ύX�ł���B
	//[=]�O�����R�s�[������́B��������������B
	PhysicsWorld::GetInstance()->ContactTest(m_charaCon,
		[&](const btCollisionObject& contactObject) {
			if (m_warp->m_physicsGhostObj.IsSelf(contactObject) == true)
			{
				//m_physicsGhostObject�ƂԂ�������
				//�t���O��true�ɂ���B
				isHit = true;
			}
		});
	return isHit;
}