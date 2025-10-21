#pragma once
class Player;
class Enemy1:public IGameObject
{
public:
	Enemy1() {};
	~Enemy1() {};
	bool Start()override;
	void Update();
	void Move();
	void Damage(int damage);
	void Render(RenderContext& rc);

	enum EnEnemy1Action{
		enWandering,   //œpœjB
		enCharge,      //“ËiB
		enBump,        //Õ“ËB
		enAction_Num
	};

	EnEnemy1Action EnemyActions[enAction_Num];

	ModelRender m_enemy1Render;
	PhysicsStaticObject m_physicsStaticObj;                  //“G1‚Ì“–‚½‚è”»’èB
	Vector3 m_enemy1Pos = Vector3(-100.0f, 0.0f, 300.0f);    //“G1‚ÌÀ•WB
	Vector3 m_enemy1MoveSpeed = Vector3::Zero;               //“G1‚ÌˆÚ“®‘¬“xB
	Vector3 m_enemy1Scale = Vector3::One;                    //“G1‚Ì‘å‚«‚³B
	Quaternion m_enemy1Rotation;                             //“G1‚Ì‰ñ“]B

	Player* m_player = nullptr;

private:
	
};

