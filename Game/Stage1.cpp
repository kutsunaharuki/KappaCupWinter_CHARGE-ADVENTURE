#include "stdafx.h"
#include "Stage1.h"

namespace {
	const char* STAGE1_INIT = "Assets/modelData/Stage1.tkm";
}

bool Stage1::Start()
{
	m_stage1Render.Init(STAGE1_INIT);
	m_stage1Render.SetPosition(m_Pos);
	m_stage1Render.SetScale(m_scale);
	m_stage1Render.Update();

	m_physicsStaticObj.CreateFromModel(m_stage1Render.GetModel(),
		m_stage1Render.GetModel().GetWorldMatrix());
	return true;
}