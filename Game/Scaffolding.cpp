#include "stdafx.h"
#include "Scaffolding.h"

namespace{
	const char* SCAFFOLDING = "Assets/modelData/Koutei.tkm";
}

bool Scaffolding::Start()
{
	m_scaffoldingRender.Init(SCAFFOLDING);
	m_scaffoldingRender.SetPosition(m_scaffoldingPos);
	m_scaffoldingRender.SetRotation(m_scaffoldingRotation);
	m_scaffoldingRender.SetScale(m_scaffoldingSc);
	m_scaffoldingRender.Update();

	m_physicsStaticObj.CreateFromModel(
		m_scaffoldingRender.GetModel(),
		m_scaffoldingRender.GetModel().GetWorldMatrix()
	);

	return true;
}

void Scaffolding::Render(RenderContext& rc)
{
	m_scaffoldingRender.Draw(rc);
}
