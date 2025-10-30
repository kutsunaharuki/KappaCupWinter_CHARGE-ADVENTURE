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
		Quaternion rot;

		std::string GetFullPath() {
			return FILE_PATH + fileName + FILE_EXTENSTION;
		}
	};

	EnemyInfo Enemys[Enemy::enEnemy_Num] = {
		{"Enemy1", {200.0f,0.0f,200.0f},{0.12f,0.12f,0.12f},Quaternion::Identity},
		{"Enemy02",{200.0f,0.0f,200.0f},{0.12f,0.12f,0.12f},Quaternion::Identity},
		{"Boss"  , {200.0f,0.0f,200.0f},{0.12f,0.12f,0.12f},Quaternion::Identity}
	};

	Vector3 ENEMY_GHOSTOBJ_POS = { 100.0f,200.0f,300.0f };//敵の視認範囲用のゴーストオブジェクト。

	const float CHARACON_RADIUS = 30.0f ;//カプセルコライダーの半径。
	const float CHARACON_HEIGHT = 50.0f;//カプセルコライダーの高さ。

	const float ENEMY_RANGE      = 120.0f;//Enemyの追従判定の範囲。
	const float ENEMY_MOVESPEED  = 120.0f;//Eenemyの移動速度。
	
	Vector3 ENEMY_LIMIT = { 400.0f,0.0f,400.0f };
	const float ENEMY_MOVE_LIMIT = 200.0f;//Enemyの行動距離。
	const float ENEMY_GRAVITY = -1.2f * 0.2;//Enemyの重力。
}

bool Enemy1::Start() {
	SetModel(enEnemy1);
	SetPhysicsGameObj(enEnemy1);
	SetCollisionObj(enEnemy1);
	SetFindGOInfo();
	return true;
}

bool Enemy2::Start() {
	SetModel(enEnemy2);
	SetPhysicsGameObj(enEnemy2);
	SetCollisionObj(enEnemy2);
	SetFindGOInfo();
	return true;
}

bool Boss::Start() {
	SetModel(enBoss);
	SetPhysicsGameObj(enBoss);
	SetCollisionObj(enBoss);
	SetFindGOInfo();
	return true;
}

Enemy::~Enemy()
{
	delete m_collisionObj;
}

void Enemy::Update()
{
	Move();
	m_collisionObj->SetPosition(m_enemyPos);
	m_collisionObj->SetRotation(Quaternion::Identity);
	m_collisionObj->Update();
	CollisionUpdate();
	CanHit();
	m_enemyRender.Update();
}

//キャラコンの初期化関数。
void Enemy::SetPhysicsGameObj(int enemyModels)
{
	//キャラコンの初期化(移動用)。
	m_charaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_enemyPos
	);
	m_enemyRender.Update();
}

void Enemy::SetModel(int enemyModel)
{
	m_enemyPos   = Enemys[enemyModel].pos;
	m_enemyScale = Enemys[enemyModel].scale;
	m_enemyRotation = Enemys[enemyModel].rot;

	std::string file = Enemys[enemyModel].GetFullPath();
	//敵の読み込み。
	m_enemyRender.Init(file.c_str());
	m_enemyRender.SetTRS(m_enemyPos, Quaternion::Identity, m_enemyScale);
	m_enemyRender.Update();
}

//コリジョンオブジェク初期化関数。
void Enemy::SetCollisionObj(int enemyModel)
{
	wchar_t enemyPos[256];
	swprintf_s(enemyPos, 256, L"pos X: %f, Y: %f, Z: %f", m_enemyPos.x, m_enemyPos.y, m_enemyPos.z);
	m_collisionFontRender.SetText(enemyPos);

	m_collisionObj = new CollisionObject;

	//コリジョンオブジェクトの初期化。
	m_collisionObj->CreateBox(
		m_enemyPos,             
		Quaternion::Identity,   
		m_enemyCollisionScale
	);

	m_collisionObj->SetPosition(m_enemyPos);
	m_collisionObj->SetRotation(Quaternion::Identity);
	m_collisionObj->Update();
	
}

//FindGO系はSetPlayerInfoに入れる。
void Enemy::SetFindGOInfo() {
	m_player = FindGO<Player>("player");
}

void Enemy::Move()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	
	EnWalkVector vector;
	//enumのEnWalkVector型をint型のrandを型変換して
	vector = static_cast<EnWalkVector>(rand() % enWalkVector_Num);
	Vector3 early = Vector3::Zero;
	const float speed = ENEMY_MOVESPEED;
	const float fall  = ENEMY_GRAVITY;
	early.y += fall;
	Vector3 moveVecCalc = m_enemyPos - m_enemyStartPos;
	float moveLength    = moveVecCalc.Length();

	//現在地が行動範囲のリミットよりも大きかったら
	if (moveLength >= ENEMY_MOVE_LIMIT)
	{
		//ステートを当てはめる。
		m_enemyMoveState = vector;
		//スタート地点を更新。
		m_enemyStartPos  = m_enemyPos;
	}

	switch (m_enemyMoveState) {
	case enWalkVector_Front:
		early = { 0.0f, 0.0f, speed };
		//early.y;
		break;
	case enWalkVector_Back:
		early = { 0.0f,0.0f,-speed };
		//early.y;
		break;
	case enWalkVector_Right:
		early = { speed,0.0f,0.0f };
		//early.y;
		break;
	case enWalkVector_Left:
		early = { -speed,0.0f,0.0f };
		//early.y;
		break;
	case enWalkVector_FrontRight:
		early = { speed,0.0f,speed };
		//early.y;
		break;
	case enWalkVector_FronLeft:
		early = { -speed,0.0f,speed };
		//early.y;
		break;
	case enWalkVector_BackRight:
		early = { speed,0.0f,-speed };
		//early.y;
		break;
	case enWalkVector_BackLeft:
		early = { -speed,0.0f,-speed };
		//early.y;
		break;
	}

	m_enemyMoveSpeed = early;
	m_enemyPos = m_charaCon.Execute(m_enemyMoveSpeed, 1.0f / 60.0f);
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRotation.SetRotationYFromDirectionXZ(m_enemyMoveSpeed);
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


void Enemy::CanHit()
{
	//nullチェック。
	//プレイヤーがnullなら
	if (!m_player->m_collisionObj) {
		return;
	}

	//敵のコリジョンがプレイヤーのコリジョンに
	// 当たったらダメージを受ける。
	if (m_collisionObj->IsHit(m_player->m_collisionObj) == true) {
		Damage(20);
		m_player->force.y = 390.0f;
		DeleteGO(this);
	}
	return;
}

void Enemy::Damage(int damage)
{
	maxHp = hp;
	hp -= damage;
	if (hp < 0)
	{
		hp = 0;
	}
}

void Enemy::Attack()
{
	
}

void Enemy::Render(RenderContext& rc)
{
	//敵の描画。
	m_enemyRender.Draw(rc);

	//コリジョンの座標表示用。
	m_collisionFontRender.Draw(rc);
}

void Enemy::CollisionUpdate() {
	Vector3 collisionPos = m_enemyPos;
	collisionPos.y += 50.0f;
	m_collisionObj->SetPosition(collisionPos);
	//m_collisionObj->SetRotation(m_enemyRotation);
	//m_collisionObj->Update();
}