#pragma once

class Warp;
class MovingFloor;
class SkyGround;
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start()override;
	void Update();
	void Move();
	void Rotation();
	void PlayAnimation();
	void ManageState();
    void Render(RenderContext& rc);

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

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Jump,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};

	ModelRender m_modelRender;
	AnimationClip animationClips[enAnimationClip_Num];
	Vector3 m_position = Vector3::Zero;                 //座標。
	Vector3 moveSpeed = Vector3::Zero;                  //移動速度。
	Quaternion m_rot;                                   //回転。


	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //ワープ先。

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //フォントの座標。
	FontRender m_posFontRender;                         //座標の描画。

	CharacterController m_charaCon;                     //キャラクターコントローラーの当たり判定。
	
	float m_jumpTime = 0.0f;                            //ジャンプしてる時間。	
	
	bool isDead     = false;                            //プレイヤーが死亡したら切り替わるフラグ。
	bool isDash     = false;                            //歩きから走りに変わるフラグ。
	bool chargeJump = true;                             //チャージジャンプのフラグ。
	bool isHit      = false;                            //ゴーストオブジェクトに当たったらのフラグ。
	bool canJump    = false;                            //ジャンプしてるかのフラグ。

private:
	Warp* m_warp = nullptr;

	int playerState    = 0;

	bool CanWarp();
	//bool canJump = true;                               //切り替え。
	MovingFloor* m_movingFloor = nullptr;
	SkyGround* m_skyGround = nullptr;
	
};

