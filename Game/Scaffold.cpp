#include "stdafx.h"
#include "Scaffold.h"

namespace {
	const char* SCAFFOLD_FILE_PATH = "Assets/modelData/Scaffolding.tkm";
}

Scaffold::Scaffold()
{
}

Scaffold::~Scaffold()
{
}

bool Scaffold::Start()
{
	m_scaffoldRender.Init(SCAFFOLD_FILE_PATH);
	m_scaffoldRender.SetTRS(
		m_scaffoldPos,
		m_scaffoldRot,
		m_scaffoldScale
	);
	m_scaffoldRender.Update();

	m_scaffoldPhysicsObj.CreateFromModel(
		m_scaffoldRender.GetModel(),
		m_scaffoldRender.GetModel().GetWorldMatrix()
	);
	return true;
}

void Scaffold::Update()
{
}

void Scaffold::Render(RenderContext& rc)
{
	m_scaffoldRender.Draw(rc);
}
