#include "stdafx.h"
#include "CrossRoad.h"

namespace {
    const char* CROSS_ROAD_FILE_PATH = "Assets/modelData/CrossRoad.tkm";
}


bool CrossRoad::Start()
{
    m_crossRoadRender.Init(CROSS_ROAD_FILE_PATH);
    m_crossRoadRender.SetTRS(
        m_crossRoadPos,
        m_crossRoadRot,
        m_crossRoadScale
    );
    m_crossRoadRender.Update();

    m_physicsStaticObj.CreateFromModel(
        m_crossRoadRender.GetModel(),
        m_crossRoadRender.GetModel().GetWorldMatrix()
    );
    return true;
}

void CrossRoad::Update()
{
}

void CrossRoad::Render(RenderContext& rc)
{
    m_crossRoadRender.Draw(rc);
}
