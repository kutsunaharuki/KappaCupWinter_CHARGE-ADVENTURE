#pragma once
class BossStage : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc);
	
	
	ModelRender m_bossStageRender;
	PhysicsStaticObject m_physicsObj;
	Vector3 m_pos = Vector3::Zero;//座標。
	Vector3 m_sc = { 1.0f,1.0f,1.0f };//スケール。
};

