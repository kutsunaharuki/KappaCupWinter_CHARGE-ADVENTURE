#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class Warp;
class MovingFloor;
class WarpHole;
class SkyGround;
class Stage1;
class Enemy1;
class Enemy2;

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	LevelRender m_levelRender;
	std::vector<Stage1*> m_stage1s;
	std::vector<WarpHole*> m_warpHoles;

private:
	Player    * m_player       = nullptr;//�v���C���[�B
	GameCamera* m_gameCamera   = nullptr;//�Q�[���J�����B
	Warp      * m_warp         = nullptr;//���[�v�B
	MovingFloor* m_movingFloor = nullptr;//�������B
	WarpHole   * m_warpHole    = nullptr;//���[�v�{�b�N�X�B
	SkyGround  * m_skyGround   = nullptr;//���V����B
	Stage1     * m_stage1      = nullptr;//�X�e�[�W1�B
	Enemy1     * m_enemy1      = nullptr;//�G1�B
	Enemy2     * m_enemy2      = nullptr;//�G2�B
};

