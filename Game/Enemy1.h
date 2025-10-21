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
		enWandering,   //�p�j�B
		enCharge,      //�ːi�B
		enBump,        //�ՓˁB
		enAction_Num
	};

	EnEnemy1Action EnemyActions[enAction_Num];

	ModelRender m_enemy1Render;
	PhysicsStaticObject m_physicsStaticObj;                  //�G1�̓����蔻��B
	Vector3 m_enemy1Pos = Vector3(-100.0f, 0.0f, 300.0f);    //�G1�̍��W�B
	Vector3 m_enemy1MoveSpeed = Vector3::Zero;               //�G1�̈ړ����x�B
	Vector3 m_enemy1Scale = Vector3::One;                    //�G1�̑傫���B
	Quaternion m_enemy1Rotation;                             //�G1�̉�]�B

	Player* m_player = nullptr;

private:
	
};

