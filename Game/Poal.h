#pragma once
class Player;
class BossStage;
class Enemy;
class Poal : public IGameObject
{
public:
	~Poal();
	bool Start()override;
	void Update();
	void SetCollisionObj();
	//bool PoalHit();

	CollisionObject* GetCollision() {
		return m_collisionObj;
	}

	//const bool SetLevelModel();
	void Render(RenderContext& rc);
	ModelRender m_poalRender;//モデル。
	PhysicsStaticObject m_physicsObj;
	CollisionObject* m_collisionObj = nullptr;//コリジョン。
	Quaternion m_rot = Quaternion::Identity;//回転。
	Vector3 m_pos = Vector3::Zero;//座標。
	Vector3 m_scale = { 1.5f,3.0f,1.5f };//スケール。
	Vector3 m_colPos = { 0.0f,100.0f,0.0f };
	//bool isHit = false;//ゴールポールに当たったらのフラグ。
	//LevelRender m_bossLevelRender;
	//bool isFlag = false;
	//std::vector<Enemy*> m_enemys;
	//std::vector<BossStage*> m_bossStages;

private:
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BossStage* m_bossStage = nullptr;
};

