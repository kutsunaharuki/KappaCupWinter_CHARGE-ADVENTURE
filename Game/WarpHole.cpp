#include "stdafx.h"
#include "WarpHole.h"
#include "Warp.h"

//モデルの情報。
namespace {
	const char* WARPBOX_HOLE_INIT = "Assets/modelData/WarpBoxHole.tkm";
}

bool WarpHole::Start()
{
	m_warpHoleRender.Init(WARPBOX_HOLE_INIT);
	m_warpHoleRender.SetPosition(m_position);
	m_warpHoleRender.SetScale(m_scale);

	m_warpHoleRender.Update();
	
	m_warp = FindGO<Warp>("warp");
	return true;
}

void WarpHole::Update()
{
	
}

void WarpHole::Render(RenderContext& rc)
{
	//ワープボックスの描画。
	m_warpHoleRender.Draw(rc);
}