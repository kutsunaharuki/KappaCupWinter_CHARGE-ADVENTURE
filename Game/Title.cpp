#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "GameClear.h"
#include "GameOver.h"

namespace {
	const char* TITLE = "Assets/Sprite/Title.dds";
	const float WIDE = 1920.0f;
	const float HEIGHT = 1080.0f;
}

bool Title::Start()
{
	m_titleRender.Init(TITLE, WIDE, HEIGHT);
	m_fontRender.SetText(L"PLESS A");
	m_fontRender.SetPosition(m_pos);
	m_fontRender.SetColor(g_vec4Yellow);
	m_fontRender.SetScale(2.2f);

	m_gameClear = NewGO<GameClear>(0, "gameClear");
	m_gameClear->Deactivate();
	
	m_gameOver = NewGO<GameOver>(0, "gameOver");
	m_gameOver->Deactivate();
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_game = nullptr;
		m_game = NewGO<Game>(0, "game");
	
		this->Deactivate();
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleRender.Draw(rc);
	m_fontRender.Draw(rc);
}