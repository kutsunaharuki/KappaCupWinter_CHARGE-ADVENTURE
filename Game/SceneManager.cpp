#include "stdafx.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"

SceneManager* SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	m_sceneType = SceneType::enTitle;
	TitleSceneStart();
	return true;
}


void SceneManager::Update()
{
	SceneStateMachine();
}


void SceneManager::SceneStateMachine()
{
	switch (m_sceneType) {
	case SceneType::enTitle:
		TitleSceneUpdate();
		TitleSceneChange();
		break;
	case SceneType::enGame:
		GameSceneUpdate();
		GameSceneChange();
		break;
	case SceneType::enGameOver:
		GameOverSceneUpdate();
		GameOverSceneChange();
		break;
	case SceneType::enGameClear:
		GameClearSceneUpdate();
		GameClearSceneChange();
		break;
	case SceneType::enPause:
		PauseSceneUpdate();
		PauseSceneChange();
		break;
	}
}

void SceneManager::TitleSceneStart()
{
	m_spriteManager = NewGO<SpriteManager>(0, "spriteManager");
	m_spriteManager->SetSpriteType(SpriteType::enSpriteType_Title);
}

void SceneManager::TitleSceneUpdate()
{

}

void SceneManager::TitleSceneEnd()
{
	DeleteGO(m_spriteManager);
	m_spriteManager = nullptr;
}

void SceneManager::TitleSceneChange()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		TitleSceneEnd();
		m_sceneType = SceneType::enGame;
		GameSceneStart();
	}
}

void SceneManager::GameSceneStart()
{
	m_game = NewGO<Game>(0, "game");
	m_firstStage = NewGO<FirstStage>(0, "firstStage");
}

void SceneManager::GameSceneUpdate()
{
	
}

void SceneManager::GameSceneEnd()
{
	m_game = FindGO<Game>("game");
	m_firstStage = FindGO<FirstStage>("firstStage");
	DeleteGO(m_game);
	m_game = nullptr;
	DeleteGO(m_firstStage);
	m_firstStage = nullptr;
}

void SceneManager::GameSceneChange()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");
	if (!m_player)
	{
		return;
	}
	if (m_player->GetHp() <= 0) {
		GameSceneEnd();
		m_sceneType = SceneType::enGameOver;
		GameOverSceneStart();
	}
	if (m_player->m_position.y <= -200.0f) {
		GameSceneEnd();
		m_sceneType = SceneType::enGameOver;
		GameOverSceneStart();
	}
	if (!m_game) {
		return;
	}
	if (m_game->IsTimeUp())
	{
		GameSceneEnd();
		m_sceneType = SceneType::enGameOver;
		GameOverSceneStart();
	}
}

void SceneManager::GameOverSceneStart()
{
	m_spriteManager = NewGO<SpriteManager>(0, "spriteManager");
	m_spriteManager->SetSpriteType(SpriteType::enSpriteType_GameOver);
}

void SceneManager::GameOverSceneUpdate()
{
}

void SceneManager::GameOverSceneEnd()
{
	DeleteGO(m_spriteManager);
	m_spriteManager = nullptr;
}

void SceneManager::GameOverSceneChange()
{
	if (g_pad[0]->IsTrigger(enButtonX)) {
		GameOverSceneEnd();
		m_sceneType = SceneType::enTitle;
		TitleSceneStart();
	}
}

void SceneManager::GameClearSceneStart()
{
	m_spriteManager = NewGO<SpriteManager>(0, "spriteManager");
	m_spriteManager->SetSpriteType(SpriteType::enSpriteType_GameClear);
}

void SceneManager::GameClearSceneUpdate()
{
}

void SceneManager::GameClearSceneEnd()
{
	DeleteGO(m_spriteManager);
	m_spriteManager = nullptr;
}

void SceneManager::GameClearSceneChange()
{
	if (g_pad[0]->IsTrigger(enButtonX)) {
		GameClearSceneEnd();
		m_sceneType = SceneType::enTitle;
		TitleSceneStart();
	}
}

void SceneManager::PauseSceneStart()
{
	m_spriteManager = NewGO<SpriteManager>(0, "spriteManager");
	m_spriteManager->SetSpriteType(SpriteType::enSpriteType_Pause);
}

void SceneManager::PauseSceneUpdate()
{
}

void SceneManager::PauseSceneEnd()
{
	DeleteGO(m_spriteManager);
	m_spriteManager = nullptr;
}

void SceneManager::PauseSceneChange()
{
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		PauseSceneEnd();
		m_sceneType = SceneType::enPause;
		/** TODO::Œã‚Å’Ç‰Á */
	}
}


bool SceneManagerObject::Start()
{
	SceneManager::GetInstance()->Start();
	return true;
}

void SceneManagerObject::Update()
{
	SceneManager::GetInstance()->Update();
}

void SceneManagerObject::Render(RenderContext& rc)
{
}