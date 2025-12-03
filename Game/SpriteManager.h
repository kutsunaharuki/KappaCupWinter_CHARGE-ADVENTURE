#pragma once
#include "Text.h"
class SpriteManager : public IGameObject
{
private:
	SpriteRender m_spriteRender;
	SpriteType m_spriteType;


public:
	void SetSpriteType(SpriteType spriteType) {
		m_spriteType = spriteType;
	}


public:
	SpriteManager();
	~SpriteManager();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


private:
	void SetUpSprite();
};

