#pragma once

//チャージスプライト。
enum ChargeStage {
	enCharge_StageZero = 0,
	enCharge_StageOne,
	enCharge_StageTwo,
	enCharge_StageThree,
	enCharge_StageFour,
	enCharge_Max,
	enCharge_Num
};

class ChargeBar : public IGameObject
{
public:
	ChargeBar(){}
	~ChargeBar(){}


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

public:
	void AddChargeAmount();
	void SmallDecreaseChargeAmount();
	void BigDecreaseChargeAmounrt();

	/** ゲッター */
	ChargeStage GetChargeSprites()const { return chargeSprites; }
	
	/** セッター */
	void SetChargeAmount(int amount) {
		if (amount < 0)amount = 0;
		if (amount >= enCharge_Num)
			amount = enCharge_Num - 1;
		chargeSprites = static_cast<ChargeStage>(amount);
		UpdateChargeSprite();
	}


private:
	void DecreaseChargeAmount(int value);

private:
	void HideAllSprites();//全てのスプライトを非表示。
	void ShowCurrentSprite();//現段階のみを表示。
	void ApplyChargeState();//チャージ段階に応じての演出。

	void UpdateChargeSprite();


private:
	Vector4 m_hidden = { 1.0f,1.0f,1.0f,0.0f };
	Vector4 m_maxFlashColor = { 1.0f,0.9f,0.4f,1.0f };
	Vector4 m_show = { 1.0f,1.0f,1.0f,1.0f };
	
	
	bool m_isInited = false;


private:
	SpriteRender m_chargeBarSpriteRender[enCharge_Num];
	ChargeStage chargeSprites = ChargeStage::enCharge_StageZero;
};

