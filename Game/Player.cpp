#include "stdafx.h"
#include "Player.h"
#include "Warp.h"//ゴーストオブジェクト。
#include "MovingFloor.h"
#include "SkyGround.h"
#include "Enemy.h"

namespace
{
	float JUMP_FRAME_TIME          = 0.52f; //小ジャンプしてる判定の時間。
	const float SMAL_JUMP_POWER    = 500.0f;//小ジャンプ。
	const float BIG_JUMP_POWER     = 590.0f;//大ジャンプ。

	const float GRAVITY            = -10.2f * 2.8f;//重力。

	const char* ANIM_PATH[static_cast<int>(Player::EnAnimationClip::enAnimationClip_Num)] = {
		"Assets/animData/idle.tka",
		"Assets/animData/jump.tka",
		"Assets/animData/walk.tka",
		"Assets/animData/run.tka",
	};
	const char* PLAYER_1 = "Assets/modelData/unityChan.tkm";
}

bool Player::Start()
{
	//アニメーションを読み込む
	for (int i = 0; i < static_cast<int>(Player::EnAnimationClip::enAnimationClip_Num); i++) {
		animationClips[i].Load(ANIM_PATH[i]);
		if (i != static_cast<int>(Player::EnAnimationClip::enAnimationClip_Jump)) {
			animationClips[i].SetLoopFlag(true);
			//continueはスキップする機能がある。
			continue;
		}
		animationClips[i].SetLoopFlag(false);
	}

	//ユニティちゃんの読み込み。
	m_modelRender.Init(
		PLAYER_1,
		animationClips,
		static_cast<int>(Player::EnAnimationClip::enAnimationClip_Num),
		enModelUpAxisY
	);

	//キャラコンの初期化。
	m_charaCon.Init(25.0f, 75.0f, m_position);

	SetBodyCollision();
	return true;
}


void Player::FindGameObjInfo()
{
	m_skyGround    = FindGO<SkyGround>("skyGround");
	m_warp         = FindGO<Warp>("warp");
	m_enemy        = FindGO<Enemy>("enemy");
	m_collisionObj = FindGO<CollisionObject>("collisionObject");
}

Player::~Player()
{
	
}

void Player::Update()
{
	Vector3 pos = m_position;
	if (pos.y < -200.0f)
	{
		ResPawn();
	}

	Move();
	Rotation();
	ManageState();
	TreaderCollisionObj();

	//プレイヤーの座標の描画準備。
	wchar_t playerText[256];
	Vector3 Pos = Vector3(m_position);
	swprintf_s(playerText, 256, L"PosX=%.1f,Y=%.1f,Z=%.1f", Pos.x,Pos.y,Pos.z);
	m_posFontRender.SetText(playerText);
	m_posFontRender.SetPosition(m_fontPos);
	m_posFontRender.SetScale(1.2f);
	m_modelRender.Update();
}

//リスポーンするだけの関数。
void Player::ResPawn()
{
	m_position = m_resPawnPos;
	m_charaCon.SetPosition(m_position);
}

//現在の位置m_positionにdeltaを加算して、
// 新しい位置を計算する。
void Player::AddPosition(const Vector3& delta)
{
	m_position += delta;
	m_charaCon.SetPosition(m_position);
}

void Player::Move()
{
	//リスポーン処理。
	if (m_position.y < -200.0f)
	{
		m_modelRender.SetPosition(m_resPawnPos);
	}

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
	moveSpeed += right   * stickL.x * 240.0f;
	moveSpeed += forward * stickL.z * 240.0f;

	//ダッシュの制御。
	if (g_pad[0]->IsPress(enButtonB))
	{
		isDash = true;
	}
	if (isDash)
	{
		moveSpeed.x *= 2.6f;
		moveSpeed.z *= 2.6f;
	}

	//地面に接しているなら
	if (m_charaCon.IsOnGround())
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
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
		//敵を踏んだ後にY座標が+になる。
		//外部から力を加える。
		moveSpeed += force;
		force *=0.7f;
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
	if (IsMove())
	{
		m_rot.SetRotationYFromDirectionXZ(moveSpeed);
		m_modelRender.SetRotation(m_rot);
	}
}

/// <summary>
/// プレイヤーのステート管理。
/// </summary>

void Player::ManageState()	
{
	if (IsMove())
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

	//キャラクターコントローラーが
	// 地面に接していなければステートを1にする。
	if (m_charaCon.IsOnGround() == false)	{
		playerState = 1;
	}

	m_modelRender.PlayAnimation(playerState);
}

void Player::TreaderCollisionObj()
{
	//もし地面に付いていなかったら(ジャンプ中)
	//コリジョンを作成する。!
	if (JumpAttack()) {

		if (m_collisionObj) {
			m_footCollisionPos = m_position;
			m_collisionObj->SetPosition(m_footCollisionPos);
			m_collisionObj->SetRotation(Quaternion::Identity);
			m_collisionObj->Update();
		}

		if (!m_collisionObj) {
			m_collisionObj = new CollisionObject;
		}

		m_collisionObj->SetIsEnableAutoDelete(false);
		
		m_footCollisionPos = m_position;

		m_collisionObj->CreateBox(
			m_footCollisionPos,      //足の座標。
			Quaternion::Identity,    //回転。
			m_playerCollisionScale   //コリジョンのサイズ。
		);

		m_modelRender.SetPosition(m_footCollisionPos);
		m_modelRender.Update();
	}
}

//プレイヤーの側面(横側から)に当たったらの処理。
void Player::HitBodyPlayer()
{
	if (EnemyCollisionHit())
	{
		
	}
}

//プレイヤーの体にコリジョンを付ける。
void Player::SetBodyCollision()
{
	//コリジョンをnewする。
	m_collisionObj = new CollisionObject;
	m_playerBodyCollisionPos = m_position;

	m_collisionObj->CreateBox(
		m_playerBodyCollisionPos,
		Quaternion::Identity,
		m_playerCollisionScale
	);

	m_collisionObj->SetPosition(m_playerBodyCollisionPos);
	m_collisionObj->SetRotation(Quaternion::Identity);
	m_collisionObj->Update();
	
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

const bool Player::JumpAttack()const
{
	//
	if (m_charaCon.IsOnGround() == false) {
		return true;
	}
	//地面についていれば実行しない。
	else {
		return false;
	}
}

const bool Player::IsMove()const
{
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f) {
		return true;
	}
	else {
		return false;
	}
}

//Playerの側面のコリジョンがEnemyのコリジョンに当たった時に
//Playerが少し後ろに下がる(ノックバックする)。
const bool Player::EnemyCollisionHit()const
{
	if (m_collisionObj->IsHit(m_enemy->m_collisionObj) == true)
	{
		return true;
	}
	else {
		return false;
	}
	return true;
}