#pragma once
enum StandbyScreen {
	enPause_CurrentAlpha,
	enPause,
	enResume,
	enQuite,
	enSprite_Num
};

class PauseScene : public IGameObject
{
public:
	PauseScene(){}
	~PauseScene(){}


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc);


private:
	SpriteRender m_pauseSpriteRender[enSprite_Num];
};

