#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class Warp;
class MovingFloor;
class WarpHole;
class SkyGround;
class Stage1;
class Enemy;
class ObstacleBox;

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
	Player    * m_player       = nullptr;//プレイヤー。
	GameCamera* m_gameCamera   = nullptr;//ゲームカメラ。
	Warp      * m_warp         = nullptr;//ワープ。
	MovingFloor* m_movingFloor = nullptr;//動く床。
	WarpHole   * m_warpHole    = nullptr;//ワープボックス。
	SkyGround  * m_skyGround   = nullptr;//浮遊足場。
	Stage1     * m_stage1      = nullptr;//ステージ1。
	Enemy      * m_enemy       = nullptr;//敵。
	ObstacleBox* m_obstacleBox = nullptr;//障害物(ボックス)。
};

