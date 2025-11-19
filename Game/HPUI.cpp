#include "stdafx.h"
#include "HPUI.h"
#include "Player.h"
#include "GameOver.h"

namespace {
	const char* HP_HEART  = "Assets/Sprite/HP.dds";
	const char* HP_BREAK  = "Assets/Sprite/HPHeartBreak.dds";
	const char* HP_PARLIN = "Assets/Sprite/HeartAllBreak.dds";
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
	m_gameOver = FindGO<GameOver>("gameOver");
	return true;
}

//数値型から列挙型への変換。
//enum classは暗黙の型変換に対応していないので、
//明示的なキャスト処理をしなければならない。(static_cast<>())を使用。
// (static_cast<型を指定する>(enum型の～～を指定する)。	

//ダメージ後のHPの値を外部から渡すための関数。
void HPUI::TakeDamage(int hp)
{
	if (hp == 3)
	{
		m_hpState = HpState::enFull_Hp;
	}
	else if (hp == 2)
	{
		m_hpState = HpState::enBreak_Hp;
	}
	else if (hp == 1)
	{
		m_hpState = HpState::enAllBreak_Hp;
	}
}


/// <summary>
/// HPUIの描画処理。
/// </summary>
/// <param name="rc"></param>
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