#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <time.h>

namespace {
	//ファイルの場所。
	std::string FILE_PATH = "Assets/Enemy/";
	//拡張子。
	std::string FILE_EXTENSTION = ".tkm";
	
	struct EnemyInfo
	{
		//ファイル名。
		std::string fileName = "fileName";

		Vector3 pos = Vector3::Zero;
		Vector3 scale;

		float ghostObjScl = 0.0f;

		std::string GetFullPath() {
			return FILE_PATH + fileName + FILE_EXTENSTION;
		}
	};

	EnemyInfo Enemys[Enemy::enEnemy_Num] = {
		{"Enemy1", {100.0f,0.0f,200.0f},{0.2f,0.49f,0.2f},1.0f},
		{"Enemy2", {100.0f,0.0f,200.0f},{0.2f,0.49f,0.2f},50.0f},
		{"Boss"  , {100.0f,0.0f,200.0f},{0.2f,0.49f,0.2f},50.0f}
	};

	Vector3 ENEMY_GHOSTOBJ_POS = { 100.0f,200.0f,300.0f };//敵の視認範囲用のゴーストオブジェクト。
	Vector3 ENEMY_GHOSTOBJ_SC = Vector3::One * 100.0f;

	const float CHARACON_RADIUS = 90.0f ;//カプセルコライダーの半径。
	const float CHARACON_HEIGHT = 100.0f;//カプセルコライダーの高さ。

	const float ENEMY_RANGE      = 120.0f;//Enemyの追従判定の範囲。
	const float ENEMY_MOVESPEED  = 120.0f;//Eenemyの移動速度。
	
	Vector3 ENEMY_LIMIT = { 400.0f,0.0f,400.0f };
	const float ENEMY_MOVE_LIMIT = 200.0f;//Enemyの行動距離。
}

bool Enemy1::Start() {
	SetModel(enEnemy1);
	SetFindGOInfo();
	m_enemyStartPos = m_enemyPos;
	return true;
}


bool Enemy2::Start() {
	SetModel(enEnemy2);
	SetFindGOInfo();
	return true;
}

bool Boss::Start() {
	SetModel(enBoss);
	SetFindGOInfo();
	return true;
}



void Enemy::SetModel(int enemyModel)
{
	m_enemyPos   = Enemys[enemyModel].pos;
	m_enemyScale = Enemys[enemyModel].scale;
	std::string file = Enemys[enemyModel].GetFullPath();
	//敵の読み込み。
	m_enemyRender.Init(file.c_str());

	//キャラコンの初期化(移動用)。
	m_charaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_enemyPos
	);
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.SetScale(m_enemyScale);
	m_enemyRender.Update();

	//ゴーストオブジェクトの初期化(衝突判定用)。
	m_physicsGhostObj.CreateBox(
		m_enemyPos,
		Quaternion::Identity,
		ENEMY_GHOSTOBJ_SC
	);

	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.SetScale(m_enemyScale);
	m_enemyRender.Update();
	//m_physicsGhostObj.SetPosition(m_enemyPos);
}

//FindGO系はSetPlayerInfoに入れる。
void Enemy::SetFindGOInfo() {
	m_player = FindGO<Player>("player");
}

void Enemy::Update()
{
	Move();
	m_enemyRender.SetPosition(m_enemyPos);
	m_physicsGhostObj.SetPosition(m_enemyPos);
	m_enemyRender.Update();
	//敵とプレイヤーのベクトルを計算してプレイヤーに向かわせる。
	//Vector3 diff = m_enemyPos - m_player->m_position;
	//if (diff.Length() <= ENEMY_RANGE)
	//{
	//	//早期リターンはこれ以上下は実行しない。
	//	return;
	//}

	
}

void Enemy::Move()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	
	if (!m_charaCon.IsOnGround())
	{
		//canMove = true;
		return;
	}


	//EnWalkVector num = rand() % enWalkVector_Num;
	EnWalkVector vector;
	//enumのEnWalkVector型をint型のrandを型変換して
	vector = static_cast<EnWalkVector>(rand() % enWalkVector_Num);
	Vector3 early = Vector3::Zero;
	const float speed = ENEMY_MOVESPEED;

	Vector3 moveVecCalc = m_enemyPos - m_enemyStartPos;
	float moveLength = moveVecCalc.Length();

	if (moveLength >= ENEMY_MOVE_LIMIT)
	{
		m_enemyMoveState = vector;
		m_enemyStartPos  = m_enemyPos;
	}

	switch (m_enemyMoveState) {
	case enWalkVector_Front:
		early = { 0.0f, 0.0f, speed };
		m_enemyRotation.SetRotationDegY(0.0f);
		break;
	case enWalkVector_Back:
		early = { 0.0f,0.0f,-speed };
		m_enemyRotation.SetRotationDegY(180.0f);
		break;
	case enWalkVector_Right:
		early = { speed,0.0f,0.0f };
		m_enemyRotation.SetRotationDegY(90.0f);
		break;
	case enWalkVector_Left:
		early = { -speed,0.0f,0.0f };
		m_enemyRotation.SetRotationDegY(-90.0f);
		break;
	case enWalkVector_FrontRight:
		early = { speed,0.0f,speed };
		m_enemyRotation.SetRotationDegY(45.0f);
		break;
	case enWalkVector_FronLeft:
		early = { -speed,0.0f,speed };
		m_enemyRotation.SetRotationDegY(-45.0f);
		break;
	case enWalkVector_BackRight:
		early = { speed,0.0f,-speed };
		m_enemyRotation.SetRotationDegY(135.0f);
		break;
	case enWalkVector_BackLeft:
		early = { -speed,0.0f,-speed };
		m_enemyRotation.SetRotationDegY(-135.0f);
		break;
	}

	m_enemyMoveSpeed = early;
	
	m_enemyPos = m_charaCon.Execute(m_enemyMoveSpeed, deltaTime);
	
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.SetRotation(m_enemyRotation);
}

void Enemy::EnemyBehavior()
{
	//プレイヤーとの距離を測る。
	Vector3 diff = m_player->m_position - m_enemyPos;
	float distance = diff.Length();

	//一定距離内に入ると追跡モードに切り替わる。
	if (distance <= ENEMY_RANGE)
	{
		m_enemyActionState = enEnemyActionState_Chase;
	}
	//一定距離外なら徘徊モードに切り替わる。
	else {
		m_enemyActionState = enEnemyActionState_Wandering;
	}


	switch (m_enemyActionState) {
	case enEnemyActionState_Wandering:

		break;
	case enEnemyActionState_Chase:

		break;
	case enEnemyActionState_Idle:

		break;
	case enEnemyActionState_Attack:

		break;
	}
}

void Enemy::Attack()
{
	
}

void Enemy::Render(RenderContext& rc)
{
	//敵の描画。
	m_enemyRender.Draw(rc);
}