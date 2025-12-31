#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "ObstacleBox.h"
#include "Score.h"
#include "Game.h"
#include "SoundManager.h"
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
		{"Frogs"   ,   {220.0f,100.0f,220.0f}},
		{"Skelton" ,   {120.0f,140.0f,100.0f}},
		{"Bear"    ,   {300.0f,200.0f,300.0f}},
	};


	AnimInfo Animas[enEnemy_Num] = {
		{"Jumping"},
		{""},
		{""}
	};


	AnimInfo enemy1[1] = { "Jumping" };


	AnimInfo enemy2[8] = { 
		"SkeltonWalk"     ,"SkeltonIdle"    ,"SkeltonFall",
		"SkeltonTurnRight","SkeltonTurnLeft","SkeltonRun",
		"SkeltonSlash01"  ,"SkeltonDeth"
	};


	AnimInfo boss[9] = { 
		"Walk","Idle","Hit","Buff",
		"Sleep","Run","Eat","Attack1",
		"Death"
	};


	Vector3 ENEMY_GHOSTOBJ_POS = { 100.0f,200.0f,300.0f };//敵の視認範囲用のゴーストオブジェクト。

	const float CHARACON_RADIUS = 30.0f ;//カプセルコライダーの半径。
	const float CHARACON_HEIGHT = 25.0f;//カプセルコライダーの高さ。

	const float ENEMY_ATTACK_RANGE = 140.0f;//Enemyの攻撃判定の範囲。
	const float ENEMY_RANGE      = 120.0f;//Enemyの追従判定の範囲。
	const float ENEMY_MOVESPEED  = 120.0f;//Eenemyの移動速度。
	
	const Vector3 COLL_PLASS_POS = { 0.0f,70.0f,0.0f };//キャラコンの調整用namespace。

	const float DISTANCE = 200.0f;
	Vector3 ENEMY_LIMIT = { 400.0f,0.0f,400.0f };
	const float ENEMY_GRAVITY = 8.0f * 1.4;   //重力。
	const float ENEMY_MOVE_LIMIT = 150.0f;//Enemyの行動距離。
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
	m_animationClips.Load("Assets/EnmyAnimData/Jumping.tka");
	m_animationClips.SetLoopFlag(true);
	m_enemyRender.SetAnimationSpeed(0.6f);
	std::string e1ModelFile = Enemys[enEnemy1].GetFullPath();
	m_enemyRender.Init(e1ModelFile.c_str(), &m_animationClips,1, enModelUpAxisZ);
	

	m_gekihaHp = 1;


	SetModel(enEnemy1);
	SetCharacon(enEnemy1);
	SetCollisionObj(enEnemy1);
	EnemyBehavior();
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}


void Enemy1::Update()
{
	const bool& isPause = Game::GetIsPause();
	if (g_pad[0]->IsTrigger(enButtonSelect)) {
		Game::SetIsPause(!isPause);
	}
	if (isPause) {
		return;
	}

	/** プレイヤーへの視認チェック */
	IsFoundPlayer();

	/** 行動ステートと速度を決定 */
	EnemyBehavior();

	/** 重力と実際の移動を実行 */
	UpdateEnemyInfo();

	/** アニメーションの再生 */
	PlayAnimation();

	/** コリジョンの更新 */
	if (m_collisionObj) {
		m_collisionObj->SetPosition(m_enemyPos + COLL_PLASS_POS);
		m_collisionObj->SetRotation(Quaternion::Identity);
		m_collisionObj->Update();
	}

	/** ヒットチェック */
	EnemyHit();
	CanHit();

	/** 描画位置更新 */
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.Update();
}


bool Enemy2::Start() {
	m_enemy2AtCollisionObject = new CollisionObject;
	m_enemy2AtCollisionObject->Activate();
	Vector3 atPos = m_enemyPos * 50.0f;
	m_enemy2AtCollisionObject->CreateSphere(
		atPos,
		m_enemyRotation,
		70.0f
	);
	m_enemy2AtCollisionObject->Deactivate();

	for (int i = 0; i <= enEnemy2AnimClip_Deth; i++)
	{
		std::string animfile = enemy2[i].GetAnimPath();
		m_animationClips[i].Load(animfile.c_str());
		if (i == enEnemy2AnimClip_Run)
		{
			m_animationClips[i].SetLoopFlag(false);
		}
		else {
			m_animationClips[i].SetLoopFlag(true);
		}
	}
	std::string e2ModelFile = Enemys[enEnemy2].GetFullPath();
	m_enemyRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName)
		{
			(void)clipName;
			m_isAttack = true;
			if (wcscmp(eventName, L"Attack") == 0)
			{
				m_isAttack = true;
			}

			else if (wcscmp(eventName, L"Attack_End") == 0)
			{
				m_isAttack = false;
			}

		});
	m_enemyRender.Init(e2ModelFile.c_str(), m_animationClips, 8, enModelUpAxisZ);
	m_enemyRender.SetAnimationSpeed(0.5f);


	m_gekihaHp = 2;


	AnimationManager();
	SetModel(enEnemy2);
	SetCharacon(enEnemy2);
	SetCollisionObj(enEnemy2);
	EnemyBehavior();
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}


void Enemy2::Update()
{
	const bool& isPause = Game::GetIsPause();
	if (g_pad[0]->IsTrigger(enButtonSelect)) {
		Game::SetIsPause(!isPause);
	}
	if (isPause) {
		return;
	}

	/** プレイヤーへの視認チェック */
	IsFoundPlayer();

	/** 行動ステートと速度を決定 */
	EnemyBehavior();

	/** 重力と実際の移動を実行 */
	UpdateEnemyInfo();

	/** アニメーションの再生 */
	PlayAnimation();

	/** コリジョンの更新 */
	if (m_collisionObj) {
		m_collisionObj->SetPosition(m_enemyPos + COLL_PLASS_POS);
		m_collisionObj->SetRotation(Quaternion::Identity);
		m_collisionObj->Update();
	}

	/** ヒットチェック */
	EnemyHit();
	CanHit();

	/** 描画位置更新 */
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.Update();
}


bool Boss::Start() {
	m_bossAtCollisionObject = new CollisionObject;
	m_bossAtCollisionObject->Activate();
	Vector3 CollPos = m_enemyPos * 70.0f;
	m_bossAtCollisionObject->CreateSphere(
		CollPos,
		m_enemyRotation,
		80.0f
	);
	m_bossAtCollisionObject->Deactivate();


	for (int i = 0; i < enBossAnimClip_Num; i++)
	{
		std::string bAnimFile = boss[i].GetAnimPath();
		m_animationClips[i].Load(bAnimFile.c_str());
		if (i != enBossAnimClip_Death)
		{
			m_animationClips[i].SetLoopFlag(false);
		}
		else {
			m_animationClips[i].SetLoopFlag(true);
		}
	}
	std::string bossFile = Enemys[enBoss].GetFullPath();
	m_enemyRender.SetAnimationSpeed(0.3f);
	m_enemyRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		(void)clipName;
		m_isAttack = true;
		if (wcscmp(eventName, L"B_Attack_Start") == 0)
		{
			m_isAttack = true;
		}
		else if (wcscmp(eventName, L"B_Attack_End") == 0)
		{
			m_isAttack = false;
		}
	});
	m_enemyRender.Init(bossFile.c_str(), m_animationClips, 9, enModelUpAxisZ);


	m_gekihaHp = 5;


	AnimationManager();
	SetModel(enBoss);
	SetCharacon(enBoss);
	SetCollisionObj(enBoss);
	EnemyBehavior();
	SetSphereColliderObj();
	SetFindGOInfo();
	return true;
}


void Boss::Update()
{
	const bool& isPause = Game::GetIsPause();
	if (g_pad[0]->IsTrigger(enButtonSelect)) {
		Game::SetIsPause(!isPause);
	}
	if (isPause) {
		return;
	}

	/** プレイヤーへの視認チェック */
	IsFoundPlayer();

	/** 行動ステートと速度を決定 */
	EnemyBehavior();

	/** 重力と実際の移動を実行 */
	UpdateEnemyInfo();

	/** アニメーションの再生 */
	PlayAnimation();

	/** コリジョンの更新 */
	if (m_collisionObj) {
		m_collisionObj->SetPosition(m_enemyPos + COLL_PLASS_POS);
		m_collisionObj->SetRotation(Quaternion::Identity);
		m_collisionObj->Update();
	}

	/** ヒットチェック */
	EnemyHit();
	CanHit();

	/** 描画位置更新 */
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.Update();
}


Enemy::~Enemy()
{
	if (m_collisionObj != nullptr)
	{
		delete m_collisionObj;
		m_collisionObj = nullptr;
	}
}


void Enemy1::PlayAnimation()
{
	/** Jumping.tkaを再生 */
	m_enemyRender.PlayAnimation(0);
}


/** Enemy2のアニメーション管理 */
void Enemy2::PlayAnimation()
{
	switch (m_enemyActionState)
	{
	case enEnemyActionState_Idle:
		m_animStatus = enEnemy2AnimClip_Idle;
		break;
	case enEnemyActionState_Wandering:
		m_animStatus = enEnemy2AnimClip_Walk;
		break;
	case enEnemyActionState_Run:
		m_animStatus = enEnemy2AnimClip_Run;
		break;
	case enEnemyActionState_Attack:
		m_animStatus = enEnemy2AnimClip_Slash01;
		break;
	case enEnemyActionState_Deth:
		m_animStatus = enEnemy2AnimClip_Deth;
		break;
	case enEnemyActionState_Situ:
		m_animStatus = enEnemy2AnimClip_Fall;
		break;
	}
	m_enemyRender.PlayAnimation(m_animStatus);
}



/** ボスのアニメーション管理 */
void Boss::PlayAnimation()
{
	switch (m_enemyActionState)
	{
	case enEnemyActionState_Idle:
		m_animStatus = enBossAnimClip_Idle;		
		break;
	case enEnemyActionState_Wandering:
		m_animStatus = enBossAnimClip_Walk;
		break;
	case enEnemyActionState_Run:
		m_animStatus = enBossAnimClip_Run;
		break;
	case enEnemyActionState_Attack:
		m_animStatus = enBossAnimClip_Attack1;
		break;
	case enEnemyActionState_Deth:
		m_animStatus = enBossAnimClip_Death;
		break;
	case enEnemyActionState_Situ:
		m_animStatus = enBossAnimClip_Sleep;
		break;
	}

	m_enemyRender.PlayAnimation(m_animStatus);
}


/** Enemy1,Enemy2,Bossのアニメーションステート */
void Enemy::AnimationManager()
{
	/** 待機アニメーション */
	if (m_charaCon.IsOnGround())
	{
		m_enemyActionState = enEnemyActionState_Idle;
	}
	/** 追跡のアニメーション */
	if (!IsFoundPlayer())
	{
		m_enemyActionState = enEnemyActionState_Chase;
	}
	else if(IsFoundPlayer()){
		m_enemyActionState = enEnemyActionState_Wandering;
	}
}


//キャラコンの初期化関数。
void Enemy::SetCharacon(int enemyModels)
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

	//std::string file = Enemys[enemyModel].GetFullPath();
	//敵の読み込み。
	//m_enemyRender.Init(file.c_str(),);
	m_enemyRender.SetTRS(m_enemyPos, Quaternion::Identity, m_enemyScale);
	m_enemyRender.Update();
}

//コリジョンオブジェクト初期化関数。
void Enemy::SetCollisionObj(int enemyModel)
{
	m_collisionObj = new CollisionObject;

	//コリジョンオブジェクトの初期化。
	m_collisionObj->CreateBox(
		m_enemyPos,
		Quaternion::Identity,
		Enemys[enemyModel].collisionSc
	);

	m_collisionObj->SetPosition(m_enemyPos);
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
	/*if (!IsFoundPlayer()) {
		RandomWalk();
	}
	else if(IsFoundPlayer()){
		Tracking();
	}*/
}

//追跡の関数。
void Enemy::Tracking()
{
	//nullチェック。
	if (m_player == nullptr)
	{
		m_player = FindGO<Player>("player");
	}

	if (m_isSearchPlayer == true)
	{
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

	/** todo::追加 */
	if (!m_isSearchPlayer) {
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
	}

	m_moveSpeed = early;
	m_enemyRotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//UpdateEnemyInfo();
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
	/** TODO::修正するところ */
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
	}

	if (m_player != nullptr)
	{
		m_isSearchPlayer = false;
		return false;
	}
	if (m_player == nullptr)
	{
		m_isSearchPlayer = false;
		return false;
	}
	m_enemyRotation.Apply(FORWARD);
	
	Vector3 playerPos = m_player->GetPosition();
	Vector3 diff = playerPos - m_enemyPos;

	//ベクトルを正規化する。
	diff.Normalize();
	//ベクトルの内積の計算。
	float angle = acosf(diff.Dot(FORWARD));

	/** todo::プレイヤーが視界以外にいるならfalse */
	if (fabsf(angle) > Math::PI * 0.35f)
	{
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
		m_isSearchPlayer = false;
		return false;
	}
	m_isSearchPlayer = true;
	return true;
}

void Enemy::UpdateEnemyInfo() {
	float gTime = g_gameTime->GetFrameDeltaTime();
	if (m_charaCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}
	else if (m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y -= ENEMY_GRAVITY * gTime;
	}
	m_enemyPos = m_charaCon.Execute(m_moveSpeed, gTime);
	m_enemyRender.SetPosition(m_enemyPos);
	m_enemyRender.SetRotation(m_enemyRotation);
	m_enemyRender.Update();
}

//行動の関数。
void Enemy::EnemyBehavior()
{
	m_player = FindGO<Player>("player");
	/** nullチェック */
	if (!m_player)
	{
		return;
	}
	
	//プレイヤーとの距離を測る。
	Vector3 diff = m_player->GetPosition() - m_enemyPos;
	float distance = diff.Length();


	/**
	 * 攻撃処理 
	 */
	if (distance <= ENEMY_ATTACK_RANGE)
	{
		/** 攻撃ステート */
		m_enemyActionState = enEnemyActionState_Attack;
		/** 攻撃中は移動を停止 */
		m_moveSpeed = Vector3::Zero;

		/** 敵の向きをプレイヤー方向に向ける */
		Vector3 direction = diff;
		direction.y = 0.0f;
		m_enemyRotation.SetRotationYFromDirectionXZ(direction);
	}


	/**
	 * 追跡処理 
	 */
	//一定距離内に入ると追跡モードに切り替わる。
	else if (distance <= ENEMY_RANGE || m_isSearchPlayer)
	{
		m_enemyActionState = enEnemyActionState_Chase;
		if (m_isSearchPlayer)
		{
			/** TODO::修正 */
			/** プレイヤーの座標をローカル変数に渡す */
			Vector3 playerPos = m_player->GetPosition();
			/** プレイヤーからエネミーへのベクトルを測る */
			Vector3 diff_to_player = playerPos - m_enemyPos;
			/** エネミーのY座標は0に合わせる */
			diff_to_player.y = 0.0f;
			diff_to_player.Normalize();

			/** 追跡した時の速度 */
			float targetSpeed = 200.0f;
			/** 追跡速度の設定 */
			m_moveSpeed = diff_to_player * targetSpeed;

			/** エネミーの回転を設定 */
			m_enemyRotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		}
		else {
			/** 見失った場合は停止(速度を0) */
			m_moveSpeed = Vector3::Zero;
		}
	}
	//一定距離外なら徘徊モードに切り替わる。
	else {
		m_enemyActionState = enEnemyActionState_Wandering;
		RandomWalk();
	}

	
}

//EnemyがPlayerに衝突したらダメージを与える処理。
void Enemy::EnemyHit()
{
	if (!m_isAttack) {
		return;
	}
	if (!m_player) {
		m_player = FindGO<Player>("player");
	}
	if (!m_player)
	{
		return;
	}

	//プレイヤーの体についてるコリジョンに当たったら
	if (m_collisionObj->IsHit(m_player->GetCharacterController() ))
	{
		m_player->ReceiveDamage(1,m_enemyPos);
	}
}


/** Enemy2がPlayerに衝突したらダメージを与える */
void Enemy2::EnemyHit()
{
	if (!m_isAttack)
	{
		return;
	}
	if (!m_player) {
		m_player = FindGO<Player>("player");
	}
	if (!m_player) {
		return;
	}

	if (m_enemy2AtCollisionObject->IsHit(m_player->GetCharacterController() ))
	{
		m_player->ReceiveDamage(1, m_enemyPos);
	}
}


void Boss::EnemyHit()
{
	if (!m_isAttack)
	{
		return;
	}
	if (!m_player) {
		m_player = FindGO<Player>("player");
	}
	if (!m_player) {
		return;
	}

	if (m_bossAtCollisionObject->IsHit(m_player->GetCharacterController() ))
	{
		SoundManager* sound = FindGO<SoundManager>("soundManager");
		m_e_DamageSe = sound->PlayingSound(Sound::enSound_EnemyAttackSe, false, 2.0f);
		m_player->ReceiveDamage(1, m_enemyPos);
	}
}


void Enemy::CanHit()
{
	/** プレイヤーとコリジョンのnullチェック */
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
	}
	if (!m_player) {
		return;
	}


	float enemyHeight = m_enemyPos.y + CHARACON_HEIGHT * 2.0f;
	if (m_player->GetPosition().y <= enemyHeight) {
		return;
	}

	if (!m_collisionObj->IsHit(m_player->GetCollisionObj())) {
		return;
	}


	SoundManager* sound = FindGO<SoundManager>("soundManager");
	m_gekihaSe = sound->PlayingSound(Sound::enSound_GekihaSe, false, 1.8f);
	m_player->force.y = 190.0f;

	/** TODO:修正 */
	if (m_gekihaHp < 0)
	{
		/** 死亡処理 */
		Death();
		DeleteGO(m_gekihaSe);
		return;
	}
	m_gekihaHp--;
	return;
}


/** TODO:死亡アニメーション追加 */
void Enemy2::Death()
{
	m_enemyRender.PlayAnimation(enEnemyActionState_Deth);
	m_enemyRender.SetAnimationSpeed(0.6f);
	m_isDead = true;
	DeleteGO(this);
}


/** TODO:死亡アニメーション追加 */
void Boss::Death()
{
	m_enemyRender.PlayAnimation(enEnemyActionState_Deth);
	m_enemyRender.SetAnimationSpeed(0.6f);
	if (Game::GetIsBossKill) {
		Game::SetIsBossKill(true);
	}
	DeleteGO(this);
}


/** 死亡処理 */
void Enemy::Death()
{
	m_enemyRender.PlayAnimation(enEnemyActionState_Deth);
	m_enemyRender.SetAnimationSpeed(0.6f);
	DeleteGO(this);
}


/** Enemy2の攻撃の時だけコリジョンを出す */
bool Enemy2::AttackCollision()
{
	if (!m_enemy2AtCollisionObject)
	{
		return false;
	}

	if (m_isAttack)
	{
		m_enemyRotation.Apply(FORWARD);
		m_enemy2AtCollisionObject->SetPosition(m_enemyPos + FORWARD * 50.0f);
		m_enemy2AtCollisionObject->Activate();
	}
	else {
		m_enemy2AtCollisionObject->Deactivate();
	}
	return true;
}


/** ボスの攻撃する時だけコリジョンを出す */
bool Boss::AttackCollision()
{
	if (!m_bossAtCollisionObject)
	{
		return false;
	}

	if (m_isAttack)
	{
		m_enemyRotation.Apply(FORWARD);
		m_bossAtCollisionObject->SetPosition(m_enemyPos + FORWARD * 50.0f);
		m_bossAtCollisionObject->Activate();
	}
	else {
		m_bossAtCollisionObject->Deactivate();
	}
	return true;
}


void Enemy::Attack()
{
	
}

void Enemy::Render(RenderContext& rc)
{
	//敵の描画。
	m_enemyRender.Draw(rc);
}