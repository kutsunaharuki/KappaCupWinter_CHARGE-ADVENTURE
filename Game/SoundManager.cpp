#include "stdafx.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace {
	const char* SOUND_FEAD = "Assets/Sound/";
	const char* SOUND_END = ".wav";
	const char* SOUND_FILE_NAMELIST[enSound_Num] = {
		"GameBGM",
		"BossBGM",
		"GameClear",
		"GameOver",
		"HplossSe",
		"EnemyGekihaSe",
		"EnemyAttackSe",
		"ClickSe"
	};
}


SoundManager::SoundManager()
{
	for (int i = 0; i < enSound_Num; i++)
	{
		std::string sound = std::string(SOUND_FEAD) + SOUND_FILE_NAMELIST[i] + (SOUND_END);
		const char* filePath = sound.c_str();
		g_soundEngine->ResistWaveFileBank(i, filePath);
	}
}

SoundSource* SoundManager::PlayingSound(Sound number, bool isLoop, float valume)
{
	SoundSource* sound = NewGO<SoundSource>(0, "soundSource");
	sound->Init(number);
	sound->SetVolume(valume);
	sound->Play(isLoop);

	return sound;
}