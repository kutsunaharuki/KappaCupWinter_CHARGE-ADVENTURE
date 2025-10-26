#pragma once
//#include "physics/PhysicsGhostObject.h"
class Player;
class Enemy : public IGameObject
{
public:
	void Update()override;
	void Move();
	void Attack();
	void EnemyBehavior();
	void Render(RenderContext& rc);

protected:
	void SetModel(int enemyModel);
	void SetFindGOInfo();
public:
	enum EnEnemy {
		enEnemy1,//ファイル番号0(クリボー)。
		enEnemy2,//ファイル番号1(ノコノコ)。
		enBoss,  //ファイル番号2(クッパ)。
		enEnemy_Num
	};

public:
	ModelRender m_enemyRender;
	Quaternion m_enemyRotation = Quaternion::Identity;//回転。
	CharacterController m_charaCon;                   //キャラクターコントローラー。
	PhysicsGhostObject m_physicsGhostObj;             //ゴーストオブジェクト。

	Vector3 m_enemyStartPos  = Vector3::Zero;   //敵の最初座標。
	Vector3 m_enemyPos       = Vector3::Zero;   //敵の座標。
	Vector3 m_enemyMoveSpeed = Vector3::Zero;   //敵の移動速度。
	Vector3 m_enemyScale     = Vector3::One;    //敵の大きさ。

	bool isHit   = false;

private:
	enum EnEnemyActionState {
		enEnemyActionState_Wandering,        //徘徊。
		enEnemyActionState_Chase,            //追跡。
		enEnemyActionState_Idle,             //待機。
		enEnemyActionState_Attack,           //攻撃。
		enEnemyActionState_Num               //総数(4個ある)。
	};

	//m_enemyActionStateに持たせる。
	EnEnemyActionState m_enemyActionState = enEnemyActionState_Wandering;

public:
	enum EnWalkVector {
		enWalkVector_Front,      //前。
		enWalkVector_Back,       //後ろ。
		enWalkVector_Right,      //右。
		enWalkVector_Left,       //左。
		enWalkVector_FrontRight, //右斜め前。
		enWalkVector_FronLeft,   //左斜め前。
		enWalkVector_BackRight,  //右斜め後ろ。
		enWalkVector_BackLeft,   //左斜め後ろ。
		enWalkVector_Num         //総数(8個ある)。
	};

private:
	int m_enemyMoveState = 0;
	int enemyState = 0;
	Player* m_player = nullptr;
    //protected
	//派生クラスで使える。他の単体のクラスでは使えない。
};

//基底クラスはEnemy。
class Enemy1 : public Enemy
{
public:
	bool Start()override;
};

class Enemy2 : public Enemy
{
public:
	bool Start()override;
};

class Boss : public Enemy
{
public:
	bool Start()override;
};