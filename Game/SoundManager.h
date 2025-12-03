#pragma once
#include "sound/SoundEngine.h"

enum Sound {
	enSound_GameBGM,
	enSound_BossBGM,
	enSound_GameClearSe,
	enSound_GameOverSe,
	enSound_HPlossSe,
	enSound_GekihaSe,
	enSound_EnemyAttackSe,
	enSound_ClickSe,
	enSound_Num
};

class SoundManager : public IGameObject
{
public:
	SoundManager();
	~SoundManager() {};
	SoundSource* PlayingSound(Sound number, bool isLoop = true, float valume = 1.0f);
};

