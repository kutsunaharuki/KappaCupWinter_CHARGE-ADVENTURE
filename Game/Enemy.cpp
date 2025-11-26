#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "ObstacleBox.h"
#include "Score.h"
#include <time.h>

namespace {
	//ファイルの場所。
	std::string FILE_PATH = "Assets/modelData/";
	//拡張子。
	std::string FILE_EXTENSTION = ".tkm";



	struct EnemyInfo
	{
		//ファイル名。
		std::string fileName = "fileName";

		Vector3 collisionSc;
		//Vector3 collisionPos;

		std::string GetFullPath() {
			return FILE_PATH + fileName + FILE_EXTENSTION;
		}
	};


	//ファイルの場所。
	std::string ENEMY_ANIM_PATH = "Assets/EnmyAnimData/";
	//拡張子。
	std::string ENEMY_FILE_EXTENSTION = ".tka";

	struct AnimInfo
	{
		//ファイル名。
		std::string fileName = "fileName";

		std::string GetAnimPath()
		{
			return ENEMY_ANIM_PATH + fileName + ENEMY_FILE_EXTENSTION;
		}
	};

	EnemyInfo Enemys[enEnemy_Num] = {
		{"Frogs"   ,   {200.0f,120.0f,200.0f}},
		{"Enemy02" ,   {120.0f,140.0f,100.0f}},
		{"Bear"    ,   {300.0f,200.0f,300.0f}},
	};


	AnimInfo Animas[enEnemy_Num] = {
		{"Jumping"},
		{""},
		{""}
	};

	AnimInfo enemy1[1] = { "Jumping" };

	AnimInfo enemy2[]

	Vector3 ENEMY_GHOSTOBJ_POS = { 100.0f,200.0f,300.0f };//敵の視認範囲用のゴーストオブジェクト。

	const float CHARACON_RADIUS = 30.0f ;//カプセルコライダーの半径。
	const float CHARACON_HEIGHT = 25.0f;//カプセルコライダーの高さ。

	const float ENEMY_RANGE      = 120.0f;//Enemyの追従判定の範囲。
	const float ENEMY_MOVESPEED  = 120.0f;//Eenemyの移動速度。
	
	Vector3 ENEMY_LIMIT = { 400.0f,0.0f,400.0f };
	const float ENEMY_MOVE_LIMIT = 50.0f;//Enemyの行動距離。
	//const float ENEMY_GRAVITY = -1.2f * 0.2;//Enemyの重力。

	Vector3 FORWARD = Vector3::AxisZ;
}

Enemy::Enemy()
{

}


bool Enemy::Start()
{
	return true;
}

bool Enemy1::Start() {
	SetModel(enEnemy1);
	SetPhysicsGameObj(enEnemy1);
	SetCollisionObj(enEnemy1);
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}

bool Enemy2::Start() {
	SetModel(enEnemy2);
	SetPhysicsGameObj(enEnemy2);
	SetCollisionObj(enEnemy2);
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}

bool Boss::Start() {
	SetModel(enBoss);
	SetPhysicsGameObj(enBoss);
	SetCollisionObj(enBoss);
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}

Enemy::~Enemy()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
}


void Enemy::Update()
{
	Move();
	EnemyBehavior();
	IsFoundPlayer();
	m_collisionObj->SetPosition(m_enemyPos);
	m_collisionObj->SetRotation(Quaternion::Identity);
	m_collisionObj->Update();
	EnemyHit();
	CanHit();
	m_enemyRender.SetPosition(m_enemyPos);
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
	m_collisionObjStartPos = m_enemyPos;

	std::string file = Enemys[enemyModel].GetFullPath();
	//敵の読み込み。
	m_enemyRender.Init(file.c_str());
	m_enemyRender.SetTRS(m_enemyPos, Quaternion::Identity, m_enemyScale);
	m_enemyRender.Update();
}

//コリジョンオブジェクト初期化関数。
void Enemy::SetCollisionObj(int enemyModel)
{
	Vector3 pos = m_enemyPos;

	m_collisionObj = new CollisionObject;

	//コリジョンオブジェクトの初期化。
	m_collisionObj->CreateBox(
		pos,
		Quaternion::Identity,
		Enemys[enemyModel].collisionSc
	);

	m_collisionObj->SetPosition(pos);
	m_collisionObj->SetRotation(Quaternion::Identity);
	m_collisionObj->Update();
}

//スフィアコライダーの半径の設定関数。
void Enemy::SetSphereColliderObj()
{
	const float radius = 2.0f;
	m_sphereCollider.Create(radius);
}

//FindGO系はSetPlayerInfoに入れる。
void Enemy::SetFindGOInfo() {
	m_player = FindGO<Player>("player");
}

void Enemy::Move() {
	if (IsFoundPlayer()) {
		RandomWalk();
	}
	Tracking();
}

//追跡の関数。
void Enemy::Tracking()
{
	//nullチェック。
	if (m_player == nullptr)
	{
		m_player = FindGO<Player>("player");
		return;
	}

	if (!IsFoundPlayer())
	{
		return;
	}

	//プレイヤーの座標を空のローカル変数に渡す。
	Vector3 playerPos = m_player->m_position;

	//プレイヤーの座標からエネミーの座標を引く。
	Vector3 diff = playerPos - m_enemyPos;

	//これがないとプレイヤーが上にいても追いかけてくる。
	diff.y = 0.0f;

	//ベクトルの距離計算。
	float distance = diff.Length();

	//ベクトルを正規化。
	diff.Normalize();

	//追跡計算を行う。
	float targetSpeed = 200.0f;

	//m_moveSpeedにどの方向でどんな速さで進むかを代入させる。
	//「進行方向　×　速度」＝　実際の移動ベクトル。
	m_moveSpeed = diff * targetSpeed;

	UpdateEnemyInfo();
	
}

/** Enemyがランダムに動く */
void Enemy::RandomWalk()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	
	EnWalkVector vector;
	//enumのEnWalkVector型をint型のrandを型変換して
	vector = static_cast<EnWalkVector>(rand() % enWalkVector_Num);
	Vector3 early = Vector3::Zero;
	const float speed = ENEMY_MOVESPEED;

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
	m_enemyRotation.SetRotationYFromDirectionXZ(m_enemyMoveSpeed);
	UpdateEnemyInfo();

}

struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//ボックスとぶつかったら
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Box)
		{
			return 0.0f;
		}

		//ボックスとぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};

//プレイヤーを見つけるかどうかの関数。
const bool Enemy::IsFoundPlayer()
{
	m_enemyRotation.Apply(FORWARD);
	
	Vector3 playerPos = m_player->GetPosition();
	Vector3 diff = playerPos - m_enemyPos;

	//ベクトルを正規化する。
	diff.Normalize();
	//ベクトルの内積の計算。
	float angle = acosf(diff.Dot(FORWARD));

	//プレイヤーが視界内にいるなら。
	if (Math::PI * 0.35f >= fabsf(angle))
	{
		//プレイヤーが見つかったなら。
		m_isSearchPlayer = true;
		return true;
	}

	//プレイヤーが視界内に居なかったら。
	else if(Math::PI * 0.35f <= fabsf(angle))
	{
		//プレイヤーが見つかってない。
	    m_isSearchPlayer = false;
		return false;
	}


	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	//エネミーのY座標を上げるのはプレイヤーの足元を見ないため。
	start.setOrigin(btVector3(m_enemyPos.x, m_enemyPos.y + 70.0f, m_enemyPos.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPos.x, playerPos.y + 70.0f, playerPos.z));

	//TODO:これより下のEnemyの視野角Y座標だけを上げる処理は別のbool型の関数に移動。
	SweepResultWall callBack;
	//コライダーが始点から終点まで動かして
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callBack);
	//壁と衝突してない。
	if (callBack.isHit == true)
	{
		return false;
	}
	
	return true;
}

void Enemy::UpdateEnemyInfo() {
	m_enemyPos = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.SetRotation(m_enemyRotation);
	m_enemyRender.Update();
}

//行動の関数。
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
		//徘徊。
	case enEnemyActionState_Wandering:
		
		break;
		//追跡。
	case enEnemyActionState_Chase:

		break;
		//攻撃。
	case enEnemyActionState_Attack:

		break;
	}
}

//EnemyがPlayerに衝突したらダメージを与える処理。
void Enemy::EnemyHit()
{
	if (!m_player || !m_player->m_bodyCollisionObj)
	{
		return;
	}

	//プレイヤーの体についてるコリジョンに当たったら
	if (m_collisionObj->IsHit(m_player->m_bodyCollisionObj))
	{
		m_player->ReceiveDamage(1,m_enemyPos);
	}
}

void Enemy::CanHit()
{
	//nullチェック。
	//プレイヤーの体についてるコリジョンかプレイヤーがnullptrじゃないなら
	/*if (!m_score)
	{
		m_score = FindGO<Score>("score");
	}*/
	if (!m_player->m_collisionObj || !m_player) {
		return;
	}

	if (m_collisionObj->IsHit(m_player->m_collisionObj))
	{
		m_player->force.y = 390.0f;
		//m_score->AddScore(100);
		DeleteGO(this);
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
	//m_collisionFontRender.Draw(rc);

	if (!m_isSearchPlayer)
	{
		m_fontRender.SetText(L"見つかってない・・・");
		m_fontRender.SetPosition(m_fontPos);
	}
	else {
		m_fontRender.SetText(L"見つけた!");
		m_fontRender.SetPosition(m_fontPos);
	}
	m_fontRender.Draw(rc);
}