#include "stdafx.h"
#include "Asiba.h"

namespace {
	const char* ASIBA = "Assets/modelData/Asiba.tkm";
}

bool Asiba::Start()
{
	m_asibaRender.Init(ASIBA);
	m_asibaRender.SetPosition(m_pos);
	m_asibaRender.SetScale(m_sc);
	m_asibaRender.Update();

	m_physicsObj.CreateFromModel(
		m_asibaRender.GetModel(),
		m_asibaRender.GetModel().GetWorldMatrix()
	);

	return true;
}

void Asiba::Render(RenderContext& rc)
{
	m_asibaRender.Draw(rc);
}