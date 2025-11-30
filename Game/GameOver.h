#pragma once
class GameOver : public IGameObject
{
public:
	/** フェードイン/アウト */
	enum EnFade {
		enFadeIn,
		enFadeOut
	};
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);
	
	/** todo::フェードイン/アウト */
	void FadeIn()
	{
		m_fadeState = enFadeIn;
	}
	void FadeOut()
	{
		m_fadeState = enFadeOut;
	}
	float GetCurrentAlpha()const
	{
		return m_currentAlpha;
	}
	float m_currentAlpha = 1.0f;
	/** -------------------------------- */


private:
	EnFade m_fadeState = enFadeIn;
	SpriteRender m_gameOverRender;
	FontRender m_fontRender;
	Vector3 m_pos = { 0.0f,-300.0f,0.0f };
};

