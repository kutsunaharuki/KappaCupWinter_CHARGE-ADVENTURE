#pragma once
class StageGround : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc);
	ModelRender m_stageGroundRender;
	PhysicsStaticObject m_physicsObj;
	Vector3 m_stageGroundPos = Vector3::Zero;
	Vector3 m_stageGroundSc = Vector3::One;
};

