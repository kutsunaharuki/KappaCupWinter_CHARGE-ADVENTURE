#include "stdafx.h"
#include "Player.h"
#include "Warp.h"//ゴーストオブジェクト。
#include "MovingFloor.h"
#include "SkyGround.h"

namespace
{
	float JUMP_FRAME_TIME          = 0.52f; //小ジャンプしてる判定の時間。
	const float SMAL_JUMP_POWER    = 500.0f;//小ジャンプ。
	const float BIG_JUMP_POWER     = 590.0f;//大ジャンプ。

	const float GRAVITY            = -9.4f * 2.8f;//重力。

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
	//アニメーションを読み込む
	for (int i = 0; i < enAnimationClip_Num; i++) {
		animationClips[i].Load(ANIM_PATH[i]);
		if (i != enAnimationClip_Jump) {
			animationClips[i].SetLoopFlag(true);
			continue;
		}
		animationClips[i].SetLoopFlag(false);
	}

	//ユニティちゃんの読み込み。
	m_modelRender.Init(PLAYER_1, animationClips, enAnimationClip_Num, enModelUpAxisY);

	//キャラコンの初期化。
	m_charaCon.Init(25.0f, 75.0f, m_position);
	
	m_skyGround = FindGO<SkyGround>("skyGround");

	m_warp = FindGO<Warp>("warp");
	return true;
}

//横に動くときはこれを使う。(横移動用)
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
	//ワープしたら指定した座標に飛ぶ。
	if (CanWarp())
	{
		m_modelRender.SetPosition(m_setPos);
		isHit = false;
		m_modelRender.Update();
	}

	//関数呼び出し。
	Move();
	Rotation();
	PlayAnimation();
	ManageState();

	//プレイヤーの座標の描画準備。
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

	//移動の制御。
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

	//歩きの処理。
	moveSpeed += right   * stickL.x * 200.0f;
	moveSpeed += forward * stickL.z * 200.0f;

	//ダッシュの制御。
	if (g_pad[0]->IsPress(enButtonB))
	{
		isDash = true;
	}
	if (isDash)
	{
		moveSpeed.x *= 2.0f;
		moveSpeed.z *= 2.0f;
	}

	//地面に接しているなら
	if (m_charaCon.IsOnGround())
	{
		canJump = false;
		m_jumpTime = 0.0f;
		//押した瞬間に小ジャンプ。
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
				//離したらジャンプ終了する。
				canJump = false;
			}
		}
		//重力。
		moveSpeed.y += GRAVITY;
	}
	
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(moveSpeed, deltaTime);
	m_modelRender.SetPosition(m_position);
}

/// <summary>
/// プレイヤーの方向(回転)を行う。
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
///プレイヤーのアニメーションのフラグ管理。
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
/// プレイヤーのステート管理。
/// </summary>

void Player::ManageState()
{
	//キャラクターコントローラーが
	// 地面に接していなければステートを1にする。
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
/// 描画処理。
/// </summary>

void Player::Render(RenderContext& rc)
{
	//プレイヤーモデルの描画。
	m_modelRender.Draw(rc);

	//座標の描画。
	m_posFontRender.Draw(rc);
}

bool Player::CanWarp() {
	//キャラクターとゴーストオブジェクトのあたり判定を行う。
	//CanWarpの関数を参照([&])してコピーして値を変更できる。
	//[=]外部をコピーするもの。メモリが増える。
	PhysicsWorld::GetInstance()->ContactTest(m_charaCon,
		[&](const btCollisionObject& contactObject) {
			if (m_warp->m_physicsGhostObj.IsSelf(contactObject) == true)
			{
				//m_physicsGhostObjectとぶつかったら
				//フラグをtrueにする。
				isHit = true;
			}
		});
	return isHit;
}