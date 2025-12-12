#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"

namespace {
	const char* GAME_CLEAR = "Assets/Sprite/GameClear/Clear.dds";

	const float BLINK = 0.52f;
}

bool GameClear::Start()
{
	m_gameClearRender.Init(GAME_CLEAR, 1920.0f, 1080.0f);
	m_fontRender.SetText(L"PLESS Y");
	m_fontRender.SetColor(g_vec4Yellow);
	m_fontRender.SetPosition(m_pos);
	m_fontRender.SetScale(2.2f);
	return true;
}

void GameClear::Update()
{
	float ti = g_gameTime->GetFrameDeltaTime();
	switch (m_fadeSt) {
		/** フェードイン状態 */
	case enClear_FadeIn:
		m_currentAlpha -= BLINK * ti;
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_fadeSt = enClear_FadeIn;
		}
		break;
		/** フェードアウト状態 */
	case enClear_FadeOut:
		m_currentAlpha += BLINK * ti;
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_fadeSt = enClear_FadeOut;
		}
		break;
	}
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		auto title = FindGO<Title>("title");
		title->Activate();
		this->Deactivate();
		//NewGO<Title>(0, "title");
		//DeleteGO(this);
	}
}

void GameClear::Render(RenderContext& rc)
{
	m_gameClearRender.Draw(rc);
	if (m_currentAlpha > 0.0f) {
		m_fontRender.SetColor(1.0f, 1.0f, m_currentAlpha, m_currentAlpha);
		m_fontRender.Draw(rc);
	}
}