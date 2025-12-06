#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Warp.h"
#include "MovingFloor.h"
#include "WarpHole.h"//ワープボックス(モデル)。
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
	
	DeleteGO(m_warp);
	m_warp = nullptr;
	
	DeleteGO(m_warpHole);
	m_warpHole = nullptr;
	
	DeleteGO(m_skyCube);
	m_skyCube = nullptr;
	
	DeleteGO(m_hpui);
	m_hpui = nullptr;
	
	DeleteGO(m_score);
	m_score = nullptr;

	for (auto frog : m_frogs) {
		DeleteGO(frog);
		frog = nullptr;
	}
	m_frogs.clear();

	for (auto skelton : m_skeltons) {
		DeleteGO(skelton);
		skelton = nullptr;
	}
	m_skeltons.clear();

	for (auto moving : m_movings) {
		DeleteGO(moving);
		moving = nullptr;
	}
	m_movings.clear();

	for (auto upDown : m_upDowns) {
		DeleteGO(upDown);
		upDown = nullptr;
	}
	m_upDowns.clear();

	for (auto poal : m_poals) {
		DeleteGO(poal);
		poal = nullptr;
	}
	m_poals.clear();

	for (auto secondGround : m_secondGrounds) {
		DeleteGO(secondGround);
		secondGround = nullptr;
	}
	m_secondGrounds.clear();

	for (auto sinkScaffold : m_sinkScaffolds) {
		DeleteGO(sinkScaffold);
		sinkScaffold = nullptr;
	}
	m_sinkScaffolds.clear();

	for (auto routeC : m_routeCs) {
		DeleteGO(routeC);
		routeC = nullptr;
	}
	m_routeCs.clear();
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
		return false;
	});

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
	//1-1のステージかボスステージの場合タイマーを描画する。
	if (m_gameState == GameState::Normal || m_gameState == GameState::BossStage)
	{
		TimeDraw();
		ItemDraw();
	}

	if (!m_player) {
		m_player = FindGO<Player>("player");
		return;
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


	if (!m_poal)
	{
		m_poal = FindGO<Poal>("poal");
	}
	if (m_poal->GetCollision()->IsHit(m_player->GetCharacterController())) {
		isQuick = true;
		if (isQuick) {
			DeleteGO(m_stage1);
			for (auto frog : m_frogs) {
				DeleteGO(frog);
				frog = nullptr;
			}
			for (auto skelton : m_skeltons) {
				DeleteGO(skelton);
				skelton = nullptr;
			}
			for (auto moving : m_movings) {
				DeleteGO(moving);
				moving = nullptr;
			}
			for (auto upDown : m_upDowns) {
				DeleteGO(upDown);
				upDown = nullptr;
			}
			for (auto poal : m_poals) {
				DeleteGO(poal);
				poal = nullptr;
			}
		}
		isQuick = false;
		if (!isQuick) {
			m_secondLevelRender.Init(BOSS_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData) {
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
				return false;
			});
		}
	}

	//auto型は推論なので、必要なのは右辺値が必要。
	
	//auto型の為に#includeは必要ない。
	//g_renderingEngine->DisableRaytracing();
}

void Game::ItemDraw()
{
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
	m_secondLevelRender.Draw(rc);
	//時間制限の描画。
	m_timerFontRender.Draw(rc);
	//アイテムの描画。
	m_itemFontRender.Draw(rc);
}