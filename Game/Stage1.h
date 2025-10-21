#pragma once
class Stage1:public IGameObject
{
public:
	Stage1() {};
	~Stage1() {};
	bool Start()override;

	ModelRender m_stage1Render;
	PhysicsStaticObject m_physicsStaticObj;
	Vector3 m_Pos = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
};

