#include "stdafx.h"
#include "BossStage.h"

namespace {
	const char* BOSS_STAGE = "Assets/modelData/BossStage.tkm";
}

bool BossStage::Start()
{
	m_bossStageRender.Init(BOSS_STAGE);
	m_bossStageRender.SetPosition(m_pos);
	m_bossStageRender.SetScale(m_sc);

	m_bossStageRender.Update();

	m_physicsObj.CreateFromModel(
		m_bossStageRender.GetModel(),
		m_bossStageRender.GetModel().GetWorldMatrix()
	);
	return true;
}

void BossStage::Render(RenderContext& rc)
{
	m_bossStageRender.Draw(rc);
}