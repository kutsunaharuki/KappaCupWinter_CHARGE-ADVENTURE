#pragma once
class BossStage : public IGameObject
{
public:
	bool Start()override;
	void Render(RenderContext& rc);
	ModelRender m_bossStageRender;
	PhysicsStaticObject m_physicsObj;
	Vector3 m_pos = Vector3::Zero;//ç¿ïWÅB
private:

};

