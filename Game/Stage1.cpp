#include "stdafx.h"
#include "Stage1.h"

namespace {
	const char* STAGE1_FILE_PATH = "Assets/modelData/Stage1-1StaticObject.tkm";
}

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
}

bool Stage1::Start()
{
	m_stage1Render.Init(STAGE1_FILE_PATH);
	m_stage1Render.SetPosition(m_stage1Pos);
	m_stage1Render.SetRotation(m_stage1Rot);
	m_stage1Render.SetScale(m_stage1Scale);
	m_stage1Render.Update();
	
	m_physicsStaticObj.CreateFromModel(
		m_stage1Render.GetModel(),
		m_stage1Render.GetModel().GetWorldMatrix()
	);
	return true;
}

void Stage1::Update()
{
}

void Stage1::Render(RenderContext& rc)
{
	m_stage1Render.Draw(rc);
}
