#pragma once
class GameClear : public IGameObject
{
public:
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_gameClearRender;
	FontRender m_fontRender;
	Vector3 m_pos = Vector3::Zero;
};

