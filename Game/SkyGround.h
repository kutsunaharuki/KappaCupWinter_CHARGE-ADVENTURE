#pragma once
class Player;
class SkyGround:public IGameObject
{
public:
	SkyGround() {};
	~SkyGround() {};
	bool Start()override;
	void Render(RenderContext& rc);

	ModelRender m_skyGroundRender;
	Vector3 m_pos = Vector3( -200.0f, 50.0f, 100.0f);
	Vector3 m_scale = Vector3(10.0f, 10.0f, 10.0f);
	PhysicsStaticObject m_physicsStaticObj;
	
private:
	Player* m_player = nullptr;
};

