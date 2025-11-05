#pragma once
//#include "physics/PhysicsGhostObject.h"
#include "stdint.h"


class Warp;
class MovingFloor;
class SkyGround;
class Enemy;
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
	void ResPawn();

private:
	void Move();
	void Rotation();
	const bool IsMove()const;
	const bool JumpAttack()const;

public:
	Vector3 force = Vector3::Zero;//外部から加える力(敵を踏んだ時にY座標を上げる用)。
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//キャラクターコントローラーの取得。
	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}


	enum class EnAnimationClip : uint8_t{
		enAnimationClip_Idle,
		enAnimationClip_Jump,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};

	ModelRender m_modelRender;
	AnimationClip animationClips[static_cast<int>(EnAnimationClip::enAnimationClip_Num)];
	Vector3 m_position = Vector3::Zero;                 //TODO:座標。
	Vector3 moveSpeed = Vector3::Zero;                  //TODO:移動速度。
	Quaternion m_rot;                                   //TODO:回転。
	Vector3 m_footCollisionPos = Vector3::Zero;         //TODO:足のコリジョンの座標。
	Vector3 m_playerCollisionScale = Vector3(35.0f, 10.0f, 35.0f);//TODO:プレイヤーのコリジョンのサイズ。

	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //ワープ先。

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //フォントの座標。
	
	Vector3 m_resPawnPos = Vector3::Zero;               //初期値に戻される値。

	FontRender m_posFontRender;                         //座標の描画。

	CharacterController m_charaCon;                     //TODO:キャラクターコントローラーの当たり判定。
	CollisionObject* m_collisionObj = nullptr;               //TODO:コリジョンオブジェクト。

	float m_jumpTime = 0.0f;                            //ジャンプしてる時間。	
	
	bool isDash     = false;                            //歩きから走りに変わるフラグ。
	bool isHit      = false;                            //ゴーストオブジェクトに当たったらのフラグ。
	bool canJump    = false;                            //ジャンプしてるかのフラグ。
private:
	Warp* m_warp   = nullptr;
	Enemy* m_enemy = nullptr;
	int playerState    = 0;

	MovingFloor* m_movingFloor = nullptr;
	SkyGround  * m_skyGround   = nullptr;
};

