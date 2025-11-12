#pragma once
class Asiba : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc);

	ModelRender m_asibaRender;
	PhysicsStaticObject m_physicsObj;
	Vector3 m_pos = Vector3::Zero;
	Vector3 m_sc = Vector3::One;
};

