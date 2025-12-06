#pragma once
class Player;
class FallBox : public IGameObject
{
public:
	FallBox();
	~FallBox();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	bool FallMove();
	
	void SetCollision();

	//落ちるかどうかのフラグ。
	bool m_isPlayerOn = false;
	//落ちる速度。
	float m_sinkSpeed = 50.0f;
	float m_deltaTime = g_gameTime->GetFrameDeltaTime();

	ModelRender m_fallBoxRender;
	PhysicsStaticObject m_fallPhysicsObj;
	Vector3 m_fallPos = Vector3::Zero;
	Vector3 m_fallScale = Vector3::One;
	Quaternion m_fallRot = Quaternion::Identity;
	CollisionObject* m_fallCollisionObj;

private:
	Player* m_player = nullptr;
};

