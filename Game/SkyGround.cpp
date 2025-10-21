#include "stdafx.h"
#include "SkyGround.h"
#include "Player.h"

namespace {
	const char* SKYGROUND_INIT = "Assets/modelData/SkyG.tkm";
}

bool SkyGround::Start()
{
	m_skyGroundRender.Init(SKYGROUND_INIT);
	m_skyGroundRender.SetPosition(m_pos);
	m_skyGroundRender.SetScale(m_scale);
	m_skyGroundRender.Update();

	//ìñÇΩÇËîªíËÇÃí«â¡ÅB
	m_physicsStaticObj.CreateFromModel(m_skyGroundRender.GetModel(),
		m_skyGroundRender.GetModel().GetWorldMatrix());

	return true;
}

void SkyGround::Render(RenderContext& rc)
{
	m_skyGroundRender.Draw(rc);
}