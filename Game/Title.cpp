#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "GameClear.h"
#include "GameOver.h"
#include "SoundManager.h"

namespace {
	const char* TITLE  = "Assets/Sprite/Title/Title_CHARGEADVENTURE.dds";
	
	const float W = 1920.0f;
	const float H = 1080.0f;
	const float FLASH_TIME = 0.5f;
}

bool Title::Start()
{
	m_titleRender.Init(TITLE, W, H);
	m_fontRender.SetText(L"PLESS A");
	m_fontRender.SetPosition(m_pos);
	m_fontRender.SetColor(g_vec4Yellow);
	m_fontRender.SetScale(2.2f);
	return true;
}

void Title::Update()
{
	float time = g_gameTime->GetFrameDeltaTime();
	switch (m_state) {
		/** フェードイン状態 */
	case enFadeIn:
		m_currentAlpha -= FLASH_TIME * time;
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enFadeOut;
		}
		break;
		/** フェードアウト状態 */
	case enFadeOut:
		m_currentAlpha += FLASH_TIME * time;
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enFadeIn;
		}
		break;
	}


	if (g_pad[0]->IsTrigger(enButtonA))
	{
		/** 音の追加 */
		SoundManager* sound = FindGO<SoundManager>("soundManager");
		m_clickSe = sound->PlayingSound(Sound::enSound_ClickSe, false, 1.8f);

		m_game = nullptr;
		m_game = NewGO<Game>(0, "game");
		this->Deactivate();

		DeleteGO(m_game->m_gameBGM);
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleRender.Draw(rc);
	/** フェードイン/アウトの点滅色 */
	if (m_currentAlpha > 0.0f) {
		m_fontRender.SetColor({ 1.0f,1.0f,m_currentAlpha,m_currentAlpha });
		m_fontRender.Draw(rc);
	}
	
}