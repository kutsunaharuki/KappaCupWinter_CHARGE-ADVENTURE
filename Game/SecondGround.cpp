#include "stdafx.h"
#include "SecondGround.h"

namespace {
	const char* SECOND_GROUND_FILE_PATH = "Assets/modelData/SecondGround.tkm";
}


bool SecondGround::Start()
{
	m_secondGroundRender.Init(SECOND_GROUND_FILE_PATH);
	m_secondGroundRender.SetPosition(m_secondGroundPos);
	m_secondGroundRender.SetScale(m_secondGroundScale);
	m_secondGroundRender.SetRotation(m_secondGroundRot);
	m_secondGroundRender.Update();

	m_secondGroundPhysicsObj.CreateFromModel(
		m_secondGroundRender.GetModel(),
		m_secondGroundRender.GetModel().GetWorldMatrix()
	);

	return true;
}


void SecondGround::Render(RenderContext& rc)
{
	m_secondGroundRender.Draw(rc);
}
