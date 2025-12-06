#pragma once
class Stage1 : public IGameObject
{
public:
	Stage1();
	~Stage1();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


	ModelRender m_stage1Render;
	PhysicsStaticObject m_physicsStaticObj;
	Vector3 m_stage1Pos = Vector3::Zero;
	Vector3 m_stage1Scale = Vector3::One;
	Quaternion m_stage1Rot = Quaternion::Identity;
};

