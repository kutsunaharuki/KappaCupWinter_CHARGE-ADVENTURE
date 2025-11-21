#pragma once
class Player;
class ObstacleBox;

enum EnEnemy {
	enEnemy1,//ファイル番号0(クリボー)。
	enEnemy2,//ファイル番号1(ノコノコ)。
	enBoss,  //ファイル番号2(クッパ)。
	enEnemy_Num
};

enum EnEnemyActionState {
	enEnemyActionState_Wandering,        //徘徊。
	enEnemyActionState_Chase,            //追跡。
	enEnemyActionState_Attack,           //攻撃。
	enEnemyActionState_Num               //総数(4個ある)。
};

class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	virtual bool Start()override;
	virtual void Update()override;
	void Move();
	void UpdateEnemyInfo();
	void RandomWalk();
	void Attack();
	void EnemyBehavior();
	void Tracking();
	const bool IsFoundPlayer();
	virtual void Render(RenderContext& rc);
	void EnemyHit();


	//inlineは関数に入らずに別のクラス内に持ち込むことができる。
	inline void SetPosition(const Vector3& pos) { m_enemyPos = pos; }
	inline void SetScale(const Vector3& scl) { m_enemyScale = scl; }
	inline void SetRotation(const Quaternion& rot) { m_enemyRotation = rot; }


	//inlineの中身は3行以内ですますように!
	inline void SetTRS(const Vector3& pos, const Vector3& scl, const Quaternion& rot)
	{
		m_enemyPos = pos;
		m_enemyScale = scl;
		m_enemyRotation = rot;
	}

	inline CollisionObject* GetCollision() { return m_collisionObj; }

protected:
	enum EnWalkVector {
		enWalkVector_Front,      //前。
		enWalkVector_Back,       //後ろ。
		enWalkVector_Right,      //右。
		enWalkVector_Left,       //左。
		enWalkVector_FrontRight, //右斜め前。
		enWalkVector_FronLeft,   //左斜め前。
		enWalkVector_BackRight,  //右斜め後ろ。
		enWalkVector_BackLeft,   //左斜め後ろ。
		enWalkVector_Num
	};

	ModelRender m_enemyRender;
	Quaternion m_enemyRotation = Quaternion::Identity;//回転。
	CharacterController m_charaCon;                   //キャラクターコントローラー。
	CollisionObject* m_collisionObj;                  //コリジョンオブジェクト。

	Vector3 m_collisionObjStartPos = Vector3::Zero;   //敵のコリジョンの最初の座標。
	Vector3 m_enemyStartPos = Vector3::Zero;          //敵の最初座標。
	Vector3 m_enemyPos = Vector3::Zero;               //敵の座標。
	Vector3 m_enemyMoveSpeed = Vector3::Zero;         //敵の移動速度。
	Vector3 m_enemyScale = Vector3::One;              //敵の大きさ。
	//Vector3 m_enemyCollisionScale = Vector3(130.0f, 100.0f, 130.0f);//コリジョンのサイズ。
	Vector3 m_moveSpeed = Vector3::Zero;              //敵の移動速度。
	

	bool isHit = false;
	bool m_isSearchPlayer = false;
	
	SphereCollider m_sphereCollider;
	FontRender m_fontRender;
	Vector3 m_fontPos = Vector3(-400.0f, 500.0f, 0.0f);

	std::array<AnimationClip, enEnemyActionState_Num> m_animationClips;

	//中身を変えたくない時は後ろにconstをつける。
	void CanHit();

	
	//m_enemyActionStateに持たせる。
	EnEnemyActionState m_enemyActionState = enEnemyActionState_Wandering;

protected:
	//protected
	//派生クラスで使える。他の単体のクラスでは使えない。
	void SetModel(int enemyModel);
	void SetPhysicsGameObj(int enemyModels);
	void SetCollisionObj(int enemyModel);
	void SetFindGOInfo();
	void SetSphereColliderObj();
	//void SetAnimData(int enemyModel);
	//const bool EnemySweepTest(int enemyModel);	
	
private:
	FontRender m_collisionFontRender;

	int m_enemyMoveState = 0;
	int enemyState = 0;
	Player* m_player = nullptr;
	ObstacleBox* m_obstacleBox = nullptr;
};

//基底クラスはEnemy。
class Enemy1 : public Enemy
{
public:
	bool Start()override;
	//~Enemy1()override;
};

class Enemy2 : public Enemy
{
public:
	bool Start()override;
	//~Enemy2()override;
};

class Boss : public Enemy
{
public:
	bool Start()override;
	//~Boss()override;
};