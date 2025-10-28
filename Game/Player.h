#pragma once
#include "physics/PhysicsGhostObject.h"
#include "stdint.h"


class Warp;
class MovingFloor;
class SkyGround;
class Enemy;
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start()override;
	void Update();
	
	void ManageState();
    void Render(RenderContext& rc);


private:
	void Move();
	void Rotation();
	const bool IsMove()const;

public:
	bool CanHit();

	void Damage()
	{
		hp -= damage;
		if (hp < 0)
		{
			hp = 0;
		}
	}

	int hp     = 20;
	int maxHp  = 20;
	int damage = 10;

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void  SetPosition(const Vector3& position)
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
	Vector3 m_position = Vector3::Zero;                 //座標。
	Vector3 moveSpeed = Vector3::Zero;                  //移動速度。
	Quaternion m_rot;                                   //回転。


	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //ワープ先。

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //フォントの座標。
	FontRender m_posFontRender;                         //座標の描画。

	CharacterController m_charaCon;                     //キャラクターコントローラーの当たり判定。
	
	float m_jumpTime = 0.0f;                            //ジャンプしてる時間。	
	
	bool isDash     = false;                            //歩きから走りに変わるフラグ。
	bool isHit      = false;                            //ゴーストオブジェクトに当たったらのフラグ。
	bool canJump    = false;                            //ジャンプしてるかのフラグ。
private:
	Warp* m_warp   = nullptr;
	Enemy* m_enemy = nullptr;
	int playerState    = 0;

	//bool canJump = true;                               //切り替え。
	MovingFloor* m_movingFloor = nullptr;
	SkyGround* m_skyGround = nullptr;
	
};

