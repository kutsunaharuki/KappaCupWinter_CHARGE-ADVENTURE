#include "stdafx.h"
#include "HPUI.h"
#include "Player.h"

namespace {
	const char* HP_HEART  = "Assets/Sprite/HP.dds";
	const char* HP_BREAK  = "Assets/Sprite/HPBreak.dds";
	const char* HP_PARLIN = "Assets/Sprite/HPParlin.dds";
}

bool HPUI::Start()
{
	m_hpUIRender.Init(HP_HEART,396.0f,241.0f);
	
	return true;
}

void HPUI::Update()
{
	int hp = m_player->GetHp();
	int maxHp = m_player->GetMaxHp();

	
}

void Render(RenderContext& rc)
{
	//m_hpUIRender.Draw(rc);
}