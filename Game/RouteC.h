#pragma once
class RouteC : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc)override;


	ModelRender m_routeCRender;
	PhysicsStaticObject m_routeCPhysicsObj;
	Vector3 m_routeCPos = Vector3::Zero;
	Vector3 m_routeCScale = Vector3::One;
	Quaternion m_routeCRot = Quaternion::Identity;
};

