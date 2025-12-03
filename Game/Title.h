#pragma once
class Game;
class GameClear;
class GameOver;
class Title : public IGameObject
{
public:
	enum EnFadeState {
		enFadeIn,
		enFadeOut
	};

	bool Start()override;
	void Update();
	void Render(RenderContext& rc);


	/** todo::追加 */
	/** フェードイン状態 */
	void FadeIn()
	{
		m_state = enFadeIn;
	}
	/** フェードアウト状態 */
	void FadeOut()
	{
		m_state = enFadeOut;
	}
	float GetCurrentAlpha()const
	{
		return m_currentAlpha;
	}
	float m_currentAlpha = 1.0f;
	/** -------------------------------- */

	SoundSource* m_clickSe = nullptr;
private:
	/** 今の状態 */
	EnFadeState m_state = enFadeIn;
	/** α値 */
	SpriteRender m_titleRender;
	FontRender m_fontRender;
	Vector3 m_pos = Vector3::Zero;
	Game* m_game = nullptr;
	GameOver* m_gameOver = nullptr;
	GameClear* m_gameClear = nullptr;

};

