#pragma once
class Player;

enum FallStage {
	enFall_Idle,//待機状態
	enFall,     //落下
	enFall_Back //再配置(戻る)
};

class FallBox : public IGameObject
{
public:
	FallBox();
	~FallBox();


public:
	FallStage m_fall = FallStage::enFall_Idle;

	void OnIdle();//待機中。
	void OnFall();//落下中。
	void OnBack();//復帰中。

	Vector3 m_initPos;       //初期位置。
	float m_fallTimer = 0.0f;//落下猶予タイマー。
	float m_stayTimer = 0.0f;//下で止まるタイマー。

	bool m_isPlayerOn = false;

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	bool FallMove();
	
	void SetCollision();

	//落ちる速度。
	float m_sinkSpeed = 0.0f;
	
	ModelRender m_fallBoxRender;
	PhysicsStaticObject m_fallPhysicsObj;
	Vector3 m_fallPos = Vector3::Zero;
	Vector3 m_fallScale = Vector3::One;
	Quaternion m_fallRot = Quaternion::Identity;
	CollisionObject* m_fallCollisionObj;

private:
	Player* m_player = nullptr;
};

