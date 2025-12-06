#pragma once
class Player;
class SinkScaffold : public IGameObject
{
public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetCollision();


public:
	ModelRender m_sinkScaffoldRender;
	PhysicsStaticObject m_sinkScaffoldPhysicsObj;
	Vector3 m_sinkScaffoldPos = Vector3::Zero;
	Vector3 m_sinkScaffoldScale = Vector3::One;
	Quaternion m_sinkScaffoldRot = Quaternion::Identity;
	CollisionObject* m_sinkCollision;

private:
	Player* m_player = nullptr;


private:
	
};

