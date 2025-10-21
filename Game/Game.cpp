#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Warp.h"
#include "MovingFloor.h"
#include "WarpHole.h"//���[�v�{�b�N�X(���f��)�B
#include "SkyGround.h"
#include "Stage1.h"
#include "Enemy1.h"
#include "Enemy2.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_warp);
	DeleteGO(m_warpHole);
	DeleteGO(m_movingFloor);

	for (auto m_stage1 : m_stage1s)
	{
		DeleteGO(m_stage1);
	}
	/*for (auto m_warpHole : m_warpHoles)
	{
		DeleteGO(m_warpHole);
	}*/
}

bool Game::Start()
{
	//�v���C���[�̃I�u�W�F�N�g���쐬����B
	m_player = NewGO<Player>(0, "player");

	//�Q�[���J�����̃I�u�W�F�N�g���쐬����B
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//���V����̃I�u�W�F�N�g���쐬����B
	//m_skyGround = NewGO<SkyGround>(0, "skyGround");

	//���[�v(�S�[�X�g�I�u�W�F�N�g)���쐬����B
	//m_warp = NewGO<Warp>(0, "warp");

	//�G1���쐬����B
	//m_enemy1 = NewGO<Enemy1>(0, "enemy1");

	//���[�v�{�b�N�X���쐬����B
	//m_warpHole = NewGO<WarpHole>(0, "warpHole");

	//���������쐬����B
	m_movingFloor = NewGO<MovingFloor>(0, "movingFloor");
	
	m_levelRender.Init("Assets/LevelRender/Stage1LevelMap.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Level00") == true)
		{
			//�X�e�[�W1���쐬����B
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