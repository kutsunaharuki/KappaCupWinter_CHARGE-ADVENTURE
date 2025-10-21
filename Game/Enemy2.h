#pragma once
class Player;
class Enemy2 : public IGameObject
{
public:
	Enemy2() {};
	~Enemy2() {};

	bool Start()override;
	void Update();
	void Move();
	void Damage(int damage);
	void Render(RenderContext& rc);

	ModelRender m_enemy2Render;
	PhysicsStaticObject m_physicsStaticObj;        //“G1‚Ì“–‚½‚è”»’èB
	Vector3 m_enemy2Pos = Vector3::Zero;           //“G2‚ÌÀ•WB
	Vector3 moveSpeed = Vector3::Zero;             //“G2‚ÌˆÚ“®‘¬“xB
	Vector3 m_enemy2Scale = Vector3::One;          //“G2‚Ì‘å‚«‚³B
	Quaternion m_enemy2Rotation;                   //“G2‚Ì‰ñ“]B

	Player* m_player = nullptr;
};

