#pragma once
//#include "physics/PhysicsGhostObject.h"
#include "stdint.h"


class MovingFloor;
class Enemy;
class HPUI;
class GameOver;
class Poal;
class Game;

class Player:public IGameObject
{
public:
	~Player();
	bool Start()override;
	void Update();
	
	void ManageState();
    void Render(RenderContext& rc);
	void TreaderCollisionObj();
	void FindGameObjInfo();
	void SetPlayerCollision();
	void ReceiveDamage(int damage, Vector3& enemyPos);
	//void ReStart();

	void SetTRS(Vector3& pos, Quaternion& rot, Vector3& scl)
	{
		m_position = pos;
		m_rot = rot;
		m_scale = scl;
	}

	//void ReStartPos();
	//void TreaderCollisionObj();

	//これは使わない。
	//HP計算。保持。
	//void HP(int damage)
	//{
	//	hp -= damage;
	//	if (hp < 0)
	//	{
	//		hp = 0;
	//	}
	//}
	/** チェージジャンプ専用アイテムカウント */
	int m_itemCount = 0;


	//bool  isDead()const   { return hp < 0;  } //プレイヤーが死亡時。
	int GetHp   ()const   { return hp;      } //プレイヤーのHPの保持。
	int GetMaxHp()const   { return maxHp;   } //プレイヤーの最大HPの保持。
	int hp    = 3;                            //HP。
	int maxHp = 3;                            //最大HP。

	
	Vector3 force = Vector3::Zero;//外部から加える力(敵を踏んだ時にY座標を上げる用)。
	
	//inline CollisionObject* GetBodyCollision() { return m_bodyCollisionObj; }

	void AddPosition(const Vector3& delta);

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	ModelRender* GetModelRender()
	{
		return &m_modelRender;
	}

	//キャラクターコントローラーの取得。
	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	CollisionObject* GetCollisionObj() {
		return m_collisionObj;
	}

	//DicrectXの中にあるenum型のメモリが浮く。
	enum class EnAnimationClip : uint8_t {
		enAnimationClip_Idle,
		enAnimationClip_Jump,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};

	ModelRender m_modelRender;
	AnimationClip animationClips[static_cast<int>(EnAnimationClip::enAnimationClip_Num)];
	Vector3 m_position = Vector3::Zero;                 //TODO:座標。
	Vector3 m_moveSpeed = Vector3::Zero;                  //TODO:移動速度。
	Quaternion m_rot;                                   //TODO:回転。
	Vector3 m_scale;
	Vector3 m_footCollisionPos = Vector3::Zero;         //TODO:足のコリジョンの座標。
	Vector3 m_playerCollisionScale = Vector3(35.0f, 10.0f, 35.0f);//TODO:プレイヤーのコリジョンのサイズ。
	Vector3 m_playerBodyCollisionSc = Vector3(42.0f, 140.0f, 42.0f);//プレイヤーのボディコリジョンのサイズ。敵のコリジョンに当たった時に必要。
	Vector3 m_playerBodyCollisionPos = Vector3::Zero;

	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //ワープ先。

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //フォントの座標。

	//Vector3 m_resPawnPos = Vector3::Zero;               //初期値に戻される値。

	Vector3 m_reStartPos = Vector3::Zero;//戻される座標。

	Vector3 m_backSpeed = Vector3::Zero;                //ノックバック速度を保持するための変数。

	FontRender m_posFontRender;                         //座標の描画。
	CharacterController m_charaCon;                     //TODO:キャラクターコントローラーの当たり判定。
	CollisionObject* m_collisionObj = nullptr;                 //TODO:コリジョンオブジェクト。
	//CollisionObject* m_bodyCollisionObj;
	float m_jumpTime = 0.0f;                            //ジャンプしてる時間。	
	float m_invinCibilityTime = 0.0f;                   //無敵時間。
	float m_knockBackTime = 0.0f;                       //プレイヤーを飛ばす時間。
	float m_deltaTime = g_gameTime->GetFrameDeltaTime();

	bool isDash  = false;                            //歩きから走りに変わるフラグ。
	bool isHit   = false;                            //ゴーストオブジェクトに当たったらのフラグ。
	bool canJump = false;                            //ジャンプしてるかのフラグ。


private:
	void Move();
	void Rotation();
	const bool IsMove()const;
	const bool JumpAttack()const;
	const bool EnemyCollisionHit()const;

	GameOver* m_gameOver = nullptr;
	HPUI* m_hpui = nullptr;
	Poal* m_poal = nullptr;
	Enemy* m_enemy = nullptr;
	int playerState    = 0;

	MovingFloor* m_movingFloor = nullptr;


	FontRender m_debugFont;
};

