#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "MovingFloor.h"
#include "Enemy.h"
#include "ObstacleBox.h"
#include "MovingFloorUpDown.h"
#include "Poal.h"
#include "GameClear.h"
#include "GameOver.h"
#include "BossStage.h"
#include "HPUI.h"
#include "Score.h"
#include "SoundManager.h"
#include "SecondGround.h"
#include "RouteC.h"
#include "SinkScaffold.h"
#include "ChargeItem.h"
#include "Stage1.h"
#include "WarpBox.h"
#include "PauseScene.h"

/** 
 * 実体
 * static->名前空間::変数
 */
bool Game::m_isGoal = false;
//bool Game::m_isNextGoal = false;
bool Game::m_isPause = false;

namespace{
	const char* FIRST_STAGE_LEVEL_FILE_PATH = "Assets/LevelRender/Stage1-1Level.tkl";
	const char* BOSS_STAGE_LEVEL_FILE_PATH = "Assets/LevelRender/BossStageLevel.tkl";
	const char* SECOND_STAGE_LEVEL_FILE_PATH = "Assets/LevelRender/SecondStageLevel.tkl";
}

Game::~Game()
{	
	DeleteGO(m_player);
	m_player = nullptr;

	DeleteGO(m_gameCamera);
	m_gameCamera = nullptr;
	
	DeleteGO(m_skyCube);
	m_skyCube = nullptr;
	
	DeleteGO(m_hpui);
	m_hpui = nullptr;
	
	DeleteGO(m_enemy);
	m_enemy = nullptr;

	DeleteGO(m_score);
	m_score = nullptr;

	switch (m_gameState) {
	case GameState::Stage1:
		DeleteStage1();
		break;
	case GameState::Stage2:
		DeleteStage2();
		break;
	case GameState::BossStage:
		DeleteBossStage();
		break;
	}
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

	//ゲームカメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//HPUIを作成する。
	m_hpui = NewGO<HPUI>(0, "hpui");

	m_gameState = GameState::Stage1;
	CreateStage1();

	/** ファーストステージを作成 */
	//NewGO<FirstStage>(0, "firstStage");
    /** スコアを作成する */
	//m_score = NewGO<Score>(0, "score");
	//タイムを作成する。
	//m_inGameTime = NewGO<InGameTime>(0, "inGameTime");
	
	return true;
}

void Game::Update()
{
	if (Game::m_isPause && !m_pauseScene) {
		m_pauseScene = NewGO<PauseScene>(0);
	}
	if (!Game::m_isPause && m_pauseScene) {
		DeleteGO(m_pauseScene);
		m_pauseScene = nullptr;
		return;
	}

	
	if (m_isGoal) {
		DeleteStage1();
		CreateStage2();
		m_isGoal = false;
	}

	//1-1のステージかボスステージの場合タイマーを描画する。
	if (m_gameState == GameState::Stage1 || m_gameState == GameState::Stage2 || m_gameState == GameState::BossStage)
	{
		//時間の描画。
		TimeDraw();
		//アイテムの描画。
		ItemDraw();
	}

	if (m_player->m_position.y <= -200.0f) {
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}
	if (m_player->hp == 0) {
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}
	if (m_timer <= 0.0f) {
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	//auto型は推論なので、必要なのは右辺値が必要。

	//auto型の為に#includeは必要ない。
	//g_renderingEngine->DisableRaytracing();
}


void Game::CreateStage1()
{
	m_levelRender.Init(FIRST_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Stage1-1LevelCopy")) {
			m_stage1 = NewGO<Stage1>(0, "stage1");
			m_stage1->m_stage1Pos = objData.position;
			m_stage1->m_stage1Rot = objData.rotation;
			m_stage1->m_stage1Scale = objData.scale;
			return true;
		}
		if (objData.EqualObjectName(L"Frogs")) {
			auto frog = NewGO<Enemy1>(0, "Frogs");
			frog->SetTRS(
				objData.position,
				objData.scale,
				objData.rotation
			);;
			m_frogs.push_back(frog);
			return true;
		}
		if (objData.EqualObjectName(L"Skelton")) {
			auto skelton = NewGO<Enemy2>(0, "Skelton");
			skelton->SetTRS(
				objData.position,
				objData.scale,
				objData.rotation
			);
			m_skeltons.push_back(skelton);
			return true;
		}
		if (objData.EqualObjectName(L"MovingFloor")) {
			auto moving = NewGO<MovingFloor>(0, "movingFloor");
			moving->m_speed = objData.position;
			moving->m_firstPosition = objData.position;
			moving->m_position = objData.position;
			moving->m_movingFloorRotation = objData.rotation;
			moving->m_movingSc = objData.scale;
			m_movings.push_back(moving);
			return true;
		}
		if (objData.EqualObjectName(L"MovingFloorUpDown")) {
			auto upDown = NewGO<MovingFloorUpDown>(0, "movingFloorUpDown");
			upDown->m_movingPos = objData.position;
			upDown->m_movingSpeed = objData.position;
			upDown->m_movingRot = objData.rotation;
			upDown->m_movingScale = objData.scale;
			m_upDowns.push_back(upDown);
			return true;
		}
		if (objData.EqualObjectName(L"Poal")) {
			auto poal = NewGO<Poal>(0, "poal");
			poal->m_pos = objData.position;
			poal->m_rot = objData.rotation;
			poal->m_scale = objData.scale;
			m_poals.push_back(poal);
			return true;
		}
		if (objData.EqualObjectName(L"unityChan")) {
			m_player = NewGO<Player>(0, "player");
			m_player->SetTRS(
				objData.position,
				objData.rotation,
				objData.scale
			);
			return true;
		}
		return false;
		});
}

void Game::DeleteStage1()
{
	DeleteGO(m_stage1);
	for (auto& frog    : m_frogs)
	{ 
		DeleteGO(frog);    
		frog    = nullptr; 
	}
	for (auto& skelton : m_skeltons)  
	{ 
		DeleteGO(skelton);
		skelton = nullptr;
	}
	for (auto& moving  : m_movings)  
	{ 
		DeleteGO(moving);  
		moving  = nullptr;
	}
	for (auto& upDown  : m_upDowns)  
	{
		DeleteGO(upDown); 
		upDown  = nullptr;
	}
	for (auto& poal    : m_poals)    
	{
		DeleteGO(poal);
		poal    = nullptr;
	}
}


void Game::CreateStage2()
{
	m_secondLevelRender.Init(SECOND_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"SinkScaffold")) {
			auto sinkScaffold = NewGO<SinkScaffold>(0, "sinkScaffold");
			sinkScaffold->m_sinkScaffoldPos = objData.position;
			sinkScaffold->m_sinkScaffoldRot = objData.rotation;
			sinkScaffold->m_sinkScaffoldScale = objData.scale;
			m_sinkScaffolds.push_back(sinkScaffold);
			return true;
		}
		if (objData.EqualObjectName(L"SecondGround")) {
			m_secondGround = NewGO<SecondGround>(0, "secondGround");
			m_secondGround->m_secondGroundPos = objData.position;
			m_secondGround->m_secondGroundRot = objData.rotation;
			m_secondGround->m_secondGroundScale = objData.scale;
			return true;
		}
		if (objData.EqualObjectName(L"RouteC")) {
			auto routeC = NewGO<RouteC>(0, "routeC");
			routeC->m_routeCPos = objData.position;
			routeC->m_routeCRot = objData.rotation;
			routeC->m_routeCScale = objData.scale;
			m_routeCs.push_back(routeC);
			return true;
		}
		if (objData.EqualObjectName(L"ChargeItem")) {
			auto chargeItem = NewGO<ChargeItem>(0, "chargeItem");
			chargeItem->m_itemPos = objData.position;
			chargeItem->m_itemRot = objData.rotation;
			chargeItem->m_itemScale = objData.scale;
			m_chargeItems.push_back(chargeItem);
			return true;
		}
		if (objData.EqualObjectName(L"unityChan")) {
			if (m_gameCamera) {
				m_gameCamera->m_springCamera.Refresh();
			}
			m_player->SetTRS(
				objData.position,
				objData.rotation,
				objData.scale
			);
			m_player->GetModelRender()->SetTRS(
				objData.position,
				objData.rotation,
				objData.scale
			);
			m_player->GetCharacterController().SetPosition(objData.position);
			m_player->GetCharacterController().Execute(objData.position, 1.0f / 60.0f);
			m_player->GetModelRender()->Update();
			return true;
		}
		if (objData.EqualObjectName(L"WarpBox")) {
			//auto warpBox=NewGO<>
			return true;
		}
		return false;
	});
}


void Game::DeleteStage2()
{
	/** TODO:後で修正 */
	//DeleteGO(m_stage2);
	//m_stage2 = nullptr;

	DeleteGO(m_secondGround);
	m_secondGround = nullptr;

	for (auto& sinkScaffold : m_sinkScaffolds)
	{
		DeleteGO(sinkScaffold);
		sinkScaffold = nullptr;
	}

	for (auto& routeC : m_routeCs)
	{
		DeleteGO(routeC);
		routeC = nullptr;
	}

	for (auto& chargeItem : m_chargeItems) {
		DeleteGO(chargeItem);
		chargeItem = nullptr;
	}
}


void Game::CreateBossStage()
{
	m_bossLevelRender.Init(BOSS_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"BossStage")) {
			m_bossStage = NewGO<BossStage>(0, "bossStage");
			m_bossStage->m_pos = objData.position;
			m_bossStage->m_sc = objData.scale;
			return true;
		}
		if (objData.EqualObjectName(L"Bear")) {
			m_enemy = NewGO<Boss>(0, "Bear");
			m_enemy->SetTRS(
				objData.position,
				objData.scale,
				objData.rotation
			);
			return true;
		}
		if (objData.EqualObjectName(L"Skelton")) {
			auto skelton = NewGO<Enemy2>(0, "Skelton");
			skelton->SetTRS(
				objData.position,
				objData.scale,
				objData.rotation
			);
			return true;
		}
		if (objData.EqualObjectName(L"unityChan")) {
			if (m_gameCamera) {
				m_gameCamera->m_springCamera.Refresh();
			}

			m_player->SetTRS(
				objData.position,
				objData.rotation,
				objData.scale
			);
			m_player->GetModelRender()->SetTRS(
				objData.position,
				objData.rotation,
				objData.scale
			);
			m_player->GetCharacterController().SetPosition(objData.position);
			m_player->GetCharacterController().Execute(objData.position, 1.0f / 60.0f);
			m_player->GetModelRender()->Update();
			return true;
		}
		return false;
	});
}


void Game::DeleteBossStage()
{
	DeleteGO(m_bossStage);
	DeleteGO(m_enemy);
	for (auto& skelton : m_skeltons) {
		DeleteGO(skelton);
		skelton = nullptr;
	}
}


void Game::ItemDraw()
{
	if (!m_player) {
		return;
	}
	wchar_t itemText[256];
	swprintf_s(itemText, 256, L"ITEM:%d", m_player->m_itemCount);
	m_itemFontRender.SetText(itemText);
	m_itemFontRender.SetPosition(m_itemFontPos);
	m_itemFontRender.SetScale(1.8f);
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
		m_timer-=distance;
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
	m_levelRender.Draw(rc);
	//2ステージ目のレベルの描画。
	if (m_isGoal) {
		m_secondLevelRender.Draw(rc);
	}

	//ボスステージのレベルの描画。
	if (m_isGoal) {
		m_bossLevelRender.Draw(rc);
	}
	//時間制限の描画。
	m_timerFontRender.Draw(rc);
	//アイテムの描画。
	m_itemFontRender.Draw(rc);
}