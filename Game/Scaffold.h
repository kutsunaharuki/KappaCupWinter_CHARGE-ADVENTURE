#pragma once
class Scaffold : public IGameObject
{
public:
	Scaffold();
	~Scaffold();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


	ModelRender m_scaffoldRender;
	PhysicsStaticObject m_scaffoldPhysicsObj;
	Vector3 m_scaffoldPos = Vector3::Zero;
	Vector3 m_scaffoldScale = Vector3::One;
	Quaternion m_scaffoldRot = Quaternion::Identity;
};

