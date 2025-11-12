#pragma once
class SkyGround : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc);
	ModelRender m_skyGroundRender;
	Vector3 m_skyGroundPos = Vector3::Zero;
	Vector3 m_skyGroundSc = Vector3::One;
	PhysicsStaticObject m_physicsObj;
};

