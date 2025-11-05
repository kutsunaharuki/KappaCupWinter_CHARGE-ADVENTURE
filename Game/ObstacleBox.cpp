#include "stdafx.h"
#include "Player.h"
#include "ObstacleBox.h"

namespace {
	const char* MODEL_BOX = "Assets/modelData/Box.tkm";
}

bool ObstacleBox::Start()
{
	m_boxRender.Init(MODEL_BOX);
	m_boxRender.SetTRS(m_boxPos, m_brot, m_boxSc);
	m_boxRender.Update();

	//静的オブジェクトの設定。
	//レベルを使う際は当たり判定の情報は除くこと。
	m_physicsStaticObjBox.CreateFromModel(m_boxRender.GetModel(),
		m_boxRender.GetModel().GetWorldMatrix());
	m_player = FindGO<Player>("player");
	return true;
}

void ObstacleBox::Update()
{
	SetCollisionObj();
	m_collisionObj->SetPosition(m_boxPos);
	m_collisionObj->SetRotation(m_brot);
	m_collisionObj->Update();
	m_boxRender.SetTRS(m_boxPos, m_brot, m_boxSc);
	m_boxRender.Update();
}

//コリジョンオブジェクト設定用関数。
void ObstacleBox::SetCollisionObj()
{
	m_collisionObj = new CollisionObject;

	//コリジョンオブジェクトの設定。
	m_collisionObj->CreateBox(
		m_boxPos,
		m_brot,
		m_boxSc
	);
	m_collisionObj->SetPosition(m_boxPos);
	m_collisionObj->SetRotation(m_brot);
	m_collisionObj->Update();
}

void ObstacleBox::Render(RenderContext& rc)
{
	//障害物(ボックス)の描画。
	m_boxRender.Draw(rc);
}