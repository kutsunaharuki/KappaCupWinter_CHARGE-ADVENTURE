#pragma once
class Player;
class WarpBox : public IGameObject
{
public:
	WarpBox(){}
	~WarpBox();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetCollisionObj();

	//CollisionObject* GetCollisionObj()
	//{
	//	return m_warpCollisionObj;
	//}

	//ModelRender* GetModelRender()
	//{
	//	return &m_warpBoxModelRender;
	//}

	
public:
	/** ワープする */
	void Warp();

public:
	bool m_isWarpHit = false;

	Vector3 m_warpBoxPos = Vector3::Zero;
	Vector3 m_warpBoxScale = Vector3::One;
	Vector3 m_warpTargetPos = Vector3::Zero;
	Quaternion m_warpBoxRot = Quaternion::Identity;


private:
	ModelRender m_warpBoxModelRender;
	CollisionObject* m_warpCollisionObj = nullptr;	
	

private:
	Player* m_player = nullptr;
};

