#pragma once
class GameOver : public IGameObject
{
public:
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_gameOverRender;
	FontRender m_fontRender;
	Vector3 m_pos = Vector3::Zero;
};

