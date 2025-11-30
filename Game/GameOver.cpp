#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"

namespace {
	const char* GAME_OVER = "Assets/Sprite/GameOver.dds";

	const float FLASH = 0.52f;
}

bool GameOver::Start()
{
	m_gameOverRender.Init(GAME_OVER, 1920.0f, 1080.0f);
	m_fontRender.SetText(L"PLESS Y");
	m_fontRender.SetColor(g_vec4Yellow);
	m_fontRender.SetPosition(m_pos);
	m_fontRender.SetScale(2.2f);
	return true;
}

void GameOver::Update()
{
	float t = g_gameTime->GetFrameDeltaTime();
	switch (m_fadeState)
	{
	case enFadeIn:
		m_currentAlpha -= FLASH * t;
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_fadeState = enFadeOut;
		}
		break;
	case enFadeOut:
		m_currentAlpha += FLASH * t;
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_fadeState = enFadeIn;
		}
		break;
	}

	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//NewGO<Title>(0, "title");
		auto title = FindGO<Title>("title");
		title->Activate();
		this->Deactivate();
		//DeleteGO(this);
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_gameOverRender.Draw(rc);
	if (m_currentAlpha > 0.0f) {
		m_fontRender.SetColor(1.0f, 1.0f, m_currentAlpha, m_currentAlpha);
		m_fontRender.Draw(rc);
	}
}