#include "stdafx.h"
#include "HPUI.h"
#include "Player.h"

namespace {
	const char* HP_HEART  = "Assets/Sprite/HP.dds";
	const char* HP_BREAK  = "Assets/Sprite/HPHeartBreak.dds";
	const char* HP_PARLIN = "Assets/Sprite/HPAllBreak.dds";
}

bool HPUI::Start()
{
	m_hpUIRender.Init(HP_HEART,396.0f,241.0f);
	m_hpUIRender.SetPosition(m_hpPos);
	m_hpUIRender.SetScale(m_hpSc);
	m_hpUIRender.Update();

	m_hpBreakRender.Init(HP_BREAK, 396.0f, 241.0f);
	m_hpBreakRender.SetPosition(m_hpPos);
	m_hpBreakRender.SetScale(m_hpSc);
	m_hpBreakRender.Update();

	m_hpAllBreakRender.Init(HP_PARLIN, 396.0f, 241.0f);
	m_hpAllBreakRender.SetPosition(m_hpPos);
	m_hpAllBreakRender.SetScale(m_hpSc);
	m_hpAllBreakRender.Update();
	m_player = FindGO<Player>("player");
	return true;
}

//void HPUI::RenderHPManager()
//{
//	//数値型から列挙型への変換。
//	//enum classは暗黙の型変換に対応していないので、
//	//明示的なキャスト処理をしなければならない。(static_cast<>())を使用。
//	// (static_cast<型を指定する>(enum型の～～を指定する)。	
//
//	//描画の切り替え。
//	switch (m_hpState)
//	{
//	case HpState::enFull_Hp:
//		m_hpUIRender.Init(HP_HEART, 396.0f, 241.0f);
//		break;
//	case HpState::enBreak_Hp:
//		m_hpBreakRender.Init(HP_BREAK, 396.0f, 241.0f);
//		break;
//	case HpState::enAllBreak_Hp:
//		m_hpAllBreakRender.Init(HP_PARLIN, 396.0f, 241.0f);
//		break;
//	}
//}

void HPUI::TakeDamage(HPUI::HpState state)
{
	m_hpState = state;
}

void HPUI::Update()
{
	//RenderHPManager();
}

void HPUI::Render(RenderContext& rc)
{
	switch (m_hpState)
	{
	case HpState::enFull_Hp:
		m_hpUIRender.Draw(rc);
		break;
	case HpState::enBreak_Hp:
		m_hpBreakRender.Draw(rc);
		break;
	case HpState::enAllBreak_Hp:
		m_hpAllBreakRender.Draw(rc);
		break;
	}
	//m_hpUIRender.Draw(rc);
}