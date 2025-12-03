#pragma once
#include "Text.h"
class SpriteManager;
class Game;
class Player;
class Game;
class Stage;
class SceneManager
{
public:
	SpriteManager* m_spriteManager = nullptr;
	SceneType m_sceneType;


private:
	void SceneStateMachine();


private:
	void TitleSceneStart();
	void TitleSceneUpdate();
	void TitleSceneEnd();
	void TitleSceneChange();

private:
	void GameSceneStart();
	void GameSceneUpdate();
	void GameSceneEnd();
	void GameSceneChange();


private:
	void GameOverSceneStart();
	void GameOverSceneUpdate();
	void GameOverSceneEnd();
	void GameOverSceneChange();


private:
	void GameClearSceneStart();
	void GameClearSceneUpdate();
	void GameClearSceneEnd();
	void GameClearSceneChange();


private:
	void PauseSceneStart();
	void PauseSceneUpdate();
	void PauseSceneEnd();
	void PauseSceneChange();


private:
	SceneManager();
	~SceneManager();


public:
	bool Start();
	void Update();
	

public:
	static void CreateInstance(){
		if (!m_instance) {
			m_instance = new SceneManager;
		}
	}


	static void DeleteInstance(){
		if (m_instance) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
	
	
	static SceneManager* GetInstance(){
		return m_instance;
	}
	

private:
	static SceneManager* m_instance;


private:
	Game* m_game = nullptr;
	Stage* m_firstStage = nullptr;
	Player* m_player = nullptr;
};

class SceneManagerObject : public IGameObject
{
public:
	//static‚ÅŽg‚¤Žž‚Íclass–¼::
	SceneManagerObject() {
		SceneManager::CreateInstance();
	}

	~SceneManagerObject() {
		SceneManager::DeleteInstance();
	}


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
};