#pragma once
#include "stdint.h"
class Player;
class GameOver;

class HPUI : public IGameObject
{
public:
	enum class HpState : uint8_t {
		enFull_Hp,
		enBreak_Hp,
		enAllBreak_Hp
	};
	//static_cast(キャスト処理。)
	HpState m_hpState;
	
	//コンストラクタ=>基底クラス->派生クラスの順番で呼ばれる。自動的に呼ばれる関数。
	//デストラクタ=>派生クラス->基底クラスの順番で呼ばれる。自動的に呼ばれる関数。
	bool Start()override;
	//void Update();
	void TakeDamage(int hp);
	void Render(RenderContext& rc);
	Vector3 m_hpPos = { -520.0f,280.0f,0.0f };
	Vector3 m_hpSc = Vector3::One * 2.0f;
private:
	SpriteRender m_hpUIRender;
	SpriteRender m_hpBreakRender;
	SpriteRender m_hpAllBreakRender;
	Player* m_player = nullptr;
	GameOver* m_gameOver = nullptr;
};

