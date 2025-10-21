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
	PhysicsStaticObject m_physicsStaticObj;        //�G1�̓����蔻��B
	Vector3 m_enemy2Pos = Vector3::Zero;           //�G2�̍��W�B
	Vector3 moveSpeed = Vector3::Zero;             //�G2�̈ړ����x�B
	Vector3 m_enemy2Scale = Vector3::One;          //�G2�̑傫���B
	Quaternion m_enemy2Rotation;                   //�G2�̉�]�B

	Player* m_player = nullptr;
};

