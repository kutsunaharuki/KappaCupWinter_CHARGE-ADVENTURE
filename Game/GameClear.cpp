#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"

namespace {
	const char* GAME_CLEAR = "Assets/Sprite/Clear.dds";
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
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameClear::Render(RenderContext& rc)
{
	m_gameClearRender.Draw(rc);
	m_fontRender.Draw(rc);
}