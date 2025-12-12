#pragma once
class WarpBox : public IGameObject
{
public:
	WarpBox(){}
	~WarpBox();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetCollisionObj();

	CollisionObject* GetCollisionObj()
	{
		return m_warpCollisionObj;
	}

	ModelRender* GetModelRender()
	{
		return &m_warpBoxModelRender;
	}


public:
	/** ワープする */
	void Warp();
	/** ワープ先 */
	void WarpDestination();

public:
	bool m_isWarpHit = false;


private:
	ModelRender m_warpBoxModelRender;
	CollisionObject* m_warpCollisionObj = nullptr;
	Vector3 m_warpBoxPos = Vector3::Zero;
	Vector3 m_warpBoxScale = Vector3::One;
	Quaternion m_warpBoxRot = Quaternion::Identity;
};

