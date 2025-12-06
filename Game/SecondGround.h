#pragma once
class SecondGround : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc)override;


	ModelRender m_secondGroundRender;
	PhysicsStaticObject m_secondGroundPhysicsObj;
	Vector3 m_secondGroundPos = Vector3::Zero;
	Vector3 m_secondGroundScale = Vector3::One;
	Quaternion m_secondGroundRot = Quaternion::Identity;
};

