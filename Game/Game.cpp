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
	m_enemy = NewGO<Boss>(0, "boss");

	//ワープボックスを作成する。
	//m_warpHole = NewGO<WarpHole>(0, "warpHole");

	//動く床を作成する。
	//m_movingFloor = NewGO<MovingFloor>(0, "movingFloor");

	//仮ステージを作成する。
	//m_stage1 = NewGO<Stage1>(0, "stage1");
	
	m_levelRender.Init("Assets/LevelRender/Stage01Level.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Level00") == true)
		{
			//ステージ1を作成する。
			auto m_stage1 = NewGO<Stage1>(0, "stage1");

			m_stage1->m_Pos   = objData.position;
			m_stage1->m_scale = objData.scale;

			m_stage1s.push_back(m_stage1);

			return true;
		}
		return false;
	});

	return true;
}

void Game::Update()
{
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	// g_renderingEngine->DisableRaytracing();
}

void Game::Render(RenderContext& rc)
{
	m_levelRender.Draw(rc);
}