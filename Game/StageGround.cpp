#include "stdafx.h"
#include "StageGround.h"

namespace{
	const char* STAGE_GROUND = "Assets/modelData/StageGround.tkm";
}

bool StageGround::Start()
{
	m_stageGroundRender.Init(STAGE_GROUND);
	m_stageGroundRender.SetPosition(m_stageGroundPos);
	m_stageGroundRender.SetScale(m_stageGroundSc);
	m_stageGroundRender.Update();

	m_physicsObj.CreateFromModel(
		m_stageGroundRender.GetModel(),
		m_stageGroundRender.GetModel().GetWorldMatrix()
	);

	return true;
}

void StageGround::Render(RenderContext& rc)
{
	m_stageGroundRender.Draw(rc);
}