#pragma once
class Player;
class Poal : public IGameObject
{
public:
	~Poal();
	bool Start()override;
	void SetCollisionObj();
	void PoalHit();
	void Render(RenderContext& rc);
	ModelRender m_poalRender;//モデル。
	PhysicsStaticObject m_physicsObj;
	CollisionObject* m_collisionObj = nullptr;//コリジョン。
	Quaternion m_rot = Quaternion::Identity;//回転。
	Vector3 m_pos = Vector3::Zero;//座標。
	Vector3 m_scale = { 1.5f,3.0f,1.5f };//スケール。
	Vector3 m_colPos = { 0.0f,100.0f,0.0f };
	bool isHit = false;//ゴールポールに当たったらのフラグ。
private:
	Player* m_player = nullptr;
};

