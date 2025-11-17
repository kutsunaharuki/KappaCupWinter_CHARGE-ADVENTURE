#pragma once
class Player;
class HPUI : public IGameObject
{
public:
	//コンストラクタ=>基底クラス->派生クラスの順番で呼ばれる。自動的に呼ばれる関数。
	//デストラクタ=>派生クラス->基底クラスの順番で呼ばれる。自動的に呼ばれる関数。
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_hpUIRender;
	Player* m_player = nullptr;
};

