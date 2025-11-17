#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"

namespace {
	const char* GAME_OVER = "Assets/Sprite/GameOver.dds";
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
	m_fontRender.Draw(rc);
}