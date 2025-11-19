#include "stdafx.h"
#include "Player.h"
#include "Warp.h"//ゴーストオブジェクト。
#include "MovingFloor.h"
#include "Enemy.h"
#include "HPUI.h"
#include "GameOver.h"

namespace
{
	float JUMP_FRAME_TIME          = 0.52f; //小ジャンプしてる判定の時間。
	const float SMAL_JUMP_POWER    = 500.0f;//小ジャンプ。
	const float BIG_JUMP_POWER     = 590.0f;//大ジャンプ。

	const Vector3 COLL_POS_HEIGHT = { 0.0f,42.0f,0.0f };
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

	SetPlayerCollision();
	SetBodyCollision();
	m_hpui = FindGO<HPUI>("hpui");
	return true;
}

void Player::FindGameObjInfo()
{
	m_warp         = FindGO<Warp>("warp");
	m_enemy        = FindGO<Enemy>("enemy");
}

Player::~Player()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
	if (m_bodyCollisionObj != nullptr)
	{
		delete m_bodyCollisionObj;
		m_bodyCollisionObj = nullptr;
	}
}

void Player::Update()
{
	//無敵時間の処理。
	if (m_invinCibilityTime > 0.0f)
	{
		m_invinCibilityTime -= g_gameTime->GetFrameDeltaTime();
	}
	//ノックバックの処理。
	if (m_knockBackTime > 0.0f)
	{
		m_knockBackTime -= m_deltaTime;

		//ノックバック移動を行う。
		m_position = m_charaCon.Execute(m_backSpeed, m_deltaTime);
		m_modelRender.SetPosition(m_position);

		//returnを返そうとするとプレイヤーが
		//当たった位置から動かずに瞬間移動している
		//ように見えるのでreturnは返さない。

	}
	//Vector3 pos = m_position;
	//if (pos.y < -200.0f)
	//{
	//	ResPawn();
	//}

	Move();
	Rotation();
	ManageState();
	m_collisionObj->SetPosition(m_position);
	TreaderCollisionObj();

	//m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale()
	//プレイヤーの座標の描画準備。
	wchar_t playerText[256];
	Vector3 Pos = Vector3(m_position);
	swprintf_s(playerText, 256, L"PosX=%.1f,Y=%.1f,Z=%.1f", Pos.x,Pos.y,Pos.z);
	m_posFontRender.SetText(playerText);
	m_posFontRender.SetPosition(m_fontPos);
	m_posFontRender.SetScale(1.2f);
	m_modelRender.Update();
}

//ダメージを受けたらの処理。
void Player::ReceiveDamage(int damage, Vector3& enemyPos)
{
	//無敵時間中はダメージを受けない。
	if (m_invinCibilityTime > 0.0f)return;

	hp -= damage;
	if (hp < 0)hp = 0;
	//3.2秒間無敵。
	m_invinCibilityTime = 3.2f;
	
	//現在のHPをとっている。
	if (m_hpui)
	{
		m_hpui->TakeDamage(hp);
	}

	//ノックパック処理。
	//ノックバック方向を決める式。
	Vector3 backDir = m_position - enemyPos;
	//プレイヤーのベクトルを正規化する。
	backDir.Normalize();
	//ノックバックパワー。
	float backPower = 200.0f;

	//ノックバック速度をそのまま速度にする。
	m_backSpeed = backDir * backPower;

	//1.8秒だけノックバックさせる。
	m_knockBackTime = 1.8f;

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

/// <summary>
/// 移動処理。
/// </summary>
void Player::Move()
{
	//リスポーン処理。
	if (m_position.y < -200.0f)
	{
		m_modelRender.SetPosition(m_resPawnPos);
	}

	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//移動の制御。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

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
	m_moveSpeed += right   * stickL.x * 240.0f;
	m_moveSpeed += forward * stickL.z * 240.0f;

	//ダッシュの制御。
	if (g_pad[0]->IsPress(enButtonB))
	{
		isDash = true;
	}
	if (isDash)
	{
		m_moveSpeed.x *= 2.6f;
		m_moveSpeed.z *= 2.6f;
	}

	//地面に接しているなら
	if (m_charaCon.IsOnGround())
	{
		canJump = false;
		m_jumpTime = 0.0f;
		//押した瞬間に小ジャンプ。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_moveSpeed.y = SMAL_JUMP_POWER;
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
					m_moveSpeed.y = BIG_JUMP_POWER;
					m_moveSpeed += force;
					force *= 0.7f;
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
		m_moveSpeed += force;
		force *=0.7f;
		//重力。
		m_moveSpeed.y += GRAVITY;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, deltaTime);
	m_modelRender.SetPosition(m_position);
}

/// <summary>
/// プレイヤーの方向(回転)を行う。
/// </summary>

void Player::Rotation()
{
	if (IsMove())
	{
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
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

const bool Player::TreaderCollisionObj()
{
	bool isJumpAttack = JumpAttack();
	//本体用コリジョン。
	if (m_bodyCollisionObj)
	{
		m_bodyCollisionObj->SetIsEnable(isJumpAttack);
		if (m_charaCon.IsOnGround())
		{
			m_bodyCollisionObj->SetPosition(m_position + COLL_POS_HEIGHT);
			m_bodyCollisionObj->SetRotation(m_rot);
		}
		else {
			//攻撃をしていない時は待避。
			//めちゃくちゃ遠くにコリジョンを飛ばす。
			m_bodyCollisionObj->SetPosition({ 0.0f,-10000.0f,0.0f });
		}
	}
	
	//踏み判定用コリジョン。
	if (m_collisionObj)
	{
		m_collisionObj->SetIsEnable(isJumpAttack);
		if (isJumpAttack)
		{
			m_collisionObj->SetPosition(m_position);
			m_collisionObj->SetRotation(m_rot);
		}
		else {
			//遠くに待避させる。
			//めちゃくちゃ遠くにコリジョンを飛ばす。
			m_collisionObj->SetPosition({ 0.0f,-10000.0f,0.0f });
		}
	}

	return false;
}

//プレイヤーのBodyコリジョンに当たったらの処理。
bool Player::HitBodyPlayer()
{
	return false;
}

//足元にコリジョンが生成される。
void Player::SetPlayerCollision()
{
	m_collisionObj = new CollisionObject;
	m_position = m_position + m_footCollisionPos;
	m_collisionObj->CreateBox(
		m_position,
		m_rot,
		m_playerCollisionScale
	);

	m_collisionObj->SetPosition(m_footCollisionPos);
	m_collisionObj->SetRotation(m_rot);
}

//プレイヤーの体にコリジョンを付ける。
void Player::SetBodyCollision()
{
	//コリジョンをnewする。
	m_bodyCollisionObj = new CollisionObject;

	m_position = m_position + COLL_POS_HEIGHT;

	m_bodyCollisionObj->CreateBox(
		m_position,
		m_rot,
		m_playerBodyCollisionSc
	);
	m_bodyCollisionObj->SetPosition(m_position);
	m_bodyCollisionObj->SetRotation(m_rot);
}

/// <summary>
/// 描画処理。
/// </summary>
void Player::Render(RenderContext& rc)
{
	bool isDrawPlayer = true;
	//無敵時間の間プレイヤーを点滅させる。
	if (m_invinCibilityTime > 0.0f)
	{
		//点滅スピード(10回/秒(s))。
		//無敵時間 * 点滅速度 = 点滅のクールタイム。
		const float blinkSpeed = 10.0f;
		float flashingTime = m_invinCibilityTime * blinkSpeed;

		//小数点(0～1)。
		//小数点以下を切り捨てすることが出来て、
		//"整数部分だけ"を取り出すことが出来る関数。
		float decimalPoint = flashingTime - floorf(flashingTime);

		//flashingTime < 0.6f ->表示,
		//          or
		// flashingTime >= 0.6f ->非表示。
		isDrawPlayer = (decimalPoint < 0.6f);
	}

	//点滅表示。
	//モデルの点滅。
	if (isDrawPlayer)
	{
		m_modelRender.Draw(rc);
	}

	//プレイヤーモデルの描画。
	//m_modelRender.Draw(rc);

	//座標の描画。
	//m_posFontRender.Draw(rc);
}

const bool Player::JumpAttack()const
{
	if (m_charaCon.IsOnGround() == false) {
		return true;
	}
	else {
		return false;
	}
	//地面についていれば実行しない。
	return false;
	
}

const bool Player::IsMove()const
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		return true;
	}
	return false;
	
}

//Playerの側面のコリジョンがEnemyのコリジョンに当たった時に
//Playerが少し後ろに下がる(ノックバックする)。
const bool Player::EnemyCollisionHit()const
{
	if (m_collisionObj->IsHit(m_enemy->GetCollision()))
	{
		return true;
	}
	return false;
}