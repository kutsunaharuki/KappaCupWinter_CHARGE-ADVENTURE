#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Warp.h"
#include "MovingFloor.h"
#include "WarpHole.h"//ワープボックス(モデル)。
#include "SkyGround.h"
#include "Stage1.h"
#include "Enemy.h"
#include "ObstacleBox.h"
#include "Scaffolding.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_warp);
	DeleteGO(m_warpHole);
	DeleteGO(m_movingFloor);

	//レベルを消すためのやつ。
	/*for (auto m_stage1 : m_stage1s)
	{
		DeleteGO(m_stage1);
	}*/
	/*for (auto m_warpHole : m_warpHoles)
	{
		DeleteGO(m_warpHole);
	}*/
}

bool Game::Start()
{
	//プレイヤーのオブジェクトを作成する。
	m_player = NewGO<Player>(0, "player");

	//ゲームカメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//浮遊足場のオブジェクトを作成する。
	//m_skyGround = NewGO<SkyGround>(0, "skyGround");

	//ワープ(ゴーストオブジェクト)を作成する。
	//m_warp = NewGO<Warp>(0, "warp");

	//敵を作成する。
	//m_enemy = NewGO<Enemy1>(0, "enemy1");
	//m_enemy = NewGO<Enemy2>(0, "enemy2");
	//m_enemy = NewGO<Boss>(0, "boss");

	//仮の障害物(ボックス)を作成する。
	//m_obstacleBox = NewGO<ObstacleBox>(0, "obstacleBox");

	//高低差の足場。
	//m_scaffolding = NewGO<Scaffolding>(0, "scaffolding");

	//ワープボックスを作成する。
	//m_warpHole = NewGO<WarpHole>(0, "warpHole");

	//動く床を作成する。
	//m_movingFloor = NewGO<MovingFloor>(0, "movingFloor");

	//仮ステージを作成する。
	//m_stage1 = NewGO<Stage1>(0, "stage1");
	
	m_levelRender.Init("Assets/LevelRender/Stage00Level.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Stage01") == true)
		{
			//ステージ1を作成する。
			auto m_stage1 = NewGO<Stage1>(0, "stage1");

			m_stage1->m_Pos   = objData.position;
			m_stage1->m_rot   = objData.rotation;
			m_stage1->m_scale = objData.scale;

			m_stage1s.push_back(m_stage1);

			return true;
		}

		if (objData.EqualObjectName(L"MovingFloor") == true)
		{
			//動く床を作成する。
			auto m_movingFloor = NewGO<MovingFloor>(0, "movingFloor");

			m_movingFloor->m_speed = objData.position;
			m_movingFloor->m_firstPosition = objData.position;
			m_movingFloor->m_position = objData.position;
			m_movingFloor->m_movingFloorRotation = objData.rotation;
			m_movingFloor->m_movingSc = objData.scale;

			m_movingFloors.push_back(m_movingFloor);
			return true;
		}
		return false;
	});
	return true;
}

void Game::Update()
{
	TimeDraw();

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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
	/*if (m_timer <= 0.0f)
	{
		DeleteGO(this);
	}*/
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