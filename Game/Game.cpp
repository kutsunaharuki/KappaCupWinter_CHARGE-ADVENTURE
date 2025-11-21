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
//#include "InGameTime.h"

namespace {
	const char* BOSS_STAGE_LEVEL = "Assets/LevelRender/BossStage.tkl";
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
	DeleteGO(m_movingFloor);
	m_movingFloor = nullptr;
	DeleteGO(m_skyCube);
	m_skyCube = nullptr;
	DeleteGO(m_hpui);
	m_hpui = nullptr;

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
	for (auto m_enemy : m_enemys)
	{
		DeleteGO(m_enemy);
		m_enemy = nullptr;
	}
	for (auto bossStage : m_bossStages)
	{
		DeleteGO(bossStage);
		bossStage = nullptr;
	}
	for (auto enemy : m_bossis)
	{
		DeleteGO(enemy);
		enemy = nullptr;
	}
	/*for (auto m_enemy2 : m_enemys)
	{
		DeleteGO(m_enemy2);
		m_enemy2 = nullptr;
	}*/
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
	InitSky();
	//プレイヤーのオブジェクトを作成する。
	m_player = NewGO<Player>(0, "player");

	//ゲームカメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//HPUIを作成する。
	m_hpui = NewGO<HPUI>(0, "hpui");

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

	m_levelRender.Init("Assets/LevelRender/Stage1-1Level.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Frogs") == true)
		{
			//Frog(Enemy1)を作成する。
			auto m_enemy = NewGO<Enemy1>(0,"Frogs");
		
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetScale(objData.scale);
			m_enemys.push_back(m_enemy);
			return true;
		}
		if (objData.EqualObjectName(L"StageGround") == true)
		{
			//最初に触れる足場。
			auto m_stageGround = NewGO<StageGround>(0, "stageGround");

			m_stageGround->m_stageGroundPos = objData.position;
			m_stageGround->m_stageGroundSc = objData.scale;
			m_stageGrounds.push_back(m_stageGround);
			return true;
		}
		if (objData.EqualObjectName(L"Koutei") == true)
		{
			//高低差のある段差の足場。
			auto m_scaffolding = NewGO<Scaffolding>(0, "scaffolding");
			
			m_scaffolding->m_scaffoldingPos = objData.position;
			m_scaffolding->m_scaffoldingSc = objData.scale;
			m_scaffoldings.push_back(m_scaffolding);
			return true;
		}
		if (objData.EqualObjectName(L"SkyGround") == true)
		{
			//重力のない足場。
			auto m_skyGround = NewGO<SkyGround>(0, "skyGround");

			m_skyGround->m_skyGroundPos = objData.position;
			m_skyGround->m_skyGroundSc = objData.scale;
			m_skyGrounds.push_back(m_skyGround);
			return true;
		}
		if (objData.EqualObjectName(L"MovingFloor") == true)
		{
			//Z方向に動く床を作成する。
			auto m_movingFloor = NewGO<MovingFloor>(0, "movingFloor");

			m_movingFloor->m_speed = objData.position;
			m_movingFloor->m_firstPosition = objData.position;
			m_movingFloor->m_position = objData.position;
			m_movingFloor->m_movingFloorRotation = objData.rotation;
			m_movingFloor->m_movingSc = objData.scale;
			m_movingFloors.push_back(m_movingFloor);
			return true;
		}
		if (objData.EqualObjectName(L"MovingFloorUpDown") == true)
		{
			//Y方向に動く床を作成する。
			auto m_movingFloorUpDown = NewGO<MovingFloorUpDown>(0, "movingFloorUpDown");

			m_movingFloorUpDown->m_movingSpeed = objData.position;
			m_movingFloorUpDown->m_firstPos = objData.position;
			m_movingFloorUpDown->m_movingRot = objData.rotation;
			m_movingFloorUpDown->m_movingScale = objData.scale;
			m_movingFloorUpDowns.push_back(m_movingFloorUpDown);
			return true;
		}
		if (objData.EqualObjectName(L"Poal") == true)
		{
			//ゴール条件のポールを作成する。
			auto m_poal = NewGO<Poal>(0, "poal");
			m_poal->m_pos = objData.position;
			m_poal->m_rot = objData.rotation;
			m_poal->m_scale = objData.scale;
			m_poals.push_back(m_poal);
			return true;
		}
		return false;
	});
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
	if(!m_poal)
	{
		m_poal = FindGO<Poal>("poal");
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
			for (auto m_enemy : m_enemys)
			{
				DeleteGO(m_enemy);
				m_enemy = nullptr;
			}
		}
		if (isQuick)
		{
			m_player->GetPosition();
			m_bossLevelRender.Init(BOSS_STAGE_LEVEL, [&](LevelObjectData& objectData) {
				if (objectData.EqualObjectName(L"BossStage") == true)
				{
					auto bossStage = NewGO<BossStage>(0, "bossStage");
					bossStage->m_pos = objectData.position;
					bossStage->m_sc = objectData.scale;
					m_bossStages.push_back(bossStage);
					return true;
				}
				if (objectData.EqualObjectName(L"Golem1") == true)
				{
					auto enemys = NewGO<Boss>(0, "Golem1");
					enemys->SetPosition(objectData.position);
					enemys->SetScale(objectData.scale);
					m_bossis.push_back(enemys);
					return true;
				}
				return false;
			});
		}
		isQuick = false;
	}

	/*if (!m_enemy && !m_poal)
	{
		m_enemy = FindGO<Enemy>("Golem1");
		m_poal = FindGO<Poal>("poal");
	}
	if (m_enemy->GetCollision()->IsHit(m_player->GetCharacterController()) && !isKill)
	{
		isKill = true;
		m_score->ScoreCalculator(500);
		if (isKill)
		{
			auto m_poal = NewGO<Poal>(0, "poal");
		}
		isKill = false;
	}*/

	//プレイヤーのHPが0になったら
	//ゲームオーバーにする処理。
	if (m_player->hp <= 0)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	//落下の処理。
	if (m_player->m_position.y <= -200.0f)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	//制限時間終了後の処理。
	if (m_timer <= 0.0f)
	{
		auto gameOver = FindGO<GameOver>("gameOver");
		gameOver->Activate();
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	//auto型は推論なので、必要なのは右辺値が必要。
	//auto型の為に#includeは必要ない。
	// g_renderingEngine->DisableRaytracing();
}

//時間制限の描画関数。
void Game::TimeDraw()
{
	int minutes = (int)m_timer / 60;
	int seconds = (int)m_timer % 60;
	float distance = g_gameTime->GetFrameDeltaTime();
	m_timer -= distance;

	wchar_t timer[256];
	swprintf_s(timer, 256, L"%02d:%02d", minutes, seconds);
	m_timerFontRender.SetText(timer);
	m_timerFontRender.SetPosition(m_timerFontPos);
	m_timerFontRender.SetScale(1.8f);

	//時間が0になったら終了。
	//今は実施しない。
	if (m_timer <= 0.0f)
	{
		m_timer = 0;
		return;
	}

}

void Game::Render(RenderContext& rc)
{
	//レベルの描画。
	m_levelRender.Draw(rc);
	//時間制限の描画。
	m_timerFontRender.Draw(rc);
}