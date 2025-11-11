#pragma once
class Scaffolding : public IGameObject
{
public:
	Scaffolding() {};
	~Scaffolding() {};
	bool Start()override;
	void Render(RenderContext& rc);

	ModelRender m_scaffoldingRender;
	PhysicsStaticObject m_physicsStaticObj;
	Vector3 m_scaffoldingPos = Vector3::Zero;
	Vector3 m_scaffoldingSc = Vector3::One;
	Quaternion m_scaffoldingRotation = Quaternion::Identity;
};

