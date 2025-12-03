#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Warp.h"
#include "MovingFloor.h"
#include "WarpHole.h"//ワープボックス(モデル)。
//#include "Stage1.h"
#include "Enemy.h"
#include "ObstacleBox.h"
#include "Scaffolding.h"
#include "MovingFloorUpDown.h"
#include "Asiba.h"
#include "SkyGround.h"
#include "StageGround.h"
#include "Poal.h"
#include "GameClear.h"
#include "GameOver.h"
#include "BossStage.h"
#include "HPUI.h"
#include "Score.h"
#include "SoundManager.h"
#include "Stage.h"

namespace{
	const char* BOSS_STAGE_LEVEL = "Assets/LevelRender/BossStageLevel.tkl";
}

Game::~Game()
{	
	DeleteGO(m_player);
	m_player = nullptr;

	DeleteGO(m_gameCamera);
	m_gameCamera = nullptr;
	
	DeleteGO(m_warp);
	m_warp = nullptr;
	
	DeleteGO(m_warpHole);
	m_warpHole = nullptr;
	
	DeleteGO(m_skyCube);
	m_skyCube = nullptr;
	
	DeleteGO(m_hpui);
	m_hpui = nullptr;
	
	DeleteGO(m_score);

	for (auto m_stageGround : m_stageGrounds)
	{
		DeleteGO(m_stageGround);
		m_stageGround = nullptr;
	}
	for (auto m_skyGround : m_skyGrounds)
	{
		DeleteGO(m_skyGround);
		m_skyGround = nullptr;
	}
	for (auto m_scaffolding : m_scaffoldings)
	{
		DeleteGO(m_scaffolding);
		m_scaffolding = nullptr;
	}
	for (auto m_movingFloorUpDown : m_movingFloorUpDowns)
	{
		DeleteGO(m_movingFloorUpDown);
		m_movingFloorUpDown = nullptr;
	}
	for (auto m_movingFloor : m_movingFloors)
	{
		DeleteGO(m_movingFloor);
		m_movingFloor = nullptr;
	}
	for (auto m_poal : m_poals)
	{
		DeleteGO(m_poal);
		m_poal = nullptr;
	}
	for (auto m_enemy : m_flogs)
	{
		DeleteGO(m_enemy);
		m_enemy = nullptr;
	}
	//m_enemies.clear();
	for (auto bossStage : m_bossStages)
	{
		DeleteGO(bossStage);
		bossStage = nullptr;
	}
	for (auto skelton : m_skelton) {
		DeleteGO(skelton);
		skelton = nullptr;
	}

	//for (auto bossStage : m_bossStages)
	//{
	//	DeleteGO(bossStage);
	//	bossStage = nullptr;
	//}
}

void Game::InitSky()
{
	//現在の空を破棄する。
	DeleteGO(m_skyCube);
	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	//スカイキューブの大きさを変更する。
	m_skyCube->SetScale(10000.0f);
	//明るさを設定する。
	m_skyCube->SetLuminance(0.6f);
	//スカイキューブを昼間に設定する。
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	//環境光の計算の為のIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);

	//環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);
}

bool Game::Start()
{
	/** ゲーム内の音 */
	SoundManager* sound = FindGO<SoundManager>("soundManager");
	m_gameBGM = sound->PlayingSound(Sound::enSound_GameBGM, true, 2.0f);
	
	InitSky();
	//プレイヤーのオブジェクトを作成する。
	m_player = NewGO<Player>(0, "player");

	//ゲームカメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//HPUIを作成する。
	m_hpui = NewGO<HPUI>(0, "hpui");

	/** ファーストステージを作成 */
	//NewGO<FirstStage>(0, "firstStage");

	/** スコアを作成する */
	//m_score = NewGO<Score>(0, "score");


	//タイムを作成する。
	//m_inGameTime = NewGO<InGameTime>(0, "inGameTime");
	
	//敵を作成する。
	//m_enemy = NewGO<Enemy1>(0, "enemy1");//Frog(カエル)。
	//m_enemy = NewGO<Enemy2>(0, "enemy2");//Penguin(ペンギン)。
	//m_enemy = NewGO<Boss>(0, "boss");//Gorem(ゴーレム)。

	//仮の障害物(ボックス)を作成する。
	//m_obstacleBox = NewGO<ObstacleBox>(0, "obstacleBox");

	//ワープボックスを作成する。
	//m_warpHole = NewGO<WarpHole>(0, "warpHole");
	//m_poal = FindGO<Poal>("poal");
	return true;
}

void Game::Update()
{
	//1-1のステージかボスステージの場合タイマーを描画する。
	if (m_gameState == GameState::Normal || m_gameState == GameState::BossStage)
	{
		TimeDraw();
	}


	/** 
	 * プレイヤーのキャラコンがゴールポールに
	 * 触れたら前のステージがdeleteされる
	 * ゴールポールがnullじゃないなら
	 * ゴールポールを呼ぶ。
	 */
	if (!m_poal)
	{
		m_poal = FindGO<Poal>("poal");
		return;
	}
	if (m_poal->GetCollision()->IsHit(m_player->GetCharacterController()) && !isQuick)
	{
		isQuick = true;
		DeleteGO(m_poal);
		if (isQuick) {
			for (auto m_stageGround : m_stageGrounds)
			{
				DeleteGO(m_stageGround);
				m_stageGround = nullptr;
			}
			for (auto m_skyGround : m_skyGrounds)
			{
				DeleteGO(m_skyGround);
				m_skyGround = nullptr;
			}
			for (auto m_scaffolding : m_scaffoldings)
			{
				DeleteGO(m_scaffolding);
				m_scaffolding = nullptr;
			}
			for (auto m_movingFloorUpDown : m_movingFloorUpDowns)
			{
				DeleteGO(m_movingFloorUpDown);
				m_movingFloorUpDown = nullptr;
			}
			for (auto m_movingFloor : m_movingFloors)
			{
				DeleteGO(m_movingFloor);
				m_movingFloor = nullptr;
			}
			for (auto m_poal : m_poals)
			{
				DeleteGO(m_poal);
				m_poal = nullptr;
			}
			for (auto m_enemy : m_flogs)
			{
				DeleteGO(m_enemy);
				m_enemy = nullptr;
				//m_enemies.clear();
			}
		}
		if (isQuick)
		{
			/*m_bossLevelRender.Init(BOSS_STAGE_LEVEL, [&](LevelObjectData& objData) {
				if (objData.EqualObjectName(L"BossStage") == true)
				{
					auto bossStage = NewGO<BossStage>(0, "bossStage");
					bossStage->m_pos = objData.position;
					bossStage->m_sc = objData.scale;
					m_bossStages.push_back(bossStage);
					return true;
				}
				if (objData.EqualObjectName(L"Bear") == true)
				{
					auto enemys = NewGO<Boss>(0, "Bear");
					enemys->SetPosition(objData.position);
					enemys->SetRotation(objData.rotation);
					enemys->SetScale(objData.scale);
					m_flogs.push_back(enemys);
					return true;
				}*/
				/*if (objData.EqualObjectName(L"Poal") == true)
				{
					auto poal = NewGO<Poal>(0, "poal");
					poal->m_pos = objData.position;
					poal->m_rot = objData.rotation;
					poal->m_scale = objData.scale;
					m_poals.push_back(poal);
					return false;
				}*/
			//	return false;
			//});
		}
		isQuick = false;

	}

	//プレイヤーのHPが0になったら
	//ゲームオーバーにする処理。
	if (m_player->hp <= 0)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		//m_gameOver = NewGO<GameOver>(0, "gameOver");
		/** TODO::追加 */
		SoundManager* sound = FindGO<SoundManager>("soundManager");
		m_gameOverSe = sound->PlayingSound(Sound::enSound_GameOverSe, false, 2.0f);
		DeleteGO(m_gameOverSe);
		DeleteGO(this);
	}

	//落下の処理。
	if (m_player->m_position.y <= -200.0f)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		//m_gameOver = NewGO<GameOver>(0, "gameOver");
		/** TODO::追加 */
		SoundManager* sound = FindGO<SoundManager>("soundManager");
		m_gameOverSe = sound->PlayingSound(Sound::enSound_GameOverSe, false, 2.0f);
		DeleteGO(m_gameOverSe);
		DeleteGO(this);
	}

	//制限時間終了後の処理。
	if (m_timer <= 0.0f)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		FindGO<GameOver>("gameOver")->Activate();
		//m_gameOver = NewGO<GameOver>(0, "gameOver");
		/** TODO::追加 */
		SoundManager* sound = FindGO<SoundManager>("soundManager");
		m_gameOverSe = sound->PlayingSound(Sound::enSound_GameOverSe, false, 2.0f);
		DeleteGO(m_gameOverSe);
		DeleteGO(this);
	}

	/** ゴールポールに当たったらクリア画面を出す */
	//if (m_poal->GetCollision()->IsHit(m_player->GetBodyCollision() ))
	//{
	//	auto gameClear = FindGO<GameClear>("gameClear");
	//	gameClear->Activate();
	//	m_gameClear = NewGO<GameClear>(0, "gameClear");
	//	DeleteGO(this);
	//}

	
	//auto型は推論なので、必要なのは右辺値が必要。
	//auto型の為に#includeは必要ない。
	//g_renderingEngine->DisableRaytracing();
}


/** スコアを描画するための関数 */
void Game::ScoreDraw()
{
	
}


//時間制限の描画関数。
void Game::TimeDraw()
{
	int minutes = (int)m_timer / 60;
	int seconds = (int)m_timer % 60;
	float distance = g_gameTime->GetFrameDeltaTime();
	m_timer -= distance;
	if (m_gameState == GameState::BossStage)
	{
		m_timer = 120.0f;
		return;
	}

	wchar_t timer[256];
	swprintf_s(timer, 256, L"%02d:%02d", minutes, seconds);
	m_timerFontRender.SetText(timer);
	m_timerFontRender.SetPosition(m_timerFontPos);
	m_timerFontRender.SetScale(1.8f);

	//時間が0になったら終了。
	//今は実施しない。
	m_timer = std::max<float>(m_timer, 0.0f);
	//参考演算子が  ?
	m_isTimeUp = m_timer <= 0.0f ? true : false;
}

void Game::Render(RenderContext& rc)
{
	//レベルの描画。
	//m_levelRender.Draw(rc);
	//時間制限の描画。
	m_timerFontRender.Draw(rc);
}