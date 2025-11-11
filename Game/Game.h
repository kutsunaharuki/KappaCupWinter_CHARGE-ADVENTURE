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
class Scaffolding;

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void TimeDraw();
	void Render(RenderContext& rc);

	//時間制限。
	Vector3 m_timerFontPos = { -100.0f,520.0f,0.0f };
	//float m_timer = 120.0f;
	//時間の確認用の秒数。
	float m_timer = 5.0f;
	FontRender m_timerFontRender;
	//-----------------------------------

	LevelRender m_levelRender;
	std::vector<Stage1*> m_stage1s;
	std::vector<WarpHole*> m_warpHoles;
	std::vector<MovingFloor*> m_movingFloors;

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
	Scaffolding* m_scaffolding = nullptr;//足場。
};

