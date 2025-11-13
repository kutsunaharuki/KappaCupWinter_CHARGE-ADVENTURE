#include "stdafx.h"
#include "Title.h"
#include "Game.h"

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
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleRender.Draw(rc);
	m_fontRender.Draw(rc);
}