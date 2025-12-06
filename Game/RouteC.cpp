#include "stdafx.h"
#include "RouteC.h"

namespace {
	const char* ROUTE_C_FILE_PATH = "Assets/modelData/RouteC.tkm";
}


bool RouteC::Start()
{
	m_routeCRender.Init(ROUTE_C_FILE_PATH);
	m_routeCRender.SetPosition(m_routeCPos);
	m_routeCRender.SetScale(m_routeCScale);
	m_routeCRender.SetRotation(m_routeCRot);
	m_routeCRender.Update();

	m_routeCPhysicsObj.CreateFromModel(
		m_routeCRender.GetModel(),
		m_routeCRender.GetModel().GetWorldMatrix()
	);

	return true;
}


void RouteC::Render(RenderContext& rc)
{
	m_routeCRender.Draw(rc);
}
