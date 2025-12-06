#pragma once
class Player;
class ChargeItem : public IGameObject
{
public:
	ChargeItem();
	~ChargeItem();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void GetItem();

	bool m_isDead = false;

	ModelRender m_chargeItemModelRender;
	Vector3 m_itemPos = Vector3::Zero;
	Vector3 m_itemScale = Vector3::One;
	Quaternion m_itemRot = Quaternion::Identity;


private:
	Player* m_player = nullptr;
};

