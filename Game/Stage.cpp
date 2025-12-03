#include "stdafx.h"
#include "Stage.h"
#include "Enemy.h"
#include "StageGround.h"
#include "Scaffolding.h"
#include "SkyGround.h"
#include "MovingFloor.h"
#include "MovingFloorUpDown.h"
#include "Poal.h"
#include "BossStage.h"

namespace {
	constexpr const char* FIRST_STAGE_LEVEL_FILE_PATH = "Assets/LevelRender/Stage1-1Level.tkl";
	constexpr const char* LAST_STAGE_LEVEL_FILE_PATH = "Assets/LevelRender/BossStageLevel.tkl";
}

Stage::Stage()
{

}

Stage::~Stage()
{

}

bool Stage::Start()
{
	return false;
}

void Stage::Update()
{

}

void Stage::Render(RenderContext& rc)
{

}

FirstStage::FirstStage()
{

}

FirstStage::~FirstStage()
{
	for (auto skelton : m_skeltons) {
		DeleteGO(skelton);
		skelton = nullptr;
	}
	for (auto frog : m_frogs) {
		DeleteGO(frog);
		frog = nullptr;
	}
	for (auto stageGround : m_stageGrounds) {
		DeleteGO(stageGround);
		stageGround = nullptr;
	}
	for (auto scaffolding : m_scaffols) {
		DeleteGO(scaffolding);
		scaffolding = nullptr;
	}
	for (auto skyGround : m_skyGrounds) {
		DeleteGO(skyGround);
		skyGround = nullptr;
	}
	for (auto movingFloor : m_movings) {
		DeleteGO(movingFloor);
		movingFloor = nullptr;
	}
	for (auto movingUpDown : m_upDowns) {
		DeleteGO(movingUpDown);
		movingUpDown = nullptr;
	}
	for (auto poal : m_poals) {
		DeleteGO(poal);
		poal = nullptr;
	}
}

bool FirstStage::Start()
{
	SetLevel();
	return true;
}

void FirstStage::Update()
{

}

void FirstStage::Render(RenderContext& rc)
{
	m_levelRender.Draw(rc);
}

void FirstStage::SetLevel()
{
	m_levelRender.Init(FIRST_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"Skelton"))
			{
				auto skelton = NewGO<Enemy2>(0, "skelton");

				skelton->SetTRS(
					objData.position,
					objData.scale,
					objData.rotation
				);
				m_skeltons.push_back(skelton);
				return true;
			}
			if (objData.EqualObjectName(L"Frogs"))
			{
				auto frog = NewGO<Enemy1>(0, "Frogs");

				frog->SetTRS(
					objData.position,
					objData.scale,
					objData.rotation
				);
				m_frogs.push_back(frog);
				return true;
			}
			if (objData.EqualObjectName(L"StageGround"))
			{
				auto stageGround = NewGO<StageGround>(0, "stageGround");
				stageGround->m_stageGroundPos = objData.position;
				stageGround->m_stageGroundSc = objData.scale;
				m_grounds.push_back(stageGround);
				return true;
			}
			if (objData.EqualObjectName(L"Koutei"))
			{
				auto scaffolding = NewGO<Scaffolding>(0, "scaffolding");
				scaffolding->m_scaffoldingPos = objData.position;
				scaffolding->m_scaffoldingRotation = objData.rotation;
				scaffolding->m_scaffoldingSc = objData.scale;
				m_scaffols.push_back(scaffolding);
				return true;
			}
			if (objData.EqualObjectName(L"SkyGround"))
			{
				auto skyGround = NewGO<SkyGround>(0, "skyGround");
				skyGround->m_skyGroundPos = objData.position;
				skyGround->m_skyGroundSc = objData.scale;
				m_skyGrounds.push_back(skyGround);
				return true;
			}
			if (objData.EqualObjectName(L"MovingFloor"))
			{
				auto movingFloor = NewGO<MovingFloor>(0, "movingFloor");
				movingFloor->m_speed = objData.position;
				movingFloor->m_firstPosition = objData.position;

				movingFloor->m_position = objData.position;
				movingFloor->m_movingFloorRotation = objData.rotation;
				movingFloor->m_movingSc = objData.scale;
				m_movings.push_back(movingFloor);
				return true;
			}
			if (objData.EqualObjectName(L"MovingFloorUpDown"))
			{
				auto movingUpDown = NewGO<MovingFloorUpDown>(0, "movingFloorUpDown");
				movingUpDown->m_movingSpeed = objData.position;
				movingUpDown->m_firstPos = objData.position;
				movingUpDown->m_movingRot = objData.rotation;
				movingUpDown->m_movingScale = objData.scale;
				m_upDowns.push_back(movingUpDown);
				return true;
			}
			if (objData.EqualObjectName(L"Poal"))
			{
				auto poal = NewGO<Poal>(0, "poal");
				poal->m_pos = objData.position;
				poal->m_rot = objData.rotation;
				poal->m_scale = objData.scale;
				m_poals.push_back(poal);
				return true;
			}
			return false;
		});

}

LastStage::LastStage()
{

}

LastStage::~LastStage()
{

}

bool LastStage::Start()
{
	SetLevel();
	return true;
}

void LastStage::Update()
{

}

void LastStage::Render(RenderContext& rc)
{
	m_levelRender.Draw(rc);
}

void LastStage::SetLevel()
{
	m_levelRender.Init(LAST_STAGE_LEVEL_FILE_PATH, [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"BossStage"))
		{
			auto bossStage = NewGO<BossStage>(0, "bossStage");
			bossStage->m_pos = objData.position;
			bossStage->m_sc = objData.scale;
			m_bossStagies.push_back(bossStage);
			return true;
		}
		if (objData.EqualObjectName(L"Bear"))
		{
			auto bear = NewGO<Boss>(0, "Bear");
			bear->SetTRS(
				objData.position,
				objData.scale,
				objData.rotation
			);
			m_bosses.push_back(bear);
			return false;
		}
		if (objData.EqualObjectName(L"Poal"))
		{
			auto poal = NewGO<Poal>(0, "poal");
			poal->Deactivate();
			poal->m_pos = objData.position;
			poal->m_scale = objData.scale;
			poal->m_rot = objData.rotation;
			m_poals.push_back(poal);
			return true;
		}
		return false;
	});

}
