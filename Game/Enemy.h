#pragma once
class Player;
class ObstacleBox;
class Score;


enum EnEnemy {
	enEnemy1,//ファイル番号0(クリボー)。
	enEnemy2,//ファイル番号1(ノコノコ)。
	enBoss,  //ファイル番号2(クッパ)。
	enEnemy_Num
};

enum EnEnemyActionState {
	enEnemyActionState_Idle,             //待機。
	enEnemyActionState_Wandering,        //徘徊。
	enEnemyActionState_Run,              //走り。
	enEnemyActionState_Chase,            //追跡。
	enEnemyActionState_Attack,           //攻撃。
	enEnemyActionState_Deth,             //死亡。
	enEnemyActionState_Jump,             //ジャンプ。
	enEnemyActionState_Situ,             //状態。
	enEnemyActionState_Num               
};


namespace {
	constexpr size_t ANIM_NUM[EnEnemy::enEnemy_Num] = {
		1, 8, 9
	};
}


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
	virtual void Attack();
	void EnemyBehavior();
	void Tracking();
	const bool IsFoundPlayer();
	virtual void Render(RenderContext& rc);
	void EnemyHit();
	void AnimationManager();

	//virtual void AnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


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


	/*enum  EnAnimationClip {
		enAnimClip_A,
		enAnimClip_B,
		enAnimClip_C,
		enAnimClip_D,
		enAnimClip_E,
		enAnimClip_F,
		enAnimClip_G,
		enAnimClip_H,
		enAnimClip_Num
	};*/

	ModelRender m_enemyRender;
	Quaternion m_enemyRotation = Quaternion::Identity;//回転。
	CharacterController m_charaCon;                   //キャラクターコントローラー。
	CollisionObject* m_collisionObj;                  //コリジョンオブジェクト。
	//CollisionObject* m_attackCollisionObj;            //アタック専用。


	Vector3 m_collisionObjStartPos = Vector3::Zero;   //敵のコリジョンの最初の座標。
	Vector3 m_enemyStartPos = Vector3::Zero;          //敵の最初座標。
	Vector3 m_enemyPos = Vector3::Zero;               //敵の座標。
	//Vector3 m_enemyMoveSpeed = Vector3::Zero;         //敵の移動速度(2)。
	Vector3 m_enemyScale = Vector3::One;              //敵の大きさ。
	Vector3 m_moveSpeed = Vector3::Zero;              //敵の移動速度。


	bool m_isAttack       = false;
	bool isHit            = false;
	bool m_isSearchPlayer = false;
	
	SphereCollider m_sphereCollider;
	FontRender m_fontRender;
	Vector3 m_fontPos = Vector3(-400.0f, 500.0f, 0.0f);

	//std::array<AnimationClip, enEnemyActionState_Num> m_animationClips;

	//中身を変えたくない時は後ろにconstをつける。
	void CanHit();

	
	//m_enemyActionStateに持たせる。
	EnEnemyActionState m_enemyActionState = enEnemyActionState_Idle;
	
protected:
	//protected
	//派生クラスで使える。他の単体のクラスでは使えない。
	void SetModel(int enemyModel);
	//void Rotation(int enemyModel);
	void SetPhysicsGameObj(int enemyModels);
	void SetCollisionObj(int enemyModel);
	void SetFindGOInfo();
	void SetSphereColliderObj();
	virtual void PlayAnimation() {};

private:
	FontRender m_collisionFontRender;


	int m_enemyMoveState = 0;
	int enemyState = 0;
	Player* m_player = nullptr;
	ObstacleBox* m_obstacleBox = nullptr;
	Score* m_score = nullptr;
};

//基底クラスはEnemy。
class Enemy1 : public Enemy
{
public:
	bool Start()override;
	void Attack()override {};
	void PlayAnimation()override;


private:
	AnimationClip m_animationClips;
	//~Enemy1()override;
};

class Enemy2 : public Enemy
{
private:
	/** Enemy2 */
	enum EnEnemy2AnimClip {
		enEnemy2AnimClip_Walk,
		enEnemy2AnimClip_Idle,
		enEnemy2AnimClip_Fall,
		enEnemy2AnimClip_TurnRight,
		enEnemy2AnimClip_TurnLeft,
		enEnemy2AnimClip_Run,
		enEnemy2AnimClip_Slash01,
		enEnemy2AnimClip_Deth,
		enEnemy2AnimClip_Num
	};


public:
	bool Start()override;
	void Attack()override {};
	void PlayAnimation()override;
	

private:
	AnimationClip m_animationClips[enEnemy2AnimClip_Num];
	EnEnemy2AnimClip m_animStatus;
	CollisionObject* m_enemy2AtCollisionObject;//敵2の攻撃専用のコリジョンオブジェクト
	void AnimationEvent(const wchar_t* clipName, const wchar_t* eventName) {};
	//~Enemy2()override;
};

class Boss : public Enemy
{
private:
	/** Boss */
	enum EnBossAnimClip {
		enBossAnimClip_Walk,
		enBossAnimClip_Idle,
		enBossAnimClip_Hit,
		enBossAnimClip_Buff,
		enBossAnimClip_Sleep,
		enBossAnimClip_Run,
		enBossAnimClip_Eat,
		enBossAnimClip_Attack1,
		enBossAnimClip_Death,
		enBossAnimClip_Num
	};


public:
	bool Start()override;
	void Attack()override {};
	void PlayAnimation()override;


private:
	AnimationClip m_animationClips[enBossAnimClip_Num];
	EnBossAnimClip m_animStatus;
	CollisionObject* m_bossAtCollisionObject;//攻撃専用のコリジョンオブジェクト。
	void AnimationEvent(const wchar_t* clipName, const wchar_t* eventName) {};
	//~Boss()override;
};