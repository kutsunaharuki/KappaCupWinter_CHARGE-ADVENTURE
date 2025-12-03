#include "stdafx.h"
#include "SpriteManager.h"
#include "Text.h"

namespace{
    const std::string FILE_NAME[enSpriteType_Num] = {
        "Title/Title_CHARGEADVENTURE",
        "GameOver/GameOver",
        "GameClear/GameClear"
    };
}

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

bool SpriteManager::Start()
{
    SetUpSprite();
    return true;
}

void SpriteManager::Update()
{
}

void SpriteManager::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
}

void SpriteManager::SetUpSprite()
{
    m_spriteRender.Init(nsSprite::GetFullPath(FILE_NAME[m_spriteType]).c_str(),1920.0f,1080.0f);
}
