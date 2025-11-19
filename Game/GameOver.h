#pragma once
class GameOver : public IGameObject
{
public:
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);
	SpriteRender m_gameOverRender;
private:
	FontRender m_fontRender;
	Vector3 m_pos = Vector3::Zero;
};

