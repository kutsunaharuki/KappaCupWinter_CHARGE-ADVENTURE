#pragma once
class Game;
class GameClear;
class GameOver;
class Title : public IGameObject
{
public:
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);


private:
	SpriteRender m_titleRender;
	FontRender m_fontRender;
	Vector3 m_pos = Vector3::Zero;
	Game* m_game = nullptr;
	GameOver* m_gameOver = nullptr;
	GameClear* m_gameClear = nullptr;
};

