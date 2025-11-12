#include "stdafx.h"
#include "SkyGround.h"

namespace {
	const char* SKYGROUND = "Assets/modelData/SkyGround.tkm";
}

bool SkyGround::Start()
{
	m_skyGroundRender.Init(SKYGROUND);
	m_skyGroundRender.SetPosition(m_skyGroundPos);
	m_skyGroundRender.SetScale(m_skyGroundSc);
	m_skyGroundRender.Update();

	m_physicsObj.CreateFromModel(
		m_skyGroundRender.GetModel(),
		m_skyGroundRender.GetModel().GetWorldMatrix()
	);
	return true;
}

void SkyGround::Render(RenderContext& rc)
{
	m_skyGroundRender.Draw(rc);
}