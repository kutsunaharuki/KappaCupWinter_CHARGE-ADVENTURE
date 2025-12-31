#pragma once
//#include "physics/PhysicsGhostObject.h"
#include "stdint.h"


class MovingFloor;
class Enemy;
class HPUI;
class GameOver;
class Poal;
class Game;
class ChargeItem;
class ChargeBar;

enum class ChargeAbility {
	enNone,
	enSpeshal_ChargeDash,
	enSpeshal_ChargeJump
};


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
	
public:
	void Charge();
	void ChargeManager();
	void UpdateChargeUI();
	void MoveCharge();
	void FlashDash();//瞬間加速バフ。
	void ChargeDash();//チャージダッシュ。
	void ChargeJump();//チャージジャンプ。
	bool CostItem(int cost);//アイテム消費。

	const bool IsCharge();


public:
	void SetTRS(Vector3& pos, Quaternion& rot, Vector3& scl)
	{
		m_position = pos;
		m_rot = rot;
		m_scale = scl;
	}

	
	//bool  isDead()const   { return hp < 0;  } //プレイヤーが死亡時。
	int GetHp   ()const   { return hp;      } //プレイヤーのHPの保持。
	int GetMaxHp()const   { return maxHp;   } //プレイヤーの最大HPの保持。
	int hp    = 3;                            //HP。
	int maxHp = 3;                            //最大HP。

	
	Vector3 force = Vector3::Zero;//外部から加える力(敵を踏んだ時にY座標を上げる用)。
	
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
	
	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //ワープ先。

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //フォントの座標。

	
	Vector3 m_reStartPos = Vector3::Zero;//戻される座標。

	Vector3 m_backSpeed = Vector3::Zero;                //ノックバック速度を保持するための変数。

	FontRender m_posFontRender;                         //座標の描画。
	CharacterController m_charaCon;                     //TODO:キャラクターコントローラーの当たり判定。
	CollisionObject* m_collisionObj = nullptr;                 //TODO:コリジョンオブジェクト。
	float m_jumpTime = 0.0f;                            //ジャンプしてる時間。	
	float m_invinCibilityTime = 0.0f;                   //無敵時間。
	float m_knockBackTime = 0.0f;                       //プレイヤーを飛ばす時間。
	float m_deltaTime = g_gameTime->GetFrameDeltaTime();


	bool isDash  = false;                            //歩きから走りに変わるフラグ。
	bool isHit   = false;                            //ゴーストオブジェクトに当たったらのフラグ。
	bool canJump = false;                            //ジャンプしてるかのフラグ。

	
	/** プレイヤー追加要素 */
	ChargeAbility m_chargeAbilitys = ChargeAbility::enNone;
	bool m_isChargeDash = false;                     //チャージダッシュが出来るかのフラグ。
	bool m_isChargeJump = false;                     //チャージジャンプが出来るかのフラグ。
	bool m_isCharge = false;                         //チャージができるかのフラグ。


	Vector3 m_baseMoveSpeed = Vector3::Zero;//基本の速度。
	Vector3 m_itemBonusSpeed = Vector3::Zero;//アイテムの累積ボーナス速度。
	Vector3 m_chargeBonusSpeed = Vector3::Zero;//チャージ中の速度。

	/** 累積加速型 */
	/** チェージジャンプ専用アイテムカウント */
	int m_itemCount = 0;
	//int m_minValue = 5;//チャージダッシュ用の必要アイテム数。
	//int m_maxValue = 10;//チャージジャンプ用の必要アイテム数。
	float m_cumulative = 0.0f;//累積。
	float m_chargeTimer = 0.0f;//チャージ時間。

	/** 瞬間的加速型 */
	float m_topSpeed = 0.0f;//加速力。
	float m_topSpeedTime = 0.0f;//加速時間。
	float m_finalSpeed = 0.0f;//最終的な加速量。

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
	ChargeItem* m_chargeItem = nullptr;
	ChargeBar* m_chargeBar = nullptr;
	int playerState    = 0;

	MovingFloor* m_movingFloor = nullptr;


	FontRender m_debugFont;
};

