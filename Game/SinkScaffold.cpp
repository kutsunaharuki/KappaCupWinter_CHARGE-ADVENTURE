#include "stdafx.h"
#include "SinkScaffold.h"
#include "Player.h"

namespace {
	const char* SINK_SCAFFOLD_FILE_PATH = "Assets/modelData/SinkScaffold.tkm";
	const Vector3 COLLISION_SIZE = Vector3::One * 100.0f;
}


bool SinkScaffold::Start()
{
	m_sinkScaffoldRender.Init(SINK_SCAFFOLD_FILE_PATH);
	m_sinkScaffoldRender.SetPosition(m_sinkScaffoldPos);
	m_sinkScaffoldRender.SetScale(m_sinkScaffoldScale);
	m_sinkScaffoldRender.SetRotation(m_sinkScaffoldRot);
	m_sinkScaffoldRender.Update();

	m_sinkScaffoldPhysicsObj.CreateFromModel(
		m_sinkScaffoldRender.GetModel(),
		m_sinkScaffoldRender.GetModel().GetWorldMatrix()
	);
	return true;
}


void SinkScaffold::Update()
{
	SetCollision();
	m_sinkCollision->SetPosition(m_sinkScaffoldPos);
	m_sinkCollision->SetRotation(m_sinkScaffoldRot);
}


void SinkScaffold::SetCollision()
{
	m_sinkCollision = new CollisionObject;
	m_sinkCollision->CreateBox(
		m_sinkScaffoldPos,
		m_sinkScaffoldRot,
		COLLISION_SIZE
	);
	m_sinkCollision->SetPosition(m_sinkScaffoldPos);
	m_sinkCollision->SetRotation(m_sinkScaffoldRot);
}


void SinkScaffold::Render(RenderContext& rc)
{
	m_sinkScaffoldRender.Draw(rc);
}
