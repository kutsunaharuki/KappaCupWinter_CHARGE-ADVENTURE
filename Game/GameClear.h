#pragma once
class GameClear : public IGameObject
{
public:
	enum EnClearFade {
		enClear_FadeIn,
		enClear_FadeOut
	};
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);

	void ClearFadeIn()
	{
		m_fadeSt = enClear_FadeIn;
	}
	void ClearFadeOut()
	{
		m_fadeSt = enClear_FadeOut;
	}
	float GetCurrentAlpha()const
	{
		return m_currentAlpha;
	}
	float m_currentAlpha = 1.0f;
	/** ----------------------------- */

private:
	EnClearFade m_fadeSt = enClear_FadeIn;
	SpriteRender m_gameClearRender;
	FontRender m_fontRender;
	Vector3 m_pos = { 0.0f,-300.0f,0.0f };
};

