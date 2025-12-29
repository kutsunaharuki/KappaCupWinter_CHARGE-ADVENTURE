#pragma once
class CrossRoad : public IGameObject
{
public:
	CrossRoad(){}
	~CrossRoad(){}


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


	PhysicsStaticObject m_physicsStaticObj;
	ModelRender m_crossRoadRender;
	Vector3 m_crossRoadPos = Vector3::Zero;
	Vector3 m_crossRoadScale = Vector3::One;
	Quaternion m_crossRoadRot = Quaternion::Identity;
};

